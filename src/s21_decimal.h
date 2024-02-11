#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct {
  union {
    int data[4];
    unsigned int udata[4];
  };
} s21_decimal;

// ERROR CODES
#define FALSE 0;
#define TRUE 1;
#define OK 0;
#define TOO_BIG 1;
#define TOO_SMALL 2;
#define ZERO_DIVIDING 3;
#define CONVERT_ERROR 1;
#define CALC_ERROR 1;

// TASK FUNCTIONS
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result);
int s21_sub(s21_decimal v1, s21_decimal v2, s21_decimal *result);
int s21_mul(s21_decimal v1, s21_decimal v2, s21_decimal *result);
int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal *result);

int s21_is_less(s21_decimal v1, s21_decimal v2);
int s21_is_less_or_equal(s21_decimal v1, s21_decimal v2);
int s21_is_greater(s21_decimal v1, s21_decimal v2);
int s21_is_greater_or_equal(s21_decimal v1, s21_decimal v2);
int s21_is_equal(s21_decimal v1, s21_decimal v2);
int s21_is_not_equal(s21_decimal v1, s21_decimal v2);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// OTHER FUNCTIONS

#endif
