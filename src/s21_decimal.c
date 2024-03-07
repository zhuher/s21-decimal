#include "s21_decimal.h"

#include <stdint.h>
#include <stdio.h>

// #include <stdio.h>

// TASK FUNCTIONS
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0}, rval = OK;
  int16_t exp_diff, v_cmp;
  uint8_t diff_signs =
      (s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2));

  s21_memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
  s21_memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
  s21_memset(res_double_mantissa, 0, sizeof(res_double_mantissa));

  s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                      S21_DOUBLE_MANTISSA_SIZE >> 1, &exp_diff);
  v_cmp = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                      sizeof(v1_double_mantissa));
  if (diff_signs)
    if (v_cmp > 0)
      rval = s21_sub_intfield(v2_double_mantissa, v1_double_mantissa,
                              res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    else
      rval = s21_sub_intfield(v1_double_mantissa, v2_double_mantissa,
                              res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  else if (v_cmp > 0)
    rval = s21_add_intfield(v1_double_mantissa, v2_double_mantissa,
                            res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  else
    rval = s21_add_intfield(v2_double_mantissa, v1_double_mantissa,
                            res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  s21_memmove(result, res_double_mantissa,
              (S21_DOUBLE_MANTISSA_SIZE >> 1) * sizeof(result->uint_data[0]));
  if (s21_get_top_bit_pos(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
      (S21_DOUBLE_MANTISSA_SIZE << 4) - 1)
    rval = TOO_BIG;
  s21_set_exponent(*result,
                   exp_diff > 0 ? s21_get_exponent(v1) : s21_get_exponent(v2));
  s21_write_sign(*result, v_cmp > 0 ? s21_is_decimal_negative(v1)
                                    : s21_is_decimal_negative(v2));
  return (int32_t)rval;
}
// WIP
S21_STATIC_KEYWORD uint32_t s21_add_intfield(const uint32_t operand1[],
                                             const uint32_t operand2[],
                                             uint32_t result[],
                                             uint32_t intfield_size) {
  uint32_t carry = 0;
  for (uint32_t i = 0; i < intfield_size; ++i) {
    uint32_t temp = operand1[i] + operand2[i] + carry;
    carry = temp < operand1[i] || temp < operand2[i];
    result[i] = temp;
  }
  return carry;
}
int s21_sub(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  s21_decimal v2_neg;
  s21_negate(v2, &v2_neg);
  return s21_add(v1, v2_neg, result);
}
int s21_mul(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0};
  s21_memmove(v1_double_mantissa, v1.uint_data,
              (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(v1_double_mantissa[0]));
  s21_memmove(v2_double_mantissa, v2.uint_data,
              (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(v2_double_mantissa[0]));
  uint32_t rval =
      s21_mul_intfield(v1_double_mantissa, v2_double_mantissa,
                       res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  s21_memmove(result->uint_data, res_double_mantissa,
              (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(res_double_mantissa[0]));
  s21_write_sign(*result,
                 s21_is_decimal_negative(v1) ^ s21_is_decimal_negative(v2));
  s21_set_exponent(*result, s21_get_exponent(v1) + s21_get_exponent(v2));
  if (s21_get_exponent(*result) > S21_MAX_DECIMAL_EXPONENT) rval = TOO_SMALL;
  return (int32_t)rval;
}
S21_STATIC_KEYWORD int s21_mul_intfield(const uint32_t operand1[],
                                        const uint32_t operand2[],
                                        uint32_t result[],
                                        uint32_t intfield_size) {
  uint32_t multiplication_layer[intfield_size], accumulator[intfield_size];
  s21_memset(multiplication_layer, 0, sizeof(multiplication_layer));
  s21_memset(accumulator, 0, sizeof(accumulator));
  uint32_t rval = OK;
  uint32_t i = (intfield_size * (sizeof(operand1[0]) << 3)) - 1;
  while (rval == OK) {
    if (s21_read_bit(operand2, i)) {
      s21_left_shift_intfield(operand1, i, multiplication_layer, intfield_size,
                              &(uint32_t){0});
      rval = s21_add_intfield(accumulator, multiplication_layer, accumulator,
                              intfield_size);
    }
    if (i == 0) break;
    --i;
  }
  s21_memmove(result, accumulator, intfield_size * sizeof(accumulator[0]));
  return (int32_t)rval;
}
int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  if (s21_is_decimal_zero(v2)) return ZERO_DIVIDING;
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           remainder[S21_DOUBLE_MANTISSA_SIZE] = {0};
  s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                      S21_DOUBLE_MANTISSA_SIZE >> 1, &(int16_t){0});
  s21_div_intfield(v1_double_mantissa, v2_double_mantissa, res_double_mantissa,
                   remainder, S21_DOUBLE_MANTISSA_SIZE);
  s21_memcpy(result->uint_data, res_double_mantissa,
             (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(res_double_mantissa[0]));
  (void)result;  // clear unused variable warning
  return OK;
}
// https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
S21_STATIC_KEYWORD void s21_div_intfield(uint32_t dividend[],
                                         uint32_t divisor[], uint32_t result[],
                                         uint32_t remainder[],
                                         uint32_t intfield_size) {
  uint32_t quotient[intfield_size];
  s21_memset(quotient, 0, sizeof(quotient));
  for (uint32_t i = (intfield_size << 5) - 1; TRUE; --i) {
    s21_left_shift_intfield(remainder, 1, remainder, intfield_size,
                            &(uint32_t){0});
    s21_write_bits(remainder, s21_read_bit(dividend, i), 0, 1);
    if (s21_rmemcmp(remainder, divisor, intfield_size) >= 0) {
      s21_sub_intfield(remainder, divisor, remainder, intfield_size);
      s21_write_bits(quotient, 1, i, 1);
    }
    if (i == 0) break;
  }
  s21_memcpy(result, quotient, intfield_size * sizeof(quotient[0]));
}

int s21_is_less(s21_decimal v1, s21_decimal v2) {  // TODO
  return s21_is_greater(v2, v1);
}
int s21_is_less_or_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_less(v1, v2) || s21_is_equal(v1, v2);
}
int s21_is_greater(s21_decimal v1, s21_decimal v2) {
  uint8_t is_v1_zero = s21_is_decimal_zero(v1),
          is_v2_zero = s21_is_decimal_zero(v2);
  if (is_v1_zero && is_v2_zero) return FALSE;  // are both a zero?..
  if (is_v1_zero)
    return s21_is_decimal_negative(
        v2);  // ..then, is only v1 a zero?(and therefore greater than a
              // negative or less than positive non-zero v2)..
  if (is_v2_zero)
    return !s21_is_decimal_negative(
        v1);  // ..then, is only v2 a zero?(and therefore greater than a
              // negative or less than positive non-zero v1)..
  if (s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2))
    return s21_is_decimal_negative(
        v2);  // ..then, do signs differ?(and a negative v2 is therefore less
  // than a positive v1)..

  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE],
      v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE];
  s21_memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
  s21_memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
  s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                      S21_DECIMAL_SIZE_IN_INTS - 1, &(int16_t){0});
  return s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                     sizeof(v1_double_mantissa)) > 0;
}
int s21_is_greater_or_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_greater(v1, v2) || s21_is_equal(v1, v2);
}
int s21_is_equal(s21_decimal v1, s21_decimal v2) {
  uint8_t is_v1_zero = s21_is_decimal_zero(v1),
          is_v2_zero = s21_is_decimal_zero(v2);
  if (is_v1_zero != is_v2_zero) return FALSE;  // is only one a zero?..
  if (is_v1_zero && is_v2_zero) return TRUE;   // ..then, are both a zero?..
  if (s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2))
    return FALSE;  // ..then, do signs differ?..
  // ..then...
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE],
      v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE];
  s21_memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
  s21_memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
  s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                      S21_DECIMAL_SIZE_IN_INTS - 1, &(int16_t){0});
  return s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                     sizeof(v1_double_mantissa)) == 0;
}
int s21_is_not_equal(s21_decimal v1, s21_decimal v2) {
  return !s21_is_equal(v1, v2);
}
//
// int s21_from_int_to_decimal(int src, s21_decimal *dst) {}
// int s21_from_float_to_decimal(float src, s21_decimal *dst) {}
// int s21_from_decimal_to_int(s21_decimal src, int *dst) {}
// int s21_from_decimal_to_float(s21_decimal src, float *dst) {}
//
// int s21_floor(s21_decimal value, s21_decimal *result) {}
// int s21_round(s21_decimal value, s21_decimal *result) {}
// int s21_truncate(s21_decimal value, s21_decimal *result) {}
int s21_negate(s21_decimal value, s21_decimal *result) {
  s21_memmove(result, &value, sizeof(s21_decimal));
  s21_write_sign(*result, !s21_is_decimal_negative(*result));
  return OK;
}

// OTHER FUNCTIONS

S21_STATIC_KEYWORD int s21_sub_intfield(const uint32_t operand1[],
                                        const uint32_t operand2[],
                                        uint32_t result[],
                                        uint32_t intfield_size) {
  uint32_t borrow = 0, temp = 0;
  for (uint32_t i = 0; i < intfield_size; ++i) {
    temp = operand1[i] - operand2[i] - borrow;
    borrow = temp > operand1[i] || temp > operand2[i];
    result[i] = temp;
  }
  return OK;
}

S21_STATIC_KEYWORD uint32_t s21_level_mantissae(s21_decimal *a, s21_decimal *b,
                                                uint32_t a_double_mantissa[],
                                                uint32_t b_double_mantissa[],
                                                uint32_t intfield_size,
                                                int16_t *exp_diff) {
  uint32_t rval = OK;

  *exp_diff = (int16_t)(s21_get_exponent(*a) - s21_get_exponent(*b));
  if (IABS(*exp_diff) <= S21_MAX_DECIMAL_EXPONENT) {
    s21_memmove(a_double_mantissa, a->uint_data,
                intfield_size * sizeof(a_double_mantissa[0]));
    s21_memmove(b_double_mantissa, b->uint_data,
                intfield_size * sizeof(b_double_mantissa[0]));
    if (*exp_diff < 0) {
      rval = s21_mul_intfield(a_double_mantissa, powers_of_ten[IABS(*exp_diff)],
                              a_double_mantissa, intfield_size << 1);
    } else if (*exp_diff > 0) {
      rval = s21_mul_intfield(b_double_mantissa, powers_of_ten[IABS(*exp_diff)],
                              b_double_mantissa, intfield_size << 1);
    }
  }
  return rval;
}
S21_STATIC_KEYWORD uint8_t
s21_is_decimal_divisible_by_10(uint32_t data[], uint32_t intfield_size) {
  uint64_t sum = 0;
  for (uint32_t i = 0; i < intfield_size << 2; ++i) {
    sum += s21_read_bits(data, i << 3, 8);
  }
  printf("value->uint_data = {");
  for (uint32_t i = 0; i < intfield_size; ++i) {
    printf("%10u, ", data[i]);
  }
  printf("} is ");
  if (!(sum % 5 || s21_read_bit(data, 0))) {
    printf("divisible by 10!\n");
    return TRUE;
  }
  printf("not divisible by 10!\n");
  return FALSE;
}
S21_STATIC_KEYWORD void *s21_memset(void *data, uint8_t value,
                                    uint32_t size /*in bytes */) {
  for (uint32_t i = 0; i < size; ++i) {
    ((uint8_t *)data)[i] = value;
  }
  return data;
}

S21_STATIC_KEYWORD void *s21_memcpy(void *dest, const void *src,
                                    uint32_t size /*in bytes */) {
  for (uint32_t i = 0; i < size; ++i) {
    ((uint8_t *)dest)[i] = ((uint8_t *)src)[i];
  }
  return dest;
}

S21_STATIC_KEYWORD void *s21_memmove(void *dest, const void *src,
                                     uint32_t size /*in bytes */) {
  uint8_t *d = (uint8_t *)dest;
  uint8_t *s = (uint8_t *)src;
  if (s == d || !size) {
    return dest;
  }
  if (s < d && s + size > d) {
    s += size;
    d += size;
    while (size--) {
      *--d = *--s;
    }
  }
  if (s > d && d + size > s) {
    while (size--) {
      *d++ = *s++;
    }
  }
  s21_memcpy(dest, src, size);
  return dest;
}

S21_STATIC_KEYWORD int16_t s21_rmemcmp(const void *lhs, const void *rhs,
                                       uint32_t size /*in bytes */) {
  int16_t answ = 0;
  --size;
  do {
    answ = (int16_t)(((uint8_t *)lhs)[size] - ((uint8_t *)rhs)[size]);
  } while (!answ && size--);
  return answ;
}

S21_STATIC_KEYWORD void s21_left_shift_intfield(const uint32_t operand[],
                                                uint32_t shift,
                                                uint32_t result[],
                                                uint32_t intfield_size,
                                                uint32_t *carry) {
  s21_memmove(result, operand, intfield_size * sizeof(result[0]));
  uint32_t full_shifts = shift >> 5;
  shift &= 31;
  if (shift) {
    *carry = 0;
    for (uint32_t i = 0; i < intfield_size; ++i) {
      uint32_t temp = result[i] << shift | *carry;
      *carry = result[i] >> (32 - shift);
      result[i] = temp;
    }
  }
  if (full_shifts) {
    for (uint32_t i = intfield_size - 1; i >= full_shifts; --i) {
      result[i] = result[i - full_shifts];
    }
    for (uint32_t i = 0; i < full_shifts; ++i) {
      result[i] = 0;
    }
  }
}

S21_STATIC_KEYWORD void s21_right_shift_intfield(const uint32_t operand[],
                                                 uint32_t shift,
                                                 uint32_t result[],
                                                 uint32_t intfield_size,
                                                 uint32_t *carry) {
  s21_memmove(result, operand, intfield_size * sizeof(result[0]));
  uint32_t full_shifts = shift >> 5;
  shift &= 31;
  if (shift) {
    *carry = 0;
    for (uint32_t i = intfield_size - 1; i < intfield_size; --i) {
      uint32_t temp = result[i] >> shift | *carry;
      *carry = result[i] << (32 - shift);
      result[i] = temp;
    }
  }
  if (full_shifts) {
    for (uint32_t i = 0; i < intfield_size - full_shifts; ++i) {
      result[i] = result[i + full_shifts];
    }
    for (uint32_t i = intfield_size - full_shifts; i < intfield_size; ++i) {
      result[i] = 0;
    }
  }
}

S21_STATIC_KEYWORD void s21_right_rotate_intfield(const uint32_t operand[],
                                                  uint32_t shift,
                                                  uint32_t result[],
                                                  uint32_t intfield_size) {
  uint32_t carry = 0;
  s21_right_shift_intfield(operand, shift, result, intfield_size, &carry);
  result[intfield_size - 1] |= carry;
}

S21_STATIC_KEYWORD void s21_left_rotate_intfield(const uint32_t operand[],
                                                 uint32_t shift,
                                                 uint32_t result[],
                                                 uint32_t intfield_size) {
  uint32_t carry = 0;
  s21_left_shift_intfield(operand, shift, result, intfield_size, &carry);
  result[0] |= carry;
}

S21_STATIC_KEYWORD void s21_invert_intfield(const uint32_t data[],
                                            uint32_t result[],
                                            uint32_t intfield_size) {
  for (uint32_t i = 0; i < intfield_size; ++i) result[i] = ~data[i];
}

S21_STATIC_KEYWORD void s21_2s_complement_intfield(const uint32_t value[],
                                                   uint32_t result[],
                                                   uint32_t intfield_size) {
  s21_invert_intfield(value, result, intfield_size);
  s21_add_intfield(result, powers_of_ten[0], result, intfield_size);
}

S21_STATIC_KEYWORD
uint32_t s21_read_bits(const uint32_t value[], const uint32_t bit_offset,
                       const uint32_t bit_count) {
  uint32_t rval, unit = bit_offset / 32, offset = bit_offset % 32;
  if (offset >> 5 == (offset + bit_count - 1) >> 5)
    rval = (value[unit] >> offset) & ((1 << bit_count) - 1);
  else
    rval = (value[unit] >> offset) |
           ((value[unit + 1] & ((1 << (bit_count - (32 - offset))) - 1))
            << (32 - offset));
  return rval;
}

S21_STATIC_KEYWORD void s21_write_bits(uint32_t data[], const uint32_t value,
                                       const uint32_t absolute_offset,
                                       const uint32_t bit_count) {
  uint32_t unit = absolute_offset / 32, offset_in_unit = absolute_offset % 32,
           shifted_mask;
  if (offset_in_unit >> 5 == (offset_in_unit + bit_count - 1) >> 5) {
    shifted_mask = (((1 << bit_count) - 1) << offset_in_unit);
    data[unit] &= bit_count == 32 ? 0 : ~shifted_mask;
    data[unit] |= ((bit_count == 32 ? ~(uint32_t)(0) : shifted_mask) &
                   (value << offset_in_unit));
  } else {  // could perhaps be done as a single double-sized unit by
            // casting
    data[unit] &= ((1 << offset_in_unit) - 1);
    data[unit] |= (~((1 << offset_in_unit) - 1) & (value << offset_in_unit));
    data[unit + 1] &= ~((1 << (bit_count - (32 - offset_in_unit))) - 1);
    data[unit + 1] |= (value >> (32 - offset_in_unit));
  }
}

S21_STATIC_KEYWORD uint32_t s21_get_top_bit_pos(const uint32_t data[],
                                                uint32_t intfield_size) {
  uint32_t i = (intfield_size << 5) - 1;
  while (1) {
    if (s21_read_bits(data, i, 1) || !i) break;
    --i;
  }
  return i;
}