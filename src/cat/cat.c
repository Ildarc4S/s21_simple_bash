#include "cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/s21_getline.h"

int main(int argc, char** argv) {
  sCat(argc, argv);
  return 0;
}

void sCat(int argc, char** argv) {
  arguments args = {0};
  argumentsParser(&args, argc, argv);
  output(&args, argc, argv, &optind);
}

void argumentsParser(arguments* args, int argc, char* argv[]) {
  struct option long_options[] = {{"number", no_argument, NULL, 'n'},
                                  {"number-nonblank", no_argument, NULL, 'b'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {NULL, 0, NULL, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "ebnstvET", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        args->b = 1;
        break;
      case 'e':
        args->E = 1;
        args->v = 1;
        break;
      case 'E':
        args->E = 1;
        break;
      case 'n':
        args->n = 1;
        break;
      case 's':
        args->s = 1;
        break;
      case 't':
        args->T = 1;
        args->v = 1;
        break;
      case 'T':
        args->T = 1;
        break;
      case 'v':
        args->v = 1;
        break;
      default:
        args->error = 1;
        break;
    }
  }
  if (args->error) fprintf(stderr, "Flags error: use cat [OPTION] [FILE]...\n");
}

char argumentV(char symbol) {
  if (symbol == '\n' || symbol == '\t' || symbol == ' ') return symbol;
  if (symbol >= 0 && symbol < 32) {
    putchar('^');
    symbol += 64;
  } else if (symbol == 127) {
    putchar('^');
    symbol = '?';
  }
  return symbol;
}

void outline(arguments* args, char* line, int len) {
  for (int i = 0; i < len; i++) {
    if (args->E && line[i] == '\n') {
#ifdef __APPLE__
      if (args->b && line[0] == '\n') printf("%6s\t", "");
#endif
      putchar('$');
    }
    if (args->v) line[i] = argumentV(line[i]);
    if (args->T && line[i] == '\t') {
      putchar('^');
      line[i] = 'I';
    }
    putchar(line[i]);
  }
}

void printProcess(arguments* args, int* empty_string_count, char* line,
                  int line_len, int* number, char* prev) {
  if (args->s) {
    if (line[0] == '\n') {
      (*empty_string_count)++;
    } else
      *empty_string_count = 0;
  }
  if (((args->n && !args->b) || (args->b && line[0] != '\n')) &&
      (*empty_string_count) <= 1) {
#ifdef __linux__
    if (*prev == '\n') {
#endif
      printf("%6d\t", (*number)++);
#ifdef __linux__
    }
#endif
  }

  if ((*empty_string_count) <= 1) {
    outline(args, line, line_len);
  }
  *prev = line[line_len - 1];
}

void fileProcess(arguments* args, char* file_path, int* number, char* prev,
                 int* empty_string_count) {
  FILE* f = fopen(file_path, "r");
  if (!f) {
    fprintf(stderr, "cat: %s: No such file or directory\n", file_path);
  } else {
    char* line = NULL;
    size_t line_size = 0;
    int line_len = 0;
#ifdef __APPLE__
    *number = 1;
    *empty_string_count = 0;
#endif
    while ((line_len = s21_getline(&line, &line_size, f)) != -1) {
      printProcess(args, empty_string_count, line, line_len, number, prev);
    }
    free(line);
    fclose(f);
  }
}

void output(arguments* args, int argc, char** argv, int* optind) {
  if (!args->error) {
    int number = 1;
    int empty_string_count = 0;
    char prev = '\n';
    for (int i = (*optind); i < argc; i++) {
      fileProcess(args, argv[i], &number, &prev, &empty_string_count);
    }
  }
}
