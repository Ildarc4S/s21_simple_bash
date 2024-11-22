#include "grep.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/s21_getline.h"

int main(int argc, char** argv) {
  sGrep(argc, argv);
  return 0;
}

void sGrep(int argc, char** argv) {
  arguments args = {0};
  argumentsParser(&args, argc, argv, &optind);
  if (!args.error) {
    output(&args, argc, argv, &optind);
  }
  free(args.pattern);
}

void patternAdd(arguments* args, char* pattern) {
  if (args->len_pattern == 0) {
    args->pattern = malloc(1024 * sizeof(char));
    if (args->pattern == NULL) {
      args->error = 1;
    } else {
      args->pattern[0] = '\0';
      args->mem_len_pattern = 1024;
    }
  }
  if (!args->error) {
    if (args->mem_len_pattern < args->len_pattern + (int)strlen(pattern) + 2) {
      char* pattern_alloc = realloc(args->pattern, args->mem_len_pattern * 2);
      if (pattern_alloc == NULL) {
        args->error = 1;
      } else {
        args->mem_len_pattern *= 2;
      }
    }
  }

  if (!args->error && args->len_pattern != 0) {
    strcat(args->pattern + args->len_pattern, "|");
    args->len_pattern++;
  }
  if (!args->error) {
    args->len_pattern +=
        sprintf(args->pattern + args->len_pattern, "(%s)", pattern);
  }
}

void addRegFromFile(arguments* args, char* path) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    if (!args->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", path);
    args->error = 1;
  } else {
    char* line = NULL;
    size_t memlen = 0;
    int line_number = 0;
    while (!args->error && (line_number = getline(&line, &memlen, f)) != -1) {
      if (line[line_number - 1] == '\n') line[line_number - 1] = '\0';
      patternAdd(args, line);
    }
    free(line);
    fclose(f);
  }
}

void processRemainArguments(arguments* args, int argc, char** argv,
                            int* optind) {
  if (args->len_pattern == 0 && *optind < argc) {
    patternAdd(args, argv[(*optind)++]);
  }

  if (!args->error && argc - *optind == 1) {
    args->h = 1;
  }
}

void argumentsParser(arguments* args, int argc, char** argv, int* optind) {
  int opt;
  while (!args->error && (opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        args->e = 1;
        patternAdd(args, optarg);
        break;
      case 'i':
        args->i = REG_ICASE;
        break;
      case 'v':
        args->v = 1;
        break;
      case 'c':
        args->c = 1;
        break;
      case 'l':
        args->l = 1;
        break;
      case 'n':
        args->n = 1;
        break;
      case 'h':
        args->h = 1;
        break;
      case 's':
        args->s = 1;
        break;
      case 'f':
        args->f = 1;
        addRegFromFile(args, optarg);
        break;
      case 'o':
        args->o = 1;
        break;
      default:
        args->error = 1;
        break;
    }
  }
  if (!args->error) processRemainArguments(args, argc, argv, optind);
}

void outputLine(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

void printMatch(arguments* args, regex_t* reg, char* line, int line_number,
                int* counter_for_file, char* path) {
  regmatch_t match;
  int offset = 0;
  int stop = 0;
  while (!stop) {
    int result = regexec(reg, line + offset, 1, &match, 0);
    if (result != 0) {
      stop = 1;
    } else {
      if (*counter_for_file > 0 && !args->h) printf("%s:", path);
      if (args->n) printf("%d:", line_number);
      for (int i = match.rm_so; i < match.rm_eo; i++) {
        putchar(line[offset + i]);
      }
      putchar('\n');
      offset += match.rm_eo;
    }
  }
}

void processArgs(arguments* args, regex_t* reg, int* line_number, char* line,
                 int line_len, char* path, int* counter_for_file) {
  (*counter_for_file)++;
  if (!args->c && !args->l) {
    if (!args->h && (*counter_for_file) > 0) {
      if (!args->o
#ifdef __APPLE__
          || (args->v && args->o)
#endif
      )
        printf("%s:", path);
    }
    if (args->n && !args->o) printf("%d:", *line_number);
    if (args->o
#ifdef __APPLE__
        && !args->v
#endif
    ) {
      printMatch(args, reg, line, *line_number, counter_for_file, path);
    } else {
      outputLine(line, line_len);
    }
  }
}

void processPrint(arguments* args, regex_t* reg, int* line_number, int* count,
                  char* line, int line_len, char* path, int* counter_for_file) {
  int result = regexec(reg, line, 0, NULL, 0);
  if (args->v) {
    result = !result;
  }
  if (result == 0) {
    processArgs(args, reg, line_number, line, line_len, path, counter_for_file);
    (*count)++;
  }
  (*line_number)++;
}

void argumentsCL(arguments* args, char* path, int count) {
  if (args->l && args->c) {
    if (!args->h) {
#ifdef __linux__
      if (!args->l) {
#endif
        printf("%s:", path);
#ifdef __linux__
      }
#endif
    }
#ifdef __APPLE__
    if (count > 0)
      printf("1\n");
    else
      printf("0\n");
#endif
  } else if (args->c) {
    if (!args->h) printf("%s:", path);
    printf("%d\n", count);
  }
  if (args->l && count > 0) {
    printf("%s\n", path);
  }
}

void processFile(arguments* args, char* path, regex_t* reg,
                 int* counter_for_file) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    if (!args->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", path);
  } else {
    char* line = NULL;
    size_t memlen = 0;
    int line_len = 0;
    int line_number = 1;
    int count = 0;
    while ((line_len = s21_getline(&line, &memlen, f)) != -1) {
      processPrint(args, reg, &line_number, &count, line, line_len, path,
                   counter_for_file);
    }
    argumentsCL(args, path, count);
    free(line);
    fclose(f);
  }
}

void output(arguments* args, int argc, char** argv, int* optind) {
  regex_t re;
  int error = regcomp(&re, args->pattern, REG_EXTENDED | args->i);
  if (error) perror("Error!");
  int counter_for_file = 0;
  for (int i = (*optind); i < argc; i++) {
    if (!args->error) {
      processFile(args, argv[i], &re, &counter_for_file);
    }
  }
  regfree(&re);
}
