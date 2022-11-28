CC = gcc
CFLAGS = -c -Wall -Werror -Wextra
CFLAGS_WC = -Wall -Werror -Wextra
all: clean s21_grep

s21_grep: s21_grep.o
	$(CC) $(CFLAGS_WC) s21_grep.o -o s21_grep

s21_grep.o:
	$(CC) $(CFLAGS) s21_grep.c

test:
	sh test_func_grep.sh
	sh test_leak_grep.sh

clean:
	rm -rf *.o *.out s21_grep

rebuild: clean all
