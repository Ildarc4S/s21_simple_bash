CC = gcc
FLAGS = -Wextra -Werror -Wall
SRC = cat.c ../utils/s21_getline.c
OUT = s21_cat 

all : $(OUT)

$(OUT): $(SRC)
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

rebuild: clean all