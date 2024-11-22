#ifndef CAT_H
#define CAT_H

typedef struct arguments {
  int b, n, s, E, T, v, error;
} arguments;

void sCat(int argc, char** argv);
void argumentsParser(arguments* args, int argc, char* argv[]);
char argumentV(char symbol);
void outline(arguments* args, char* line, int len);
void printProcess(arguments* args, int* empty_string_count, char* line,
                  int line_len, int* number, char* prev);
void fileProcess(arguments* args, char* file_path, int* number, char* prev,
                 int* empty_string_count);
void output(arguments* args, int argc, char** argv, int* optind);

#endif  // CAT_H