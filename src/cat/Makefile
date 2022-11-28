CC = gcc
CFLAGS = -c -Wall -Werror -Wextra
CFLAGS_WC = -Wall -Werror -Wextra
all: clean s21_cat

s21_cat: s21_cat.o catOptions.o
	$(CC) $(CFLAGS_WC) s21_cat.o catOptions.o -o s21_cat

s21_cat.o:
	$(CC) $(CFLAGS) s21_cat.c

catOptions.o:
	$(CC) $(CFLAGS) catOptions.c

test:
	sh test_func_cat.sh
	sh test_leak_cat.sh

clean:
	rm -rf *.o *.out s21_cat

rebuild: clean all
