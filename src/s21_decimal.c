#include "s21_decimal.h"

#include <stdint.h>
#include <stdio.h>

// #include <stdio.h>

// TASK FUNCTIONS
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  s21_memset(result, 0, sizeof(result->bits));
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0}, rval = OK;
  int16_t v1_exp = s21_get_exponent(v1), v2_exp = s21_get_exponent(v2);
  s21_shrink(v1.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &v1_exp);
  s21_shrink(v2.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &v2_exp);
  s21_set_exponent(v1, v1_exp);
  s21_set_exponent(v2, v2_exp);
  int16_t exp_diff, v_cmp,
      max_exp = MAX(s21_get_exponent(v1), s21_get_exponent(v2));
  uint8_t diff_signs =
      (s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2));
  s21_memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
  s21_memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
  s21_memset(res_double_mantissa, 0, sizeof(res_double_mantissa));
  s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                      S21_DOUBLE_MANTISSA_SIZE >> 1, &exp_diff);
  v_cmp = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                      S21_DOUBLE_MANTISSA_SIZE << 2);
  if (diff_signs)
    if (v_cmp > 0) {
      rval = s21_sub_intfield(v1_double_mantissa, v2_double_mantissa,
                              res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    } else {
      rval = s21_sub_intfield(v2_double_mantissa, v1_double_mantissa,
                              res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    }
  else if (v_cmp > 0) {
    rval = s21_add_intfield(v1_double_mantissa, v2_double_mantissa,
                            res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  } else {
    rval = s21_add_intfield(v2_double_mantissa, v1_double_mantissa,
                            res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  }
  s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &max_exp);
  s21_memmove(result, res_double_mantissa,
              (S21_DOUBLE_MANTISSA_SIZE >> 1) * sizeof(result->bits[0]));
  s21_write_sign(*result, v_cmp < 0 ? s21_is_decimal_negative(v2)
                                    : s21_is_decimal_negative(v1));
  if (s21_get_top_bit_pos(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
      (S21_DOUBLE_MANTISSA_SIZE << 4) - 1)
    rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
  s21_set_exponent(*result, max_exp);
  return (int32_t)rval;
}
// WIP
S21_STATIC_KEYWORD uint32_t s21_add_intfield(const uint32_t operand1[],
                                             const uint32_t operand2[],
                                             uint32_t result[],
                                             uint32_t intfield_size) {
  uint32_t carry = 0;
  for (uint32_t i = 0; i < intfield_size; ++i) {
    uint64_t temp = operand1[i] + operand2[i] + carry;
    carry = temp < operand1[i] || temp < operand2[i] ||
            ((operand1[i] == ~0u || operand2[i] == ~0u) && carry);
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
  s21_memset(result, 0, sizeof(result->bits));
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0};
  int16_t v1_exp = s21_get_exponent(v1), v2_exp = s21_get_exponent(v2);
  s21_shrink(v1.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &v1_exp);
  s21_shrink(v2.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &v2_exp);
  s21_set_exponent(v1, v1_exp);
  s21_set_exponent(v2, v2_exp);
  s21_memcpy(v1_double_mantissa, v1.bits,
             (S21_DOUBLE_MANTISSA_SIZE >> 1) * sizeof(v1_double_mantissa[0]));
  s21_memcpy(v2_double_mantissa, v2.bits,
             (S21_DOUBLE_MANTISSA_SIZE >> 1) * sizeof(v2_double_mantissa[0]));
  uint32_t rval =
      s21_mul_intfield(v1_double_mantissa, v2_double_mantissa,
                       res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  int16_t exponent = s21_get_exponent(v1) + s21_get_exponent(v2);
  s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &exponent);
  s21_memcpy(result->bits, res_double_mantissa,
             (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(res_double_mantissa[0]));
  s21_write_sign(*result,
                 s21_is_decimal_negative(v1) ^ s21_is_decimal_negative(v2));
  s21_set_exponent(*result, exponent);
  if (s21_get_exponent(*result) > S21_MAX_DECIMAL_EXPONENT) rval = TOO_SMALL;
  if (s21_get_top_bit_pos(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
      (S21_DOUBLE_MANTISSA_SIZE << 4) - 1)
    rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
  return (int32_t)rval;
}
S21_STATIC_KEYWORD int s21_mul_intfield(const uint32_t operand1[],
                                        const uint32_t operand2[],
                                        uint32_t result[],
                                        uint32_t intfield_size) {
  uint32_t product[intfield_size << 1], carry = 0;
  s21_memset(product, 0, sizeof(product));
  for (uint32_t i = 0; i < intfield_size; ++i) {
    carry = 0;
    for (uint32_t j = 0; j < intfield_size; ++j) {
      uint64_t temp =
          (uint64_t)operand1[j] * operand2[i] + product[i + j] + carry;
      carry = temp >> 32;
      product[i + j] = temp;
    }
    product[i + intfield_size] = carry;
  }
  s21_memcpy(result, product, intfield_size * sizeof(result[0]));
  return OK;
}
int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  s21_memset(result, 0, sizeof(result->bits));
  if (s21_is_intfield_zero(v2.bits, S21_DECIMAL_SIZE_IN_INTS - 1))
    return ZERO_DIVIDING;
  int16_t v1_exp = s21_get_exponent(v1), v2_exp = s21_get_exponent(v2),
          res_exp = 0;
  s21_shrink(v1.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &v1_exp);
  s21_shrink(v2.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &v2_exp);
  s21_set_exponent(v1, v1_exp);
  s21_set_exponent(v2, v2_exp);
  // printf("v1_exp: %d, v2_exp: %d\n", v1_exp, v2_exp);
  uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
           res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0};
  s21_memcpy(v1_double_mantissa, v1.bits,
             (S21_DOUBLE_MANTISSA_SIZE >> 1) * sizeof(v1_double_mantissa[0]));
  s21_memcpy(v2_double_mantissa, v2.bits,
             (S21_DOUBLE_MANTISSA_SIZE >> 1) * sizeof(v2_double_mantissa[0]));
  // s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
  //                     S21_DOUBLE_MANTISSA_SIZE >> 1, &(int16_t){0});
  // if (s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
  //                 S21_DOUBLE_MANTISSA_SIZE) < 0) {
  // }
  s21_mul_intfield(v1_double_mantissa, powers_of_ten[S21_MAX_DECIMAL_EXPONENT],
                   v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  // s21_mul_intfield(v1_double_mantissa,
  // powers_of_ten[S21_MAX_DECIMAL_EXPONENT],
  //                  v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
  res_exp = (int16_t)(28 + (v1_exp - v2_exp));
  // printf("v1_double_mantissa: \n");
  // s21_print_hex_bin(v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE,
  //                   res_exp | (s21_is_decimal_negative(v1) << 15),
  //                   PRINT_ALL);
  // printf("\nv2_double_mantissa: \n");
  // s21_print_hex_bin(v2_double_mantissa, S21_DOUBLE_MANTISSA_SIZE,
  //                   res_exp | (s21_is_decimal_negative(v2) << 15),
  //                   PRINT_ALL);
  s21_div_intfield(v1_double_mantissa, v2_double_mantissa, res_double_mantissa,
                   S21_NULL, S21_DOUBLE_MANTISSA_SIZE);
  // printf("\nres_double_mantissa: \n");
  // s21_print_hex_bin(
  //     res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE,
  //     res_exp |
  //         ((s21_is_decimal_negative(v1) ^ s21_is_decimal_negative(v2)) <<
  //         15),
  //     PRINT_ALL);
  s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &res_exp);
  s21_memcpy(result->bits, res_double_mantissa,
             (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(res_double_mantissa[0]));
  s21_set_exponent(*result, res_exp);
  s21_write_sign(*result,
                 s21_is_decimal_negative(v1) ^ s21_is_decimal_negative(v2));
  return OK;
}
// https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
S21_STATIC_KEYWORD void s21_div_intfield(const uint32_t dividend[],
                                         const uint32_t divisor[],
                                         uint32_t result[],
                                         uint32_t remainder[],
                                         uint32_t intfield_size) {
  if (!(remainder || result))
    return;  // no need to calculate if neither is needed
  uint32_t divd[intfield_size], quotient[intfield_size];
  s21_memset(divd, 0, sizeof(divd));
  s21_memset(quotient, 0, sizeof(quotient));
  uint32_t i = (sizeof(divd) << 3) - 1;
  do {
    s21_left_shift_intfield(divd, 1, divd, intfield_size, &(uint32_t){0});
    s21_write_bits(divd, s21_read_bit(dividend, i), 0, 1);
    if (s21_rmemcmp(divd, divisor, sizeof(divd)) >= 0) {
      s21_sub_intfield(divd, divisor, divd, intfield_size);
      s21_write_bits(quotient, 1, i, 1);
    }
  } while (i-- > 0);
  if (remainder) s21_memcpy(remainder, divd, intfield_size * sizeof(divd[0]));
  if (result) s21_memcpy(result, quotient, intfield_size * sizeof(quotient[0]));
}

int s21_is_less(s21_decimal v1, s21_decimal v2) {  // TODO
  return s21_is_greater(v2, v1);
}
int s21_is_less_or_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_less(v1, v2) || s21_is_equal(v1, v2);
}
int s21_is_greater(s21_decimal v1, s21_decimal v2) {
  uint8_t is_v1_zero =
              s21_is_intfield_zero(v1.bits, S21_DECIMAL_SIZE_IN_INTS - 1),
          is_v2_zero =
              s21_is_intfield_zero(v2.bits, S21_DECIMAL_SIZE_IN_INTS - 1);
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
  return s21_rmemcmp(v1.bits, v2.bits, S21_DECIMAL_SIZE_IN_INTS - 1) > 0;
}
int s21_is_greater_or_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_greater(v1, v2) || s21_is_equal(v1, v2);
}
int s21_is_equal(s21_decimal v1, s21_decimal v2) {
  uint8_t is_v1_zero =
              s21_is_intfield_zero(v1.bits, S21_DECIMAL_SIZE_IN_INTS - 1),
          is_v2_zero =
              s21_is_intfield_zero(v2.bits, S21_DECIMAL_SIZE_IN_INTS - 1);
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
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_memset(dst, 0, sizeof(dst->bits));
  s21_write_sign(*dst, src < 0);
  s21_set_exponent(*dst, 0);
  dst->bits[0] = (uint32_t)(src > 0 ? src : -src);
  return OK;
}
// int s21_from_float_to_decimal(float src, s21_decimal *dst) {
//   s21_memset(dst, 0, sizeof(dst->bits));
//   union {
//     float f;
//     uint32_t i;
//   } u = {src};
//   // check if float is nan or inf

//   s21_size_t len = snprintf(S21_NULL, 0, "%f", 200);
//   char str[len];
//   snprintf(str, len, "%f", src);
//   *dst = s21_atod(str);
// }
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
  uint32_t carry = 0;
  for (uint32_t i = 0; i < intfield_size; ++i) {
    uint32_t temp = operand1[i] - operand2[i] - carry;
    carry = operand1[i] < operand2[i] || (operand1[i] == operand2[i] && carry);
    result[i] = temp;
  }
  return (int32_t)carry;
}

S21_STATIC_KEYWORD uint32_t s21_level_mantissae(s21_decimal *a, s21_decimal *b,
                                                uint32_t a_double_mantissa[],
                                                uint32_t b_double_mantissa[],
                                                uint32_t intfield_size,
                                                int16_t *exp_diff) {
  uint32_t rval = OK;
  *exp_diff = (int16_t)(s21_get_exponent(*a) - s21_get_exponent(*b));
  if (IABS(*exp_diff) <= S21_MAX_DECIMAL_EXPONENT) {
    s21_memmove(a_double_mantissa, a->bits,
                intfield_size * sizeof(a_double_mantissa[0]));
    s21_memmove(b_double_mantissa, b->bits,
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
s21_is_decimal_divisible_by_2_5_10(uint32_t data[], uint32_t intfield_size) {
  uint64_t sum = 0, rval = 0;
  for (uint32_t i = 0; i < intfield_size * sizeof(data[0]); ++i)
    sum += s21_read_bits(data, i << 3, 8);
  if (!s21_read_bit(data, 0)) rval |= 1;
  if (!(sum % 5)) rval |= rval ? 6 : 2;
  return rval;
}

S21_STATIC_KEYWORD s21_size_t s21_strnlen(const char *s, s21_size_t maxlen) {
  s21_size_t idx = 0;
  for (; idx < maxlen && s[idx]; ++idx)
    ;
  return idx;
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

S21_STATIC_KEYWORD void s21_reverse_str(uint8_t *str, uint32_t size) {
  // uses no temp variable
  for (uint32_t i = 0; i < size >> 1; ++i) {
    str[i] ^= str[size - i - 1];
    str[size - i - 1] ^= str[i];
    str[i] ^= str[size - i - 1];
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

S21_STATIC_KEYWORD uint8_t *s21_dtoa(const uint32_t data[], uint32_t exponent,
                                     uint32_t intfield_size, uint8_t *str) {
  uint32_t temp[intfield_size], idx = 0, remcollector[intfield_size],
                                neg = exponent >> 15;
  exponent &= 0xFF;
  s21_memcpy(temp, data, intfield_size * sizeof(temp[0]));
  s21_memset(remcollector, 0, intfield_size * sizeof(remcollector[0]));
  while (!s21_is_intfield_zero(temp, intfield_size) || exponent) {
    s21_div_intfield(temp, powers_of_ten[1], temp, remcollector, intfield_size);
    str[idx++] = '0' + remcollector[0];
    if (exponent) {
      if (!!(exponent - 1) != !!(exponent)) str[idx++] = '.';
      --exponent;
    }
  }
  if (str[idx - 1] == '.') str[idx++] = '0';
  if (!idx)
    str[idx++] = '0';
  else if (neg)
    str[idx++] = '-';
  str[idx] = 0;
  s21_reverse_str(str, idx);
  return str;
}
S21_STATIC_KEYWORD s21_decimal s21_atod(const char *str) {
  s21_size_t len = s21_strnlen(str, 200);
  s21_decimal accumulator = {0};
  uint32_t idx = 0, layer[S21_DOUBLE_MANTISSA_SIZE >> 1];
  s21_memset(layer, 0, sizeof(layer));
  if (len && str[0] == '-') {
    s21_write_sign(accumulator, TRUE);
    ++idx;
  }
  for (; idx < len; ++idx) {
    if (str[idx] == '.') {
      s21_set_exponent(accumulator, len - idx - 1);
      continue;
    }
    layer[0] = str[idx] - '0';
    s21_mul_intfield(accumulator.bits, powers_of_ten[1], accumulator.bits,
                     S21_DOUBLE_MANTISSA_SIZE >> 1);
    s21_add_intfield(accumulator.bits, layer, accumulator.bits,
                     S21_DOUBLE_MANTISSA_SIZE >> 1);
  }
  return accumulator;
}

S21_STATIC_KEYWORD uint8_t s21_is_intfield_zero(const uint32_t intfield[],
                                                uint32_t intfield_size) {
  for (uint32_t i = 0; i < intfield_size; i++) {
    if (intfield[i] != 0) {
      return FALSE;
    }
  }
  return TRUE;
}

int len_of_int(uint32_t value) {
  if (value >= 1000000000) return 10;
  if (value >= 100000000) return 9;
  if (value >= 10000000) return 8;
  if (value >= 1000000) return 7;
  if (value >= 100000) return 6;
  if (value >= 10000) return 5;
  if (value >= 1000) return 4;
  if (value >= 100) return 3;
  if (value >= 10) return 2;
  return 1;
}

void s21_print_bits(const uint32_t data[], const uint32_t from,
                    const uint32_t amount) {
  uint32_t i = from + amount - 1;
  while (i < from + amount) {
    uint32_t bit = s21_read_bit(data, i);
    printf("%s%u\x1B[0m", bit ? "\x1B[32m" : "\x1B[31m", bit);
    if (i == from) break;
    --i;
  }
}

void s21_read_bits_and_print(const uint32_t value[], const uint32_t from,
                             const uint32_t amount) {
  uint32_t width = len_of_int(S21_DECIMAL_SIZE_IN_BITS);
  printf("Bits %*u to %*u: ", width, from, width, from + amount - 1);
  s21_print_bits(value, from, amount);
}

void s21_print_hex_bin(const uint32_t value[], uint32_t intfield_size,
                       uint32_t exponent, uint8_t flags) {
  if (flags & PRINT_DEC) {
    uint8_t decstr[200] = {0};
    printf("%s\n",
           s21_dtoa(value, exponent,
                    intfield_size - (flags & ACCOUNT_FOR_SERVICE), decstr));
  }
  for (uint32_t i = 0;
       i < intfield_size && ((flags & PRINT_BIN) || (flags & PRINT_HEX)); ++i) {
    if (flags & PRINT_BIN) s21_read_bits_and_print(value, i * 32, 32);
    if (flags & PRINT_HEX)
      printf("%s%08X %-10u ", (flags & PRINT_BIN) ? " " : "", value[i],
             value[i]);
    printf("%s", i < intfield_size - 1 && (flags & PRINT_ALL) ? "\n" : "");
  }
  // Print sign and exponent
  if (flags & PRINT_BIN_HEX && flags & ACCOUNT_FOR_SERVICE)
    printf(" * %c1e-%u\n", (value[intfield_size - 1] >> 31) & 1 ? '-' : '+',
           exponent & 0xFF);
}
void s21_shrink(uint32_t data[], uint32_t intfield_size, int16_t *exponent) {
  uint32_t rem[intfield_size];
  while ((*exponent &&
          (s21_get_top_bit_pos(data, intfield_size) >
               (S21_DOUBLE_MANTISSA_SIZE << 4) - 1 ||
           s21_is_decimal_divisible_by_2_5_10(data, intfield_size) >> 2)) ||
         *exponent > 28) {
    s21_div_intfield(data, powers_of_ten[1], data, rem, intfield_size);
    --*exponent;
    if ((rem[0] == 5 && s21_read_bit(data, 0)) || rem[0] > 5)
      s21_add_intfield(data, powers_of_ten[0], data, intfield_size);
  }
}