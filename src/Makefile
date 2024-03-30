CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
COVFLAGS=--coverage
OS=$(shell uname)

ifeq ($(OS), Linux)
	CHECK_FLAGS = -lcheck -lm -lsubunit
else
	CHECK_FLAGS = -lcheck
endif

all: s21_decimal.a

rebuild: clean s21_decimal.a

s21_decimal.a:
	$(CC) $(CFLAGS) -c s21_decimal.c
	ar rc s21_decimal.a s21_decimal.o
	ranlib s21_decimal.a
	rm -rf s21_decimal.o

test: s21_decimal.a
	$(CC) $(CFLAGS) test.c s21_decimal.a -o test $(CHECK_FLAGS)
	./test

gcov_report: s21_decimal.a
	gcc $(CFLAGS) $(COVFLAGS) test.c s21_decimal.c -o gcov $(CHECK_FLAGS)
	./gcov
	lcov -o gcov.info -c -d .
	genhtml -o report gcov.info
	open report/index.html
	rm -f gcov*

check:
	clang-format -n *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

ifeq ($(OS), Linux)
	valgrind --tool=memcheck --leak-check=yes ./test
else
	leaks -atExit -- ./test
endif

clean:
	rm -f s21_decimal.a test gcov*
	rm -rf report
