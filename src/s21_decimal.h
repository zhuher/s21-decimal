#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdint.h>

#ifndef S21_DECIMAL_SIZE_IN_INTS
#define S21_DECIMAL_SIZE_IN_INTS 4
#endif

#define S21_MAX_DECIMAL_EXPONENT 28

/* Most significant word MSB-LSB: 1 bit sign, 7 unused bits, 8 bits exponent,
 * 16 unused bits, other words contain mantissa
 */
typedef struct {
  uint32_t bits[S21_DECIMAL_SIZE_IN_INTS];
} s21_decimal;

#define S21_NULL ((void *)0)

typedef unsigned long s21_size_t;

#define S21_MANTISSA_SIZE (S21_DECIMAL_SIZE_IN_INTS - 1)
#define S21_DOUBLE_MANTISSA_SIZE (S21_MANTISSA_SIZE << 1)

static const uint32_t powers_of_ten[29][S21_DOUBLE_MANTISSA_SIZE] = {
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
#define UINT_MAX (1llu << 32llu)
#define S21_MAXIMUM_FLOAT 79228162514264337593543950335.0F
#define S21_SMALLEST_FLOAT 1e-28F

// TASK FUNCTIONS
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

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

#define IABS(a) ((a) > 0 ? (a) : -(a))

uint32_t s21_level_mantissae(s21_decimal *a, s21_decimal *b,
                             uint32_t a_double_mantissa[],
                             uint32_t b_double_mantissa[],
                             uint32_t intfield_size, int16_t *exp_diff);

uint32_t s21_add_intfield(const uint32_t operand1[], const uint32_t operand2[],
                          uint32_t result[], uint32_t intfield_size);

int s21_sub_intfield(const uint32_t operand1[], const uint32_t operand2[],
                     uint32_t result[], uint32_t intfield_size);

int s21_mul_intfield(const uint32_t operand1[], const uint32_t operand2[],
                     uint32_t result[], uint32_t intfield_size);

void s21_left_shift_intfield(const uint32_t operand[], uint32_t shift,
                             uint32_t result[], uint32_t intfield_size,
                             uint32_t *carry);

void s21_div_bitfield(const uint32_t dividend[], const uint32_t divisor[],
                      uint32_t result[], uint32_t remainder[],
                      uint32_t intfield_size);

uint8_t s21_is_decimal_divisible_by_2_5_10(uint32_t value[],
                                           uint32_t intfield_size);

int16_t s21_rmemcmp(const void *lhs, const void *rhs, uint32_t size);

uint32_t s21_read_bits(const uint32_t data[], const uint32_t bit_offset,
                       const uint32_t bit_count);

uint8_t *s21_dtoa(const uint32_t data[], uint32_t exponent,
                  uint32_t intfield_size, uint8_t *str);

#define s21_read_bit(data, bit_offset) s21_read_bits(data, bit_offset, 1)

// Exponent is the second most significant byte
#define s21_get_exponent(decimal) \
  ((decimal).bits[S21_DECIMAL_SIZE_IN_INTS - 1] >> 16 & 0xFF)

// Sign is the most significant bit
#define s21_is_decimal_negative(decimal) \
  ((decimal).bits[S21_DECIMAL_SIZE_IN_INTS - 1] >> 31 & 0x1)

void s21_write_bits(uint32_t data[], const uint32_t value,
                    const uint32_t absolute_offset, const uint32_t bit_count);

#define s21_set_exponent(decimal, value) \
  s21_write_bits((decimal).bits, (value), S21_DECIMAL_SIZE_IN_BITS - 16, 8)

#define s21_write_bit(data, absolute_offset, value) \
  s21_write_bits(data, value, absolute_offset, 1)

#define s21_write_sign(decimal, value) \
  s21_write_bits((decimal).bits, value, S21_DECIMAL_SIZE_IN_BITS - 1, 1)

uint32_t s21_get_top_bit_pos(const uint32_t data[], uint32_t intfield_size);

uint8_t s21_is_intfield_zero(const uint32_t intfield[], uint32_t intfield_size);

void s21_shrink(uint32_t data[], uint32_t intfield_size, int16_t *exponent);

uint8_t s21_is_decimal_valid(s21_decimal value);

s21_decimal s21_atod(const char *str);

#endif  // S21_DECIMAL_H
