#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#ifndef S21_DECIMAL_SIZE_IN_INTS
#define S21_DECIMAL_SIZE_IN_INTS 4
#endif

#ifndef S21_MAX_DECIMAL_EXPONENT
#define S21_MAX_DECIMAL_EXPONENT 28
#endif

#ifndef S21_DECIMAL_TEST  // We internally link some functions, unless we are
                          // testing
#define S21_STATIC_KEYWORD static inline
#else
#define S21_STATIC_KEYWORD
#endif

/* Most significant word MSB-LSB: 1 bit sign, 7 unused bits, 8 bits exponent,
 * 16 unused bits, other words contain mantissa
 */
typedef struct {
  union {
    unsigned int uint_data[S21_DECIMAL_SIZE_IN_INTS];
    // unsigned char
    // char_data[S21_DECIMAL_SIZE_IN_INTS * (sizeof(int) / sizeof(char))];
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

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

S21_STATIC_KEYWORD void __s21_swap(unsigned int *a, unsigned int *b);

S21_STATIC_KEYWORD unsigned int __s21_level(unsigned int a[], unsigned int b[],
                                            unsigned int a_extended[],
                                            unsigned int b_extended[],
                                            unsigned int intfield_size);

S21_STATIC_KEYWORD int __s21_add_intfield(const unsigned int v1[],
                                          const unsigned int v2[],
                                          unsigned int result[],
                                          unsigned int intfield_size);

S21_STATIC_KEYWORD int __s21_mul_intfield(const unsigned int v1[],
                                          const unsigned int v2[],
                                          unsigned int result[],
                                          unsigned int intfield_size);

S21_STATIC_KEYWORD unsigned int __s21_left_shift_intfield_one(
    const unsigned int value[], unsigned int result[],
    unsigned int intfield_size);

S21_STATIC_KEYWORD unsigned int __s21_left_shift_intfield(
    const unsigned int value[], unsigned int shift, unsigned int result[],
    unsigned int intfield_size);

S21_STATIC_KEYWORD unsigned int __s21_right_shift_intfield_one(
    const unsigned int value[], unsigned int result[],
    unsigned int intfield_size);

S21_STATIC_KEYWORD unsigned int __s21_right_shift_intfield(
    const unsigned int value[], unsigned int shift, unsigned int result[],
    unsigned int intfield_size);

void *s21_memset(void *data, char value, unsigned int size);

void *s21_memcpy(void *dest, const void *src, unsigned int size);

S21_STATIC_KEYWORD unsigned int __s21_read_bits(const unsigned int value[],
                                                const unsigned int bit_offset,
                                                const unsigned int bit_count);

#define __s21_read_bit(value, bit_offset) __s21_read_bits(value, bit_offset, 1)

// Exponent is the second most significant byte
#define __s21_read_exponent(data) \
  (data[S21_DECIMAL_SIZE_IN_INTS - 1] >> 16 & 0xFF)

// Sign is the most significant bit
#define __s21_decimal_sign(data) (data[S21_DECIMAL_SIZE_IN_INTS - 1] >> 31 & 1)

S21_STATIC_KEYWORD int __s21_write_bits(unsigned int value[],
                                        const unsigned int data,
                                        const unsigned int bit_offset,
                                        const unsigned int bit_count);

#define __s21_write_exponent(value, data) \
  __s21_write_bits(value, data, S21_DECIMAL_SIZE_IN_BITS - 16, 8)

#define __s21_write_bit(value, bit_offset, data) \
  __s21_write_bits(value, data, bit_offset, 1)

// Return values:
// 0 - bit toggled successfully
// -bit_offset - bit_offset is invalid
S21_STATIC_KEYWORD int __s21_toggle_bit(unsigned int value[],
                                        const unsigned int bit_offset);

S21_STATIC_KEYWORD int __s21_get_top_bit(const unsigned int value[],
                                         unsigned int intfield_size);

#endif  // S21_DECIMAL_H
