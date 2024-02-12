#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#ifndef S21_DECIMAL_SIZE_IN_INTS
#define S21_DECIMAL_SIZE_IN_INTS 4
#endif

#ifndef S21_DECIMAL_TEST  // We internally link some functions, unless we are
                          // testing
#define S21_STATIC_KEYWORD static inline
#else
#define S21_STATIC_KEYWORD
#endif

typedef struct {
  union {
    unsigned int uint_data[S21_DECIMAL_SIZE_IN_INTS];
    unsigned char
        char_data[S21_DECIMAL_SIZE_IN_INTS * (sizeof(int) / sizeof(char))];
  };
} s21_decimal;

#define S21_DECIMAL_SIZE_IN_BITS (sizeof(s21_decimal) << 3)
#define S21_DECIMAL_UNIT_SIZE_IN_BITS (sizeof(int) << 3)

// ERROR CODES
#define FALSE 0
#define TRUE 1
#define OK 0
#define TOO_BIG 1
#define TOO_SMALL 2
#define ZERO_DIVIDING 3
#define CONVERT_ERROR 1
#define CALC_ERROR 1

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

S21_STATIC_KEYWORD unsigned int __s21_left_shift_one(s21_decimal value,
                                                     s21_decimal *result);

S21_STATIC_KEYWORD unsigned int __s21_left_shift(s21_decimal value,
                                                 unsigned int shift,
                                                 s21_decimal *result);

S21_STATIC_KEYWORD unsigned int __s21_right_shift_one(s21_decimal value,
                                                      s21_decimal *result);

S21_STATIC_KEYWORD unsigned int __s21_right_shift(s21_decimal value,
                                                  unsigned int shift,
                                                  s21_decimal *result);

s21_decimal s21_decimal_init(void);

S21_STATIC_KEYWORD unsigned int __s21_read_bits(const s21_decimal value,
                                                unsigned int bit_offset,
                                                unsigned int bit_count);

#define __s21_read_bit(value, bit_offset) __s21_read_bits(value, bit_offset, 1)

// Exponent is 1 byte wide, located in the second most significant byte
#define __s21_read_exponent(value) \
  __s21_read_bits(value, S21_DECIMAL_SIZE_IN_BITS - 16, 8)

// Sign is the most significant bit
#define __s21_read_sign(value) \
  __s21_read_bits(value, S21_DECIMAL_SIZE_IN_BITS - 1, 1)

S21_STATIC_KEYWORD int __s21_write_bits(s21_decimal *value, unsigned int data,
                                        unsigned int bit_offset,
                                        unsigned int bit_count);

#define __s21_write_exponent(value, data) \
  __s21_write_bits(value, data, S21_DECIMAL_SIZE_IN_BITS - 16, 8)

#define __s21_write_bit(value, bit_offset, data) \
  __s21_write_bits(value, data, bit_offset, 1)

// Return values:
// 0 - bit toggled successfully
// -bit_offset - bit_offset is too big
S21_STATIC_KEYWORD int __s21_toggle_bit(s21_decimal *value,
                                        unsigned int bit_offset);

#endif  // S21_DECIMAL_H
