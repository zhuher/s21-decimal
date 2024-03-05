#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdint.h>

#ifndef S21_DECIMAL_SIZE_IN_INTS
#define S21_DECIMAL_SIZE_IN_INTS 4
#endif

#define S21_MAX_DECIMAL_EXPONENT 28

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
  // union {
  uint32_t uint_data[S21_DECIMAL_SIZE_IN_INTS];
  // };
} s21_decimal;

static const uint32_t powers_of_ten[29][(S21_DECIMAL_SIZE_IN_INTS - 1) << 1] = {
    {
        0x1,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0xA,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x64,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x3E8,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2710,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x186A0,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0xF4240,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x989680,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x5F5E100,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x3B9ACA00,
        0x0,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x540BE400,
        0x2,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x4876E800,
        0x17,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0xD4A51000,
        0xE8,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x4E72A000,
        0x918,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x107A4000,
        0x5AF3,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0xA4C68000,
        0x38D7E,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x6FC10000,
        0x2386F2,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x5D8A0000,
        0x1634578,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0xA7640000,
        0xDE0B6B3,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x89E80000,
        0x8AC72304,
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x63100000,
        0x6BC75E2D,
        0x5,
        0x0,
        0x0,
        0x0,
    },
    {
        0xDEA00000,
        0x35C9ADC5,
        0x36,
        0x0,
        0x0,
        0x0,
    },
    {
        0xB2400000,
        0x19E0C9BA,
        0x21E,
        0x0,
        0x0,
        0x0,
    },
    {
        0xF6800000,
        0x2C7E14A,
        0x152D,
        0x0,
        0x0,
        0x0,
    },
    {
        0xA1000000,
        0x1BCECCED,
        0xD3C2,
        0x0,
        0x0,
        0x0,
    },
    {
        0x4A000000,
        0x16140148,
        0x84595,
        0x0,
        0x0,
        0x0,
    },
    {
        0xE4000000,
        0xDCC80CD2,
        0x52B7D2,
        0x0,
        0x0,
        0x0,
    },
    {
        0xE8000000,
        0x9FD0803C,
        0x33B2E3C,
        0x0,
        0x0,
        0x0,
    },
    {
        0x10000000,
        0x3E250261,
        0x204FCE5E,
        0x0,
        0x0,
        0x0,
    },
};

#define S21_DECIMAL_SIZE_IN_BITS (sizeof(s21_decimal) << 3)

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

S21_STATIC_KEYWORD void __s21_swap(uint32_t *a, uint32_t *b);

S21_STATIC_KEYWORD uint32_t __s21_level(s21_decimal *a, s21_decimal *b,
                                        uint32_t a_double_mantissa[],
                                        uint32_t b_double_mantissa[],
                                        uint32_t intfield_size,
                                        uint32_t *old_exp);

S21_STATIC_KEYWORD int __s21_add_intfield(const uint32_t operand1[],
                                          const uint32_t operand2[],
                                          uint32_t result[],
                                          uint32_t intfield_size);

S21_STATIC_KEYWORD int __s21_sub_intfield(const uint32_t operand1[],
                                          const uint32_t operand2[],
                                          uint32_t result[],
                                          uint32_t intfield_size);

S21_STATIC_KEYWORD int __s21_mul_intfield(const uint32_t operand1[],
                                          const uint32_t operand2[],
                                          uint32_t result[],
                                          uint32_t intfield_size);

S21_STATIC_KEYWORD void __s21_left_shift_intfield(const uint32_t operand[],
                                                  uint32_t shift,
                                                  uint32_t result[],
                                                  uint32_t intfield_size);

S21_STATIC_KEYWORD void __s21_right_shift_intfield(const uint32_t operand[],
                                                   uint32_t shift,
                                                   uint32_t result[],
                                                   uint32_t intfield_size);

S21_STATIC_KEYWORD void __s21_negate_intfield(const uint32_t operand[],
                                              uint32_t result[],
                                              uint32_t intfield_size);

S21_STATIC_KEYWORD void __s21_2s_complement_intfield(const uint32_t operand[],
                                                     uint32_t result[],
                                                     uint32_t intfield_size);

S21_STATIC_KEYWORD void *s21_memset(void *data, uint8_t value, uint32_t size);

S21_STATIC_KEYWORD void *s21_memcpy(void *dest, const void *src, uint32_t size);

S21_STATIC_KEYWORD int16_t s21_rmemcmp(const void *lhs, const void *rhs,
                                       uint32_t size);

S21_STATIC_KEYWORD uint32_t __s21_read_bits(const uint32_t data[],
                                            const uint32_t bit_offset,
                                            const uint32_t bit_count);

#define __s21_read_bit(data, bit_offset) __s21_read_bits(data, bit_offset, 1)

// Exponent is the second most significant byte
#define __s21_read_exponent(decimal) \
  ((decimal).uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] >> 16 & 0xFF)

// Sign is the most significant bit
#define __s21_decimal_sign(decimal) \
  ((decimal).uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] >> 31 & 0x1)

S21_STATIC_KEYWORD void __s21_write_bits(uint32_t data[], const uint32_t value,
                                         const uint32_t bit_offset,
                                         const uint32_t bit_count);

#define __s21_write_exponent(data, value) \
  __s21_write_bits(data, value, S21_DECIMAL_SIZE_IN_BITS - 16, 8)

#define __s21_write_bit(data, bit_offset, value) \
  __s21_write_bits(data, value, bit_offset, 1)

S21_STATIC_KEYWORD int __s21_toggle_bit(uint32_t data[],
                                        const uint32_t bit_offset);

S21_STATIC_KEYWORD uint32_t __s21_get_top_bit_pos(const uint32_t data[],
                                                  uint32_t intfield_size);

#endif  // S21_DECIMAL_H
