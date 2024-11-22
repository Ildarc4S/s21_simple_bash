#ifndef GREP_H
#define GREP_H

#include <regex.h>

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o, error;

  char* pattern;
  int len_pattern;
  int mem_len_pattern;
} arguments;

void sGrep(int argc, char** argv);
void patternAdd(arguments* args, char* pattern);
void addRegFromFile(arguments* args, char* path);
void argumentsParser(arguments* args, int argc, char** argv, int* optind);
void outputLine(char* line, int n);
void printMatch(arguments* args, regex_t* reg, char* line, int line_number,
                int* counter_for_file, char* path);
void processPrint(arguments* args, regex_t* reg, int* line_number, int* count,
                  char* line, int line_len, char* path, int* counter_for_file);
void processFile(arguments* args, char* path, regex_t* reg,
                 int* counter_for_file);
void output(arguments* args, int argc, char** argv, int* optind);
void argumentsCL(arguments* args, char* path, int count);
void processRemainArguments(arguments* args, int argc, char** argv,
                            int* optind);
void processArgs(arguments* args, regex_t* reg, int* line_number, char* line,
                 int line_len, char* path, int* counter_for_file);

#endif  // GREP_H