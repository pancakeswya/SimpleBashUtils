#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  bool flag_b;
  bool flag_e;
  bool flag_n;
  bool flag_s;
  bool flag_t;
  bool flag_v;
} cat_opt;

static void parse_opt(int argc, char** argv, cat_opt* opt) {
  int c_opt;
  static struct option const long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  while ((c_opt = getopt_long(argc, argv, "benstuvAET", long_options, NULL)) !=
         -1) {
    switch (c_opt) {
      case 'b':
        opt->flag_b = true;
        break;
      case 'e':
        opt->flag_e = true;
        opt->flag_v = true;
        break;
      case 'n':
        opt->flag_n = true;
        break;
      case 's':
        opt->flag_s = true;
        break;
      case 't':
        opt->flag_t = true;
        opt->flag_v = true;
        break;
      case 'u':
        setbuf(stdout, NULL);
        break;
      case 'v':
        opt->flag_v = true;
        break;
      case 'A':
        opt->flag_e = true;
        opt->flag_t = true;
        opt->flag_v = true;
        break;
      case 'E':
        opt->flag_e = true;
        break;
      case 'T':
        opt->flag_t = true;
        break;
      default:
        fprintf(stderr, "usage: %s [-benstuv] [file ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (opt->flag_b && opt->flag_n) {
    opt->flag_n = false;
  }
}

static void print_file(char *filename, cat_opt *opt) {
  int c, prev_c = '\n', count_empty = 0, count_str = 1;
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
  } else {
    while ((c = fgetc(file)) != EOF) {
      if (opt->flag_s && c == '\n' && prev_c == '\n') {
        count_empty++;
        if (count_empty > 1) {
          prev_c = c;
          continue;
        }
      } else if (c != '\n' && prev_c == '\n') {
        count_empty = 0;
      }
      if (prev_c == '\n' && (opt->flag_n || (opt->flag_b && c != '\n'))) {
        printf("%6d\t", count_str);
        count_str++;
      }
      prev_c = c;
      if (opt->flag_e && c == '\n') {
        printf("$");
      }
      if (opt->flag_t && c == '\t') {
        printf("^I");
        continue;
      }
      if (opt->flag_v && c != '\n' && c != '\t') {
        if (!isascii(c) && !isprint(c)) {
          printf("M-");
          c = toascii(c);
        }
        if (iscntrl(c)) {
          printf("^%c", (c == '\177') ? '?' : c | 0100);
          continue;
        }
      }
      printf("%c", c);
    }
  }
  fclose(file);
}

int main(int argc, char* argv[]) {
  cat_opt opt = {0};
  parse_opt(argc, argv, &opt);
  for(;optind < argc; optind++) {
    print_file(argv[optind], &opt);
  }
  return 0;
}
