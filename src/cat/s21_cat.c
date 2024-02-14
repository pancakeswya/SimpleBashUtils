#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define cat_putc(c)     \
  if (putchar(c) == EOF) \
    break                 \

#define cat_puts(s)     \
  if (printf(s) == EOF)  \
    break                 \

#define cat_printf(f, args...)  \
  if (printf(f, args) == EOF)    \
    break                         \

static enum Opts {
  OPT_B = 1u << 0,
  OPT_E = 1u << 1,
  OPT_N = 1u << 2,
  OPT_S = 1u << 3,
  OPT_T = 1u << 4,
  OPT_V = 1u << 5
} opts;

static struct option const long_options[] = {
    {
        .name = "number-nonblank",
        .has_arg = no_argument,
        .val = 'b'
    },
    {
        .name = "number",
        .has_arg = no_argument,
        .val = 'n'
    },
    {
        .name = "squeeze-blank",
        .has_arg = no_argument,
        .val = 's'
    }
};

static void parse_opt(int argc, char** argv) {
  for (;;) {
    switch (getopt_long(argc,
                        argv,
                        "benstuvAET",
                        long_options,
                        NULL)) {
      case -1:
        return;
      case 'b':
        opts |= (OPT_B | OPT_N);
        break;
      case 'e':
        opts |= (OPT_E | OPT_V);
        break;
      case 'n':
        opts |= OPT_N;
        break;
      case 's':
        opts |= OPT_S;
        break;
      case 't':
        opts |= (OPT_T | OPT_V);
        break;
      case 'u':
        setbuf(stdout, NULL);
        break;
      case 'v':
        opts |= OPT_V;
        break;
      case 'A':
        opts |= (OPT_E | OPT_T | OPT_V);
        break;
      case 'E':
        opts |= OPT_E;
        break;
      case 'T':
        opts |= OPT_T;
        break;
      default:
        fprintf(stderr, "usage: %s [-benstuv] [file ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
}

static void cat(const char *filename) {
  int c, prev_c = '\n';
  int count_str = 0;
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
    return;
  }
  int empty = 0;
  for (;(c = fgetc(file)) != EOF; prev_c = c) {
    if (prev_c == '\n') {
      if (opts & OPT_S) {
        if (c == '\n') {
          if (empty) {
            continue;
          }
          empty = 1;
        } else {
          empty = 0;
        }
      }
      if ((opts & OPT_N) &&
          (!(opts & OPT_B) || c != '\n')) {
        cat_printf("%6d\t", ++count_str);
      }
    }
    if (c == '\n') {
      if (opts & OPT_E) {
        cat_putc('$');
      }
    } else if (c == '\t') {
      if (opts & OPT_T) {
        cat_puts("^I");
        continue;
      }
    } else if (opts & OPT_V) {
      if (!isascii(c) && !isprint(c)) {
        cat_puts("M-");
        c = toascii(c);
      }
      if (iscntrl(c)) {
        cat_printf("^%c", (c == '\177') ? '?' : c | 0100);
        continue;
      }
    }
    cat_putc(c);
  }
  fclose(file);
}

int main(int argc, char* argv[]) {
  parse_opt(argc, argv);
  for(;optind < argc; ++optind) {
    cat(argv[optind]);
  }
  return 0;
}
