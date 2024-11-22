#ifndef S21_GETLINE_H
#define S21_GETLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t s21_getline(char **restrict linep, size_t *restrict linecapp,
                    FILE *restrict stream);
#endif  // S21_GETLINE_H