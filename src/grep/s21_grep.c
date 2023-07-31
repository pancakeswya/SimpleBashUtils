#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER 10000

typedef struct {
  bool mul_f;
  bool empty;
  bool flag_e;
  bool flag_i;
  bool flag_v;
  bool flag_c;
  bool flag_l;
  bool flag_n;
  bool flag_h;
  bool flag_s;
  bool flag_f;
  bool flag_o;
} grep_opt;

static inline void pattern_e(int patt_count, char* pattern, grep_opt* opt) {
  if (patt_count) {
    strcat(pattern, "|");
  }
  if (!*optarg) {
    opt->empty = true;
    strcat(pattern, ".");
  } else {
    strcat(pattern, optarg);
  }
}

static int pattern_f(int patt_count, char* pattern, grep_opt* opt) {
  FILE *file;
  char line[BUFFER] = {0};
  if ((file = fopen(optarg, "r"))) {
    while (fgets(line, BUFFER, file)) {
      strtok(line, "\n");
      if (patt_count) {
        strcat(pattern, "|");
      }
      if (*line == '\0') {
        opt->empty = true;
        strcat(pattern, ".");
      } else {
        strcat(pattern, line);
      }
      patt_count++;
    }
    fclose(file);
  } else if (!opt->flag_s) {
    fprintf(stderr, "s21_grep: %s: No such file\n", optarg);
  }
  return patt_count;
}

static inline bool flag_greater(const char* flag_1, const char* flag_2) {
  return ((flag_1 && flag_2) && (flag_2 < flag_1)) || flag_1;
}

static int get_file_index(int argc, char** argv, grep_opt* opt) {
  int i = 1;
  char *flag_e, *flag_f;
  for (; i < argc && (argv[i][0] == '-'); i++) {
    flag_e = strchr(argv[i], 'e');
    flag_f = strchr(argv[i], 'f');
    if (flag_greater(flag_f,flag_e)) {
      flag_f++;
      if (!*flag_f) {
        i++;
      }
    }
    if (flag_greater(flag_e,flag_f)) {
      flag_e++;
      if (!*flag_e) {
        i++;
      }
    }
  }
  if (!opt->flag_e && !opt->flag_f) {
    i++;
  }
  return i;
}

static void print_matches(FILE *file, char* restrict pattern, char* restrict filename, grep_opt* opt) {
  bool match;
  char *word;
  char str[BUFFER] = {0};
  int compare = (opt->flag_i) ? REG_ICASE : REG_EXTENDED;
  int count_matches = 0, count_str = 1;
  regex_t reg;
  regcomp(&reg, pattern, compare);
  regmatch_t str_match[1];
  while (fgets(str, BUFFER, file)) {
    match = false;
    int success = regexec(&reg, str, 1, str_match, 0);
    if (!strchr(str, '\n')) {
      strcat(str, "\n");
    }
    if ((!success && !opt->flag_v) ||
        (success == REG_NOMATCH && opt->flag_v)) {
      match = true;
    }
    if (match && !opt->flag_l && !opt->flag_c) {
      if (opt->mul_f && !opt->flag_h) {
        printf("%s:", filename);
      }
      if (opt->flag_n) {
        printf("%d:", count_str);
      }
      if (!opt->flag_o || (opt->flag_o && opt->flag_v)) {
        printf("%s", str);
      }
      if (opt->flag_o && !opt->flag_v) {
        word = str;
        while (!success) {
          for (int i = str_match[0].rm_so; i < str_match[0].rm_eo; i++) {
            printf("%c", word[i]);
          }
          printf("\n");
          for (int i = 0; i != str_match[0].rm_eo; word++, i++);
          success = regexec(&reg, word, 1, str_match, 0);
        }
      }
    }
    count_matches += match;
    count_str++;
  }
  if (opt->flag_c && opt->flag_l && count_matches) {
    count_matches = 1;
  }
  if (opt->flag_c) {
    if (opt->mul_f && !opt->flag_h) {
      printf("%s:", filename);
    }
    printf("%d\n", count_matches);
  }
  if (opt->flag_l && count_matches) {
    printf("%s\n", filename);
  }
  regfree(&reg);
}

static void grep_pattern(int argc, char** argv, char* pattern, grep_opt* opt) {
  FILE *file;
  for (int i = get_file_index(argc, argv, opt); i < argc; i++) {
    if ((file = fopen(argv[i], "r"))) {
      print_matches(file, pattern, argv[i], opt);
      fclose(file);
    } else if (!(opt->flag_s)) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[i]);
    }
  }
}

static void parse_opt(int argc, char** argv, char* pattern, grep_opt* opt) {
  int option, patt_count = 0, file_index;
  while ((option = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (option) {
      case 'e':
        pattern_e(patt_count++, pattern, opt);
        opt->flag_e = true;
        break;
      case 'i':
        opt->flag_i = true;
        break;
      case 'v':
        opt->flag_v = true;
        break;
      case 'c':
        opt->flag_c = true;
        break;
      case 'l':
        opt->flag_l = true;
        break;
      case 'n':
        opt->flag_n = true;
        break;
      case 'h':
        opt->flag_h = true;
        break;
      case 's':
        opt->flag_s = true;
        break;
      case 'f':
        patt_count = pattern_f(patt_count, pattern, opt);
        opt->flag_f = true;
        break;
      case 'o':
        opt->flag_o = true;
        break;
      default:
        fprintf(stderr, "usage: %s [-eivclnhsfo] [file ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  file_index = get_file_index(argc, argv, opt);
  if (!(opt->flag_e) && !(opt->flag_f)) {
    if (argv[file_index - 1][0]) {
      strcat(pattern, argv[file_index - 1]);
    } else {
      strcat(pattern, ".");
    }
  }
  if (file_index < argc - 1 && !opt->flag_h) {
    opt->mul_f = true;
  }
  if (opt->empty) {
    opt->flag_o = false;
  }
}

int main(int argc, char* argv[]) {
  grep_opt opt = {0};
  char pattern[BUFFER] = {0};
  if (argc > 2) {
    parse_opt(argc, argv, pattern, &opt);
    grep_pattern(argc, argv, pattern, &opt);
  } else {
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
  }
  return 0;
}