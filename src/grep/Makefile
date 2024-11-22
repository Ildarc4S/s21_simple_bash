# Определяем переменные
CC = gcc
FLAGS = -Wextra -Werror -Wall
SRC = grep.c ../utils/s21_getline.c
OUT = s21_grep

all : $(OUT)

$(OUT): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

rebuild: clean all