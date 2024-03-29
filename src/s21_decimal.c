#include "s21_decimal.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  int32_t rval = OK;
  if (result && s21_is_decimal_valid(v1) && s21_is_decimal_valid(v2)) {
    memset(result, 0, sizeof(result->bits));
    uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
             v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
             res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0};
    int16_t v1_exp = s21_get_exponent(v1), v2_exp = s21_get_exponent(v2);
    s21_shrink(v1.bits, S21_MANTISSA_SIZE, &v1_exp);
    s21_shrink(v2.bits, S21_MANTISSA_SIZE, &v2_exp);
    s21_set_exponent(v1, v1_exp);
    s21_set_exponent(v2, v2_exp);
    int16_t exp_diff, v_cmp, max_exp = MAX(v1_exp, v2_exp);
    uint8_t diff_signs =
        (s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2));
    s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                        S21_MANTISSA_SIZE, &exp_diff);
    v_cmp = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                        S21_DOUBLE_MANTISSA_SIZE << 2);
    if (diff_signs)
      if (v_cmp > 0) {
        s21_sub_intfield(v1_double_mantissa, v2_double_mantissa,
                         res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
      } else {
        s21_sub_intfield(v2_double_mantissa, v1_double_mantissa,
                         res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
      }
    else {
      s21_add_intfield(v1_double_mantissa, v2_double_mantissa,
                       res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    }
    s21_write_sign(*result, v_cmp < 0 ? s21_is_decimal_negative(v2)
                                      : s21_is_decimal_negative(v1));
    int32_t excess_exp = 0;
    if (max_exp) {
      memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
      while (1) {
        s21_div_bitfield(res_double_mantissa, powers_of_ten[excess_exp],
                         v1_double_mantissa, S21_NULL,
                         S21_DOUBLE_MANTISSA_SIZE);
        if (s21_get_top_bit_pos(v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
            (S21_DOUBLE_MANTISSA_SIZE << 4) - 1) {
          excess_exp += 1;
          if (excess_exp > max_exp) {
            rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
            break;
          }
        } else {
          break;
        }
      }
      memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
      memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
      v2_double_mantissa[0] = 5;
      s21_mul_intfield(v2_double_mantissa,
                       powers_of_ten[(excess_exp ? excess_exp : 1) - 1],
                       v2_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    }
    if (rval == OK) {
      // printf("\nres_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      //                   PRINT_ALL);
      s21_div_bitfield(res_double_mantissa, powers_of_ten[excess_exp],
                       res_double_mantissa, v1_double_mantissa,
                       S21_DOUBLE_MANTISSA_SIZE);
      // printf("\nshrunken res_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE,
      // max_exp,
      //                   PRINT_ALL);
      // // printf("\nremainder: ");
      // // s21_print_hex_bin(v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      // //                   PRINT_ALL);
      // // printf("\ntestfield: ");
      // // s21_print_hex_bin(v2_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      // //                   PRINT_ALL);
      if (excess_exp) {
        int16_t v_cmp2 = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                                     sizeof(v1_double_mantissa));
        if ((v_cmp2 == 0 && s21_read_bit(res_double_mantissa, 0)) || v_cmp2 > 0)
          s21_add_intfield(res_double_mantissa, powers_of_ten[0],
                           res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
      }
      max_exp -= excess_exp;
      // // s21_div_bitfield(res_double_mantissa, powers_of_ten[1],
      // // res_double_mantissa,
      // //                  v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
      // // if ((v1_double_mantissa[0] == 5 && s21_read_bit(res_double_mantissa,
      // 0))
      // // ||
      // //     v1_double_mantissa[0] > 5)
      // //   s21_add_intfield(res_double_mantissa, powers_of_ten[0],
      // //                    res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
      // // max_exp += 1;
      // // s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &max_exp);
      // // printf("\nshrunken res_double_mantissa: ");
      // // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      // //                   PRINT_ALL);
      // printf("\nres_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      //                   PRINT_ALL);
      // s21_div_bitfield(res_double_mantissa, powers_of_ten[max_exp],
      //                  res_double_mantissa, NULL, S21_DOUBLE_MANTISSA_SIZE);
      // max_exp += 1;
      // s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &max_exp);
      // printf("\nshrunken res_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      //                   PRINT_ALL);
      s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &max_exp);
      memmove(result, res_double_mantissa,
              (S21_MANTISSA_SIZE) * sizeof(result->bits[0]));
      // s21_write_sign(*result, v_cmp < 0 ? s21_is_decimal_negative(v2)
      //                                   : s21_is_decimal_negative(v1));
      // printf("res_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE,
      // max_exp,
      //                   PRINT_ALL);
      if (s21_get_top_bit_pos(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
          (S21_DOUBLE_MANTISSA_SIZE << 4) - 1)
        rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
      s21_set_exponent(*result, max_exp);
    }
  } else {
    rval = CALC_ERROR;
  }
  return rval;
}
uint32_t s21_add_intfield(const uint32_t operand1[], const uint32_t operand2[],
                          uint32_t result[], uint32_t intfield_size) {
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

int s21_sub_intfield(const uint32_t operand1[], const uint32_t operand2[],
                     uint32_t result[], uint32_t intfield_size) {
  uint32_t carry = 0;
  for (uint32_t i = 0; i < intfield_size; ++i) {
    uint32_t temp = operand1[i] - operand2[i] - carry;
    carry = operand1[i] < operand2[i] || (operand1[i] == operand2[i] && carry);
    result[i] = temp;
  }
  return (int32_t)carry;
}

int s21_mul(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  int32_t rval = OK;
  if (result && s21_is_decimal_valid(v1) && s21_is_decimal_valid(v2)) {
    memset(result, 0, sizeof(result->bits));
    uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
             v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
             res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0};
    int16_t v1_exp = s21_get_exponent(v1), v2_exp = s21_get_exponent(v2);
    s21_shrink(v1.bits, S21_MANTISSA_SIZE, &v1_exp);
    s21_shrink(v2.bits, S21_MANTISSA_SIZE, &v2_exp);
    s21_set_exponent(v1, v1_exp);
    s21_set_exponent(v2, v2_exp);
    memcpy(v1_double_mantissa, v1.bits,
           (S21_MANTISSA_SIZE) * sizeof(v1_double_mantissa[0]));
    memcpy(v2_double_mantissa, v2.bits,
           (S21_MANTISSA_SIZE) * sizeof(v2_double_mantissa[0]));
    s21_mul_intfield(v1_double_mantissa, v2_double_mantissa,
                     res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    int16_t exponent = s21_get_exponent(v1) + s21_get_exponent(v2),
            excess_exp = 0;
    s21_write_sign(*result,
                   s21_is_decimal_negative(v1) ^ s21_is_decimal_negative(v2));
    if (exponent) {
      memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
      while (1) {
        s21_div_bitfield(res_double_mantissa, powers_of_ten[excess_exp],
                         v1_double_mantissa, S21_NULL,
                         S21_DOUBLE_MANTISSA_SIZE);
        if (s21_get_top_bit_pos(v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
            (S21_DOUBLE_MANTISSA_SIZE << 4) - 1) {
          excess_exp += 1;
          if (excess_exp > exponent) {
            rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
            break;
          }
        } else {
          break;
        }
      }
      memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
      memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
      v2_double_mantissa[0] = 5;
      s21_mul_intfield(v2_double_mantissa,
                       powers_of_ten[(excess_exp ? excess_exp : 1) - 1],
                       v2_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    }
    if (rval == OK) {
      // printf("\nres_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      //                   PRINT_ALL);
      s21_div_bitfield(res_double_mantissa, powers_of_ten[excess_exp],
                       res_double_mantissa, v1_double_mantissa,
                       S21_DOUBLE_MANTISSA_SIZE);
      if (excess_exp) {
        int16_t v_cmp2 = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                                     sizeof(v1_double_mantissa));
        if ((v_cmp2 == 0 && s21_read_bit(res_double_mantissa, 0)) || v_cmp2 > 0)
          s21_add_intfield(res_double_mantissa, powers_of_ten[0],
                           res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
      }
      exponent -= excess_exp;
      s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &exponent);
      // printf("\nshrunken res_double_mantissa: ");
      // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
      //                   PRINT_ALL);
      memcpy(result->bits, res_double_mantissa,
             (S21_MANTISSA_SIZE) * sizeof(res_double_mantissa[0]));
      s21_set_exponent(*result, exponent);
      if (s21_get_exponent(*result) > S21_MAX_DECIMAL_EXPONENT)
        rval = TOO_SMALL;
      if (s21_get_top_bit_pos(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
          (S21_DOUBLE_MANTISSA_SIZE << 4) - 1)
        rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
    }
  } else {
    rval = CALC_ERROR;
  }
  return rval;
}
int s21_mul_intfield(const uint32_t operand1[], const uint32_t operand2[],
                     uint32_t result[], uint32_t intfield_size) {
  uint32_t product[intfield_size << 1];
  memset(product, 0, sizeof(product));
  for (uint32_t i = 0; i < intfield_size; ++i) {
    uint32_t carry = 0;
    for (uint32_t j = 0; j < intfield_size; ++j) {
      uint64_t temp =
          (uint64_t)operand1[j] * operand2[i] + product[i + j] + carry;
      carry = temp >> 32;
      product[i + j] = temp;
    }
    product[i + intfield_size] = carry;
  }
  memcpy(result, product, intfield_size * sizeof(result[0]));
  return OK;
}
int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  int32_t rval = OK;
  if (!(result && s21_is_decimal_valid(v1) && s21_is_decimal_valid(v2))) {
    rval = CALC_ERROR;
  } else if (s21_is_intfield_zero(v2.bits, S21_MANTISSA_SIZE))
    rval = ZERO_DIVIDING;
  else {
    memset(result, 0, sizeof(result->bits));
    int16_t v1_exp = s21_get_exponent(v1), v2_exp = s21_get_exponent(v2),
            res_exp = 0;
    s21_shrink(v1.bits, S21_MANTISSA_SIZE, &v1_exp);
    s21_shrink(v2.bits, S21_MANTISSA_SIZE, &v2_exp);
    s21_set_exponent(v1, v1_exp);
    s21_set_exponent(v2, v2_exp);
    uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
             v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0},
             res_double_mantissa[S21_DOUBLE_MANTISSA_SIZE] = {0};
    memcpy(v1_double_mantissa, v1.bits,
           (S21_MANTISSA_SIZE) * sizeof(v1_double_mantissa[0]));
    memcpy(v2_double_mantissa, v2.bits,
           (S21_MANTISSA_SIZE) * sizeof(v2_double_mantissa[0]));
    // printf("v1_double_mantissa: ");
    // s21_print_hex_bin(v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
    //                   PRINT_ALL);
    // printf("\nv2_double_mantissa: ");
    // s21_print_hex_bin(v2_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
    //                   PRINT_ALL);
    s21_mul_intfield(v1_double_mantissa,
                     powers_of_ten[S21_MAX_DECIMAL_EXPONENT],
                     v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE);
    // printf("\nexpanded v1_double_mantissa: ");
    // s21_print_hex_bin(v1_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
    //                   PRINT_ALL);
    res_exp = (int16_t)(28 + (v1_exp - v2_exp));
    // printf("\nres_exp: %d", res_exp);
    s21_div_bitfield(v1_double_mantissa, v2_double_mantissa,
                     res_double_mantissa, S21_NULL, S21_DOUBLE_MANTISSA_SIZE);
    // printf("\nres_double_mantissa: ");
    // s21_print_hex_bin(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, 0,
    //                   PRINT_ALL);
    s21_shrink(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE, &res_exp);
    memcpy(result->bits, res_double_mantissa,
           (S21_MANTISSA_SIZE) * sizeof(res_double_mantissa[0]));
    s21_set_exponent(*result, res_exp);
    s21_write_sign(*result,
                   s21_is_decimal_negative(v1) ^ s21_is_decimal_negative(v2));
    if (s21_get_top_bit_pos(res_double_mantissa, S21_DOUBLE_MANTISSA_SIZE) >
        (S21_DOUBLE_MANTISSA_SIZE << 4) - 1)
      rval = s21_is_decimal_negative(*result) ? TOO_SMALL : TOO_BIG;
  }
  return rval;
}
// https://en.wikipedia.org/wiki/Division_algorithm#Integer_division_(unsigned)_with_remainder
void s21_div_bitfield(const uint32_t dividend[], const uint32_t divisor[],
                      uint32_t result[], uint32_t remainder[],
                      uint32_t intfield_size) {
  if (!(remainder || result)) return;
  uint32_t divd[intfield_size], quotient[intfield_size];
  memset(divd, 0, sizeof(divd));
  memset(quotient, 0, sizeof(quotient));
  uint32_t i = (sizeof(divd) << 3) - 1;
  do {
    s21_left_shift_intfield(divd, 1, divd, intfield_size, &(uint32_t){0});
    s21_write_bits(divd, s21_read_bit(dividend, i), 0, 1);
    if (s21_rmemcmp(divd, divisor, sizeof(divd)) >= 0) {
      s21_sub_intfield(divd, divisor, divd, intfield_size);
      s21_write_bits(quotient, 1, i, 1);
    }
  } while (i-- > 0);
  if (remainder) memcpy(remainder, divd, intfield_size * sizeof(divd[0]));
  if (result) memcpy(result, quotient, intfield_size * sizeof(quotient[0]));
}

int s21_is_less(s21_decimal v1, s21_decimal v2) {  // TODO
  return s21_is_greater(v2, v1);
}
int s21_is_less_or_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_less(v1, v2) || s21_is_equal(v1, v2);
}
int s21_is_greater(s21_decimal v1, s21_decimal v2) {
  uint8_t is_v1_zero = s21_is_intfield_zero(v1.bits, S21_MANTISSA_SIZE),
          is_v2_zero = s21_is_intfield_zero(v2.bits, S21_MANTISSA_SIZE),
          rval = FALSE, exit = FALSE;
  if (is_v1_zero && is_v2_zero) {
    rval = FALSE;
    exit = TRUE;
  }  // are both a zero?..
  if (!exit && is_v1_zero) {
    rval = s21_is_decimal_negative(v2);
    exit = TRUE;
  }  // ..then, is only v1 a zero?(and therefore greater than a
     // negative or less than positive non-zero v2)..
  if (!exit && is_v2_zero) {
    rval = !s21_is_decimal_negative(v1);
    exit = TRUE;
  }  // ..then, is only v2 a zero?(and therefore greater than a
     // negative or less than positive non-zero v1)..
  if (!exit && s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2)) {
    rval = s21_is_decimal_negative(v2);
    exit = TRUE;
  }  // ..then, do signs differ?(and a negative v2 is therefore less
  // than a positive v1)..
  if (!exit) {
    uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE],
        v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE];
    memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
    memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
    s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                        S21_MANTISSA_SIZE, &(int16_t){0});
    int16_t v_cmp = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                                sizeof(v1_double_mantissa));
    rval = s21_is_decimal_negative(v1) ? v_cmp < 0 : v_cmp > 0;
  }
  return rval;
}
int s21_is_greater_or_equal(s21_decimal v1, s21_decimal v2) {
  return s21_is_greater(v1, v2) || s21_is_equal(v1, v2);
}
int s21_is_equal(s21_decimal v1, s21_decimal v2) {
  uint8_t exit = FALSE, rval = TRUE;
  uint8_t is_v1_zero = s21_is_intfield_zero(v1.bits, S21_MANTISSA_SIZE);
  if (is_v1_zero != s21_is_intfield_zero(v2.bits, S21_MANTISSA_SIZE)) {
    rval = FALSE;
    exit = TRUE;
  }  // is only one a zero?..
  if (!exit && is_v1_zero) {
    rval = TRUE;
    exit = TRUE;
  }  // ..then, are both a zero?..
  if (!exit && s21_is_decimal_negative(v1) != s21_is_decimal_negative(v2)) {
    rval = FALSE;
    exit = TRUE;
  }  // ..then, do signs differ?..
  // ..then...
  if (!exit) {
    uint32_t v1_double_mantissa[S21_DOUBLE_MANTISSA_SIZE],
        v2_double_mantissa[S21_DOUBLE_MANTISSA_SIZE];
    memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
    memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
    s21_level_mantissae(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
                        S21_MANTISSA_SIZE, &(int16_t){0});
    rval = s21_rmemcmp(v1_double_mantissa, v2_double_mantissa,
                       sizeof(v1_double_mantissa)) == 0;
  }
  return rval;
}
int s21_is_not_equal(s21_decimal v1, s21_decimal v2) {
  return !s21_is_equal(v1, v2);
}
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int32_t rval = OK;
  if (!dst)
    rval = CALC_ERROR;
  else {
    memset(dst, 0, sizeof(dst->bits));
    s21_write_sign(*dst, src < 0);
    dst->bits[0] = (uint32_t)(src > 0 ? src : -src);
  }
  return rval;
}
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int32_t rval = OK;
  if (!dst)
    rval = CALC_ERROR;
  else {
    memset(dst, 0, sizeof(dst->bits));

    if (isinf(src) || isnan(src) || fabsf(src) > S21_MAXIMUM_FLOAT ||
        (fabsf(src) > 0 && fabsf(src) < S21_SMALLEST_FLOAT))
      rval = CONVERT_ERROR;
    else {
      uint32_t len = snprintf(NULL, 0, "%.6E", src);
      char str[len + 1];
      snprintf(str, len + 1, "%.6E", src);
      *dst = s21_atod(str);
      // if (s21_get_exponent(*dst) > 7) {
      //   s21_set_exponent(*dst, 7);
      // }
      //////////////////////////
      // int sign = ((src < 0) ? 1 : 0);
      // src = fabsf(src);
      // int tmp = (int)src, exp = 0;
      // while (src - ((float)tmp / (powf(10, (float)exp))) != 0) {
      //   ++exp;
      //   if (exp > S21_MAX_DECIMAL_EXPONENT) {
      //     rval = CONVERT_ERROR;
      //     break;
      //   }
      //   tmp = (int)(src * (powf(10, (float)exp)));
      // }
      // if (rval == OK) {
      //   s21_from_int_to_decimal(tmp, dst);
      //   s21_set_exponent(*dst, exp);
      //   s21_write_sign(*dst, sign);
      // }
    }
  }
  return rval;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int32_t rval = OK;
  if (!(dst && s21_is_decimal_valid(src)))
    rval = CALC_ERROR;
  else {
    s21_div_bitfield(src.bits, powers_of_ten[s21_get_exponent(src)], src.bits,
                     NULL, S21_MANTISSA_SIZE);
    if (src.bits[2] || src.bits[1] || src.bits[0] > INT32_MAX)
      rval = CONVERT_ERROR;
    else {
      union {
        uint32_t sval;
        int32_t uval;
      } u = {
          .sval = src.bits[0],
      };
      *dst = u.uval;
      (void)u.sval;
      if (s21_is_decimal_negative(src)) *dst = -*dst;
    }
  }
  return rval;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int32_t rval = OK;
  if (!(dst && s21_is_decimal_valid(src)))
    rval = CALC_ERROR;
  else {
    char str[200] = {0};
    s21_dtoa(src.bits, s21_get_exponent(src), S21_MANTISSA_SIZE,
             (uint8_t *)str);
    *dst = sscanf(str, "%f", dst) == 1 ? *dst : 0;
    if (s21_is_decimal_negative(src)) *dst = -*dst;
  }
  return rval;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int rval = OK;
  if (!(result && s21_is_decimal_valid(value)))
    rval = CALC_ERROR;
  else {
    memcpy(result, &value, sizeof(s21_decimal));
    uint32_t remainder[S21_MANTISSA_SIZE];
    while (s21_get_exponent(*result)) {
      s21_div_bitfield(result->bits, powers_of_ten[1], result->bits, remainder,
                       S21_MANTISSA_SIZE);
      if (remainder[0] && s21_is_decimal_negative(*result)) {
        s21_add_intfield(result->bits, powers_of_ten[0], result->bits,
                         S21_MANTISSA_SIZE);
      }
      s21_set_exponent(*result, s21_get_exponent(*result) - 1);
    }
  }
  return rval;
}
int s21_round(s21_decimal value, s21_decimal *result) {
  int32_t rval = OK;
  if (!(result && s21_is_decimal_valid(value)))
    rval = CALC_ERROR;
  else {
    memcpy(result, &value, sizeof(s21_decimal));
    uint32_t remainder[S21_MANTISSA_SIZE];
    while (s21_get_exponent(*result)) {
      s21_div_bitfield(result->bits, powers_of_ten[1], result->bits, remainder,
                       S21_MANTISSA_SIZE);
      if (remainder[0] > 4) {
        s21_add_intfield(result->bits, powers_of_ten[0], result->bits,
                         S21_MANTISSA_SIZE);
      }
      s21_set_exponent(*result, s21_get_exponent(*result) - 1);
    }
  }
  return rval;
}
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int32_t rval = OK;
  if (!(result && s21_is_decimal_valid(value)))
    rval = CALC_ERROR;
  else {
    memset(result, 0, sizeof(s21_decimal));
    s21_div_bitfield(value.bits, powers_of_ten[s21_get_exponent(value)],
                     result->bits, NULL, S21_MANTISSA_SIZE);
    s21_write_sign(*result, s21_is_decimal_negative(value));
    s21_write_bits(result->bits, 0, S21_DOUBLE_MANTISSA_SIZE << 4, 16);
    s21_write_bits(result->bits, 0, (S21_DOUBLE_MANTISSA_SIZE << 4) + 24, 7);
  }
  return rval;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int32_t rval = OK;
  if (!(result && s21_is_decimal_valid(value)))
    rval = CALC_ERROR;
  else {
    memmove(result, &value, sizeof(s21_decimal));
    s21_write_sign(*result, !s21_is_decimal_negative(*result));
  }
  return rval;
}

uint32_t s21_level_mantissae(s21_decimal *a, s21_decimal *b,
                             uint32_t a_double_mantissa[],
                             uint32_t b_double_mantissa[],
                             uint32_t intfield_size, int16_t *exp_diff) {
  uint32_t rval = OK;
  *exp_diff = (int16_t)(s21_get_exponent(*a) - s21_get_exponent(*b));
  if (IABS(*exp_diff) <= S21_MAX_DECIMAL_EXPONENT) {
    memmove(a_double_mantissa, a->bits,
            intfield_size * sizeof(a_double_mantissa[0]));
    memmove(b_double_mantissa, b->bits,
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
// https://web.archive.org/web/20171110144318/http://mathforum.org/library/drmath/view/55908.html
uint8_t s21_is_decimal_divisible_by_2_5_10(uint32_t data[],
                                           uint32_t intfield_size) {
  uint64_t sum = 0, rval = 0;
  for (uint32_t i = 0; i < intfield_size * sizeof(data[0]); ++i)
    sum += s21_read_bits(data, i << 3, 8);
  if (!s21_read_bit(data, 0)) rval |= 1;
  if (!(sum % 5)) rval |= rval ? 6 : 2;
  return rval;
}

int16_t s21_rmemcmp(const void *lhs, const void *rhs,
                    uint32_t size /*in bytes */) {
  int16_t answ = 0;
  --size;
  do {
    answ = (int16_t)(((uint8_t *)lhs)[size] - ((uint8_t *)rhs)[size]);
  } while (!answ && size--);
  return answ;
}

void s21_left_shift_intfield(const uint32_t operand[], uint32_t shift,
                             uint32_t result[], uint32_t intfield_size,
                             uint32_t *carry) {
  memmove(result, operand, intfield_size * sizeof(result[0]));
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

void s21_write_bits(uint32_t data[], const uint32_t value,
                    const uint32_t absolute_offset, const uint32_t bit_count) {
  uint32_t unit = absolute_offset / 32, offset_in_unit = absolute_offset % 32;
  if (offset_in_unit >> 5 == (offset_in_unit + bit_count - 1) >> 5) {
    uint32_t shifted_mask = (((1 << bit_count) - 1) << offset_in_unit);
    data[unit] &= bit_count == 32 ? 0 : ~shifted_mask;
    data[unit] |= ((bit_count == 32 ? ~(uint32_t)(0) : shifted_mask) &
                   (value << offset_in_unit));
  } else {
    data[unit] &= ((1 << offset_in_unit) - 1);
    data[unit] |= (~((1 << offset_in_unit) - 1) & (value << offset_in_unit));
    data[unit + 1] &= ~((1 << (bit_count - (32 - offset_in_unit))) - 1);
    data[unit + 1] |= (value >> (32 - offset_in_unit));
  }
}

void s21_reverse_str(uint8_t *str, uint32_t size) {
  for (uint32_t i = 0; i < size >> 1; ++i) {
    str[i] ^= str[size - i - 1];
    str[size - i - 1] ^= str[i];
    str[i] ^= str[size - i - 1];
  }
}

uint32_t s21_get_top_bit_pos(const uint32_t data[], uint32_t intfield_size) {
  uint32_t i = (intfield_size << 5) - 1;
  while (1) {
    if (s21_read_bits(data, i, 1) || !i) break;
    --i;
  }
  return i;
}

uint8_t *s21_dtoa(const uint32_t data[], uint32_t exponent,
                  uint32_t intfield_size, uint8_t *str) {
  uint32_t temp[intfield_size], idx = 0, remcollector[intfield_size],
                                neg = exponent >> 15;
  exponent &= 0xFF;
  memcpy(temp, data, intfield_size * sizeof(temp[0]));
  memset(remcollector, 0, intfield_size * sizeof(remcollector[0]));
  while (!s21_is_intfield_zero(temp, intfield_size) || exponent) {
    s21_div_bitfield(temp, powers_of_ten[1], temp, remcollector, intfield_size);
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

uint8_t s21_is_intfield_zero(const uint32_t intfield[],
                             uint32_t intfield_size) {
  uint8_t rval = TRUE;
  for (uint32_t i = 0; rval && i < intfield_size; i++) {
    if (intfield[i] != 0) {
      rval = FALSE;
    }
  }
  return rval;
}
void s21_shrink(uint32_t data[], uint32_t intfield_size, int16_t *exponent) {
  uint32_t rem[intfield_size];
  while ((*exponent &&
          (s21_get_top_bit_pos(data, intfield_size) >
               (S21_DOUBLE_MANTISSA_SIZE << 4) - 1 ||
           s21_is_decimal_divisible_by_2_5_10(data, intfield_size) >> 2)) ||
         *exponent > 28) {
    s21_div_bitfield(data, powers_of_ten[1], data, rem, intfield_size);
    --*exponent;
    if ((rem[0] == 5 && s21_read_bit(data, 0)) || rem[0] > 5)
      s21_add_intfield(data, powers_of_ten[0], data, intfield_size);
  }
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
s21_decimal s21_atod(const char *str) {
  s21_size_t len = strnlen(str, 200);
  int16_t exp = 0;
  char str_copy[len + 1];
  memcpy(str_copy, str, len + 1);
  s21_decimal accumulator = {0};
  uint32_t idx = 0, layer[S21_DOUBLE_MANTISSA_SIZE >> 1];
  memset(layer, 0, sizeof(layer));
  if (len && str_copy[0] == '-') {
    s21_write_sign(accumulator, TRUE);
    ++idx;
  }
  for (; idx < len; ++idx) {
    if (str_copy[idx] == '.') {
      uint32_t eidx = idx;
      for (; eidx < len; ++eidx) {
        if (str_copy[eidx] == 'e' || str_copy[eidx] == 'E') {
          str_copy[eidx] = 0;
          break;
        }
      }
      exp = (int16_t)(strnlen(str_copy, 200) - idx - 1);
      if (eidx < len) str_copy[eidx] = 'e';
      s21_set_exponent(accumulator, exp);
      continue;
    }
    if (str_copy[idx] == 'e' || str_copy[idx] == 'E') {
      int16_t eexp = (int16_t)atoi(str_copy + idx + 1);
      exp -= (int16_t)eexp;
      if (exp < 0) {
        s21_mul_intfield(accumulator.bits, powers_of_ten[IABS(exp)],
                         accumulator.bits, S21_MANTISSA_SIZE);
        exp = 0;
      }
      s21_set_exponent(accumulator, exp);
      break;
    } else {
      layer[0] = str_copy[idx] - '0';
      s21_mul_intfield(accumulator.bits, powers_of_ten[1], accumulator.bits,
                       S21_DOUBLE_MANTISSA_SIZE >> 1);
      s21_add_intfield(accumulator.bits, layer, accumulator.bits,
                       S21_DOUBLE_MANTISSA_SIZE >> 1);
    }
  }
  exp = s21_get_exponent(accumulator);
  s21_shrink(accumulator.bits, S21_DOUBLE_MANTISSA_SIZE >> 1, &exp);
  s21_set_exponent(accumulator, exp);
  return accumulator;
}

uint8_t s21_is_decimal_valid(s21_decimal value) {
  return !s21_read_bits(value.bits, S21_DOUBLE_MANTISSA_SIZE << 4, 16) &&
         !s21_read_bits(value.bits, (S21_DOUBLE_MANTISSA_SIZE << 4) + 24, 7) &&
         s21_get_exponent(value) <= S21_MAX_DECIMAL_EXPONENT;
}
