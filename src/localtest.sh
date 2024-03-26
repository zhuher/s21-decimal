#!/usr/bin/env bash
gcc -Wall -Wextra -Werror -Wpedantic -Wshift-count-overflow -o local/privet s21_decimal.c local/test.c -I. -DS21_DECIMAL_TEST && ./local/privet && rm -f local/privet