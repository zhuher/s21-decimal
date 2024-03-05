#include "s21_decimal.h"

// #include <stdio.h>

// TASK FUNCTIONS
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  uint32_t double_mantissa_size = (S21_DECIMAL_SIZE_IN_INTS - 1) << 1,
           v1_double_mantissa[double_mantissa_size],
           v2_double_mantissa[double_mantissa_size],
           res_double_mantissa[double_mantissa_size], rval = OK, old_exp = 0;
  uint8_t diff_signs = (__s21_decimal_sign(v1) != __s21_decimal_sign(v2));
  int16_t v_cmp =
      s21_rmemcmp(v1.uint_data, v2.uint_data, double_mantissa_size << 1);

  s21_memset(v1_double_mantissa, 0, sizeof(v1_double_mantissa));
  s21_memset(v2_double_mantissa, 0, sizeof(v2_double_mantissa));
  s21_memset(res_double_mantissa, 0, sizeof(res_double_mantissa));
  __s21_level(&v1, &v2, v1_double_mantissa, v2_double_mantissa,
              double_mantissa_size >> 1, &old_exp);
  rval = diff_signs
             ? __s21_sub_intfield(v1_double_mantissa, v2_double_mantissa,
                                  res_double_mantissa, double_mantissa_size)
             : __s21_add_intfield(v1_double_mantissa, v2_double_mantissa,
                                  res_double_mantissa, double_mantissa_size);
  // TODO: deexponentiate le result
  // printf("v1 is %s\x1b[0m v2!\n",
  // (v_cmp > 0 && !__s21_decimal_sign(v1) && !diff_signs) ||
  // (!v_cmp && !__s21_decimal_sign(v1) && diff_signs) ||
  // (v_cmp < 0 && __s21_decimal_sign(v1) && !diff_signs)
  // ? "\x1b[32mgreater than"
  // : (!v_cmp && __s21_decimal_sign(v1) == __s21_decimal_sign(v2))
  // ? "\x1b[34mequal to"
  // : "\x1b[31mless than");

  s21_memcpy(result, res_double_mantissa,
             (double_mantissa_size >> 1) * sizeof(res_double_mantissa[0]));
  if (__s21_get_top_bit_pos(res_double_mantissa, double_mantissa_size) >
      (double_mantissa_size << 4) - 1)
    rval = TOO_BIG;
  return rval;
  // return __s21_add_intfield(v1.uint_data, v2.uint_data, result->uint_data,
  // S21_DECIMAL_SIZE_IN_INTS - 1);
}
// WIP
S21_STATIC_KEYWORD int __s21_add_intfield(const uint32_t operand1[],
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
// TODO
// int s21_sub(s21_decimal v1, s21_decimal v2, s21_decimal *result) {}
int s21_mul(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  s21_memset(result->uint_data, 0, sizeof(s21_decimal));
  uint32_t rval =
      __s21_mul_intfield(v1.uint_data, v2.uint_data, result->uint_data,
                         S21_DECIMAL_SIZE_IN_INTS - 1);
  ;
  result->uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] =
      (v1.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] & 0x80000000) ^
      (v2.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] & 0x80000000);
  __s21_write_exponent(result->uint_data,
                       __s21_read_exponent(v1) + __s21_read_exponent(v2));
  if (__s21_read_exponent(*result) > S21_MAX_DECIMAL_EXPONENT) rval = TOO_SMALL;
  return rval;
}
S21_STATIC_KEYWORD int __s21_mul_intfield(const uint32_t operand1[],
                                          const uint32_t operand2[],
                                          uint32_t result[],
                                          uint32_t intfield_size) {
  uint32_t multiplication_layer[intfield_size];
  s21_memset(multiplication_layer, 0, intfield_size * sizeof(operand1[0]));
  uint32_t rval = OK;
  uint32_t i = (intfield_size * (sizeof(operand1[0]) << 3)) - 1;
  while (rval == OK) {
    if (__s21_read_bit(operand2, i)) {
      __s21_left_shift_intfield(operand1, i, multiplication_layer,
                                intfield_size);
      rval = __s21_add_intfield(result, multiplication_layer, result,
                                intfield_size);
    }
    if (i == 0) break;
    --i;
  }
  return rval;
}
// int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal *result) {}
//
// int s21_is_less(s21_decimal v1, s21_decimal v2) {  // TODO
//   uint8_t v1_is_zero =
//               __s21_read_bits(v1.uint_data, 0, 1) ==
//               __s21_get_top_bit_pos(v1.uint_data, S21_DECIMAL_SIZE_IN_INTS -
//               1),
//           v2_is_zero =
//               __s21_read_bits(v2.uint_data, 0, 1) ==
//               __s21_get_top_bit_pos(v2.uint_data, S21_DECIMAL_SIZE_IN_INTS -
//               1);
// }
// int s21_is_less_or_equal(s21_decimal v1, s21_decimal v2) {}
// int s21_is_greater(s21_decimal v1, s21_decimal v2) {}
// int s21_is_greater_or_equal(s21_decimal v1, s21_decimal v2) {}
// int s21_is_equal(s21_decimal v1, s21_decimal v2) {}
// int s21_is_not_equal(s21_decimal v1, s21_decimal v2) {}
//
// int s21_from_int_to_decimal(int src, s21_decimal *dst) {}
// int s21_from_float_to_decimal(float src, s21_decimal *dst) {}
// int s21_from_decimal_to_int(s21_decimal src, int *dst) {}
// int s21_from_decimal_to_float(s21_decimal src, float *dst) {}
//
// int s21_floor(s21_decimal value, s21_decimal *result) {}
// int s21_round(s21_decimal value, s21_decimal *result) {}
// int s21_truncate(s21_decimal value, s21_decimal *result) {}
// int s21_negate(s21_decimal value, s21_decimal *result) {}

// OTHER FUNCTIONS

S21_STATIC_KEYWORD int __s21_sub_intfield(const uint32_t operand1[],
                                          const uint32_t operand2[],
                                          uint32_t result[],
                                          uint32_t intfield_size) {
  __s21_2s_complement_intfield(operand2, result, intfield_size);
  __s21_add_intfield(operand1, result, result, intfield_size);
  __s21_2s_complement_intfield(result, result, intfield_size);
  return OK;
}

S21_STATIC_KEYWORD uint32_t __s21_level(s21_decimal *a, s21_decimal *b,
                                        uint32_t a_double_mantissa[],
                                        uint32_t b_double_mantissa[],
                                        uint32_t intfield_size,
                                        uint32_t *old_exp) {
  uint32_t exp_a = __s21_read_exponent(*a), exp_b = __s21_read_exponent(*b),
           exp_diff = MAX(exp_a, exp_b) - MIN(exp_a, exp_b),
           levelled_mantissa[intfield_size << 1], rval = OK;
  s21_memset(levelled_mantissa, 0, sizeof(levelled_mantissa));
  if (MAX(exp_a, exp_b) <= S21_MAX_DECIMAL_EXPONENT) {
    s21_memcpy(a_double_mantissa, a->uint_data, intfield_size * sizeof(a[0]));
    s21_memcpy(b_double_mantissa, b->uint_data, intfield_size * sizeof(a[0]));
    if (exp_a < exp_b) {
      *old_exp = exp_a;
      __s21_write_exponent(a->uint_data, exp_b);
      __s21_mul_intfield(a_double_mantissa, powers_of_ten[exp_diff],
                         levelled_mantissa, intfield_size << 1);
      s21_memcpy(a_double_mantissa, levelled_mantissa,
                 sizeof(levelled_mantissa));
    } else {
      *old_exp = exp_b;
      __s21_write_exponent(b->uint_data, exp_a);
      __s21_mul_intfield(b_double_mantissa, powers_of_ten[exp_diff],
                         levelled_mantissa, intfield_size << 1);
      s21_memcpy(b_double_mantissa, levelled_mantissa,
                 sizeof(levelled_mantissa));
    }
  }
  return rval;
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

S21_STATIC_KEYWORD int16_t s21_rmemcmp(const void *lhs, const void *rhs,
                                       uint32_t size /*in bytes */) {
  int answ = 0;
  --size;
  while (!answ) {
    // printf("lhs[%d] = %d, rhs[%d] = %d\n", size, ((uint8_t *)lhs)[size],
    // size, ((uint8_t *)rhs)[size]);
    answ = ((uint8_t *)lhs)[size] - ((uint8_t *)rhs)[size];
    if (size == 0) break;
    --size;
  }
  return answ;
}

// Function to shift a decimal to the left by a given amount
S21_STATIC_KEYWORD void __s21_left_shift_intfield(const uint32_t operand[],
                                                  uint32_t shift,
                                                  uint32_t result[],
                                                  uint32_t intfield_size) {
  s21_memcpy(result, operand, intfield_size * sizeof(result[0]));
  uint32_t carry, dword_idx, shift_step;
  for (dword_idx = 0, shift_step = MIN(shift, 31), carry = 0; shift;
       shift -= shift_step, dword_idx = 0, shift_step = MIN(shift, 31),
      carry = 0)
    do {
      uint32_t temp = operand[dword_idx] << (shift_step) | carry;
      carry = operand[dword_idx] >> (32 - shift_step);
      result[dword_idx] = temp;
    } while (dword_idx++ < intfield_size - 1);
}

S21_STATIC_KEYWORD void __s21_right_shift_intfield(const uint32_t operand[],
                                                   uint32_t shift,
                                                   uint32_t result[],
                                                   uint32_t intfield_size) {
  s21_memcpy(result, operand, intfield_size * sizeof(result[0]));
  uint32_t carry, dword_num, shift_step;
  for (dword_num = intfield_size - 1, shift_step = MIN(shift, 31), carry = 0;
       shift; shift -= shift_step, dword_num = intfield_size - 1,
      shift_step = MIN(shift, 31), carry = 0)
    do {
      uint32_t temp = operand[dword_num] >> (shift_step) | carry;
      carry = operand[dword_num] << (32 - shift_step);
      result[dword_num] = temp;
    } while (dword_num--);
}

S21_STATIC_KEYWORD void __s21_negate_intfield(const uint32_t data[],
                                              uint32_t result[],
                                              uint32_t intfield_size) {
  for (uint32_t i = 0; i < intfield_size; ++i) result[i] = ~data[i];
}

S21_STATIC_KEYWORD void __s21_2s_complement_intfield(const uint32_t value[],
                                                     uint32_t result[],
                                                     uint32_t intfield_size) {
  __s21_negate_intfield(value, result, intfield_size);
  __s21_add_intfield(result, powers_of_ten[0], result, intfield_size);
}

S21_STATIC_KEYWORD
uint32_t __s21_read_bits(const uint32_t value[], const uint32_t bit_offset,
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

S21_STATIC_KEYWORD void __s21_write_bits(uint32_t data[], const uint32_t value,
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

// S21_STATIC_KEYWORD int __s21_toggle_bit(uint32_t value[],
//                                         const uint32_t bit_offset) {
//   int rval = -bit_offset;
//   if (bit_offset < S21_DECIMAL_SIZE_IN_BITS) {
//     value[bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS] ^=
//         (1 << (bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS));
//     rval = OK;
//   }
//   return rval;
// }

S21_STATIC_KEYWORD uint32_t __s21_get_top_bit_pos(const uint32_t data[],
                                                  uint32_t intfield_size) {
  uint32_t i = (intfield_size << 5) - 1;
  while (1) {
    if (__s21_read_bits(data, i, 1) || !i) break;
    --i;
  }
  return i;
}
