#include "s21_getline.h"

ssize_t s21_getline(char **restrict linep, size_t *restrict linecapp,
                    FILE *restrict stream) {
  size_t result_len = 0;
  int c;
  ssize_t return_value = 0;

  if (*linep == NULL) {
    *linecapp = 256;
    *linep = malloc(*linecapp);
    if (*linep == NULL) {
      return_value = -1;
    }
  }

  int stop = 0;
  while (!stop && return_value != -1 && (c = fgetc(stream)) != EOF) {
    if (result_len + 1 >= *linecapp) {
      *linecapp *= 2;
      char *new_linep = realloc(*linep, *linecapp);
      if (new_linep == NULL) {
        return_value = -1;
        stop = 1;
      }
      if (!stop) *linep = new_linep;
    }

    if (!stop) {
      (*linep)[result_len++] = (char)c;

      if (c == '\n') {
        stop = 1;
      }
    }
  }
  if (result_len == 0 && c == EOF) {
    return_value = -1;
  } else {
    (*linep)[result_len] = '\0';
    return_value = result_len;
  }

  return return_value;
}