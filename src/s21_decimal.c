#include "s21_decimal.h"

// #include <stdio.h>
// TASK FUNCTIONS
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  unsigned int bigv1[(S21_DECIMAL_SIZE_IN_INTS - 1) << 1],
      bigv2[(S21_DECIMAL_SIZE_IN_INTS - 1) << 1],
      bigresult[(S21_DECIMAL_SIZE_IN_INTS - 1) << 1], rval = OK;
  s21_memset(bigv1, 0, sizeof(bigv1));
  s21_memset(bigv2, 0, sizeof(bigv2));
  s21_memset(bigresult, 0, sizeof(bigresult));
  __s21_level(v1.uint_data, v2.uint_data, bigv1, bigv2,
              S21_DECIMAL_SIZE_IN_INTS - 1);
  rval = __s21_add_intfield(bigv1, bigv2, bigresult,
                            (S21_DECIMAL_SIZE_IN_INTS - 1) << 1);
  s21_memcpy(result, bigresult,
             (S21_DECIMAL_SIZE_IN_INTS - 1) * sizeof(bigresult[0]));
  // TODO: deexponentiate le result
  if (__s21_get_top_bit(bigresult, ((S21_DECIMAL_SIZE_IN_INTS - 1) << 1)) >
      ((S21_DECIMAL_SIZE_IN_INTS - 1) << 5) - 1)
    rval = TOO_BIG;
  return rval;
  // return __s21_add_intfield(v1.uint_data, v2.uint_data, result->uint_data,
  // S21_DECIMAL_SIZE_IN_INTS - 1);
}
// WIP
S21_STATIC_KEYWORD int __s21_add_intfield(const unsigned int v1[],
                                          const unsigned int v2[],
                                          unsigned int result[],
                                          unsigned int intfield_size) {
  unsigned int carry = 0;
  for (int i = 0; i < intfield_size; ++i) {
    unsigned int temp = v1[i] + v2[i] + carry;
    carry = temp < v1[i] || temp < v2[i];
    result[i] = temp;
  }
  return carry;
}
// TODO
// int s21_sub(s21_decimal v1, s21_decimal v2, s21_decimal *result) {}
int s21_mul(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  s21_memset(result->uint_data, 0, sizeof(s21_decimal));
  unsigned int rval =
      __s21_mul_intfield(v1.uint_data, v2.uint_data, result->uint_data,
                         S21_DECIMAL_SIZE_IN_INTS - 1);
  // result->sign = v1.sign ^ v2.sign;
  result->uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] =
      (v1.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] & 0x80000000) ^
      (v2.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] & 0x80000000);
  // printf("result->uint_data[S21_DECIMAL_SIZE_IN_INTS - 1]: %u\n",
  //        result->uint_data[S21_DECIMAL_SIZE_IN_INTS - 1]);
  // printf("v1.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1]: %u\n",
  //        v1.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1]);
  // printf("v2.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1]: %u\n",
  //        v2.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1]);
  // // print rval
  // if (rval == TOO_SMALL) {
  //   printf("TOO_SMALL\n");
  // } else if (rval == TOO_BIG) {
  //   printf("TOO_BIG\n");
  // } else {
  //   printf("OK\n");
  // }
  // exponent is the sum of the exponents of the two numbers being multiplied
  __s21_write_exponent(
      result->uint_data,
      __s21_read_exponent(v1.uint_data) + __s21_read_exponent(v2.uint_data));
  if (__s21_read_exponent((*result).uint_data) > 28) rval = TOO_SMALL;
  return rval;
}
S21_STATIC_KEYWORD int __s21_mul_intfield(const unsigned int v1[],
                                          const unsigned int v2[],
                                          unsigned int result[],
                                          unsigned int intfield_size) {
  unsigned int temp[intfield_size];
  s21_memset(temp, 0, intfield_size * sizeof(v1[0]));
  unsigned int rval = OK;
  unsigned int i = (intfield_size * (sizeof(v1[0]) << 3)) - 1;
  while (rval == OK) {
    if (__s21_read_bit(v2, i)) {
      __s21_left_shift_intfield(v1, i, temp, intfield_size);
      rval = __s21_add_intfield(result, temp, result, intfield_size);
      ;
    }
    if (i == 0) break;
    --i;
  }
  return rval;
}
// int s21_div(s21_decimal v1, s21_decimal v2, s21_decimal *result) {}
//
// int s21_is_less(s21_decimal v1, s21_decimal v2) {}
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

S21_STATIC_KEYWORD void __s21_swap(unsigned int *a, unsigned int *b) {
  *a = *a ^ *b;
  *b = *a ^ *b;
  *a = *a ^ *b;
}
// not ready
S21_STATIC_KEYWORD unsigned int __s21_level(unsigned int a[], unsigned int b[],
                                            unsigned int a_extended[],
                                            unsigned int b_extended[],
                                            unsigned int intfield_size) {
  unsigned int exp_a = __s21_read_exponent(a), exp_b = __s21_read_exponent(b),
               exp_diff = MAX(exp_a, exp_b) - MIN(exp_a, exp_b),
               tenpower[intfield_size << 1], ten[intfield_size << 1],
               tempten[intfield_size << 1], rval = OK;
  s21_memset(tenpower, 0, sizeof(tenpower));
  s21_memset(ten, 0, sizeof(ten));
  s21_memset(tempten, 0, sizeof(tempten));
  if (MAX(exp_a, exp_b) > S21_MAX_DECIMAL_EXPONENT)
    rval = CONVERT_ERROR;
  else if (exp_diff > 28) {
    rval = TOO_SMALL;
  } else {
    tenpower[0] = 1;
    ten[0] = 10;
    for (unsigned int i = 0; i < exp_diff; ++i) {
      __s21_mul_intfield(tenpower, ten, tempten, intfield_size << 1);
      s21_memcpy(tenpower, tempten, sizeof(tempten));
      s21_memset(tempten, 0, sizeof(tempten));
    }
    s21_memcpy(a_extended, a, intfield_size * sizeof(a[0]));
    s21_memcpy(b_extended, b, intfield_size * sizeof(a[0]));
    s21_memset(tempten, 0, sizeof(tempten));
    if (exp_a < exp_b) {
      __s21_write_exponent(a, exp_b);
      __s21_mul_intfield(a_extended, tenpower, tempten, intfield_size << 1);
      s21_memcpy(a_extended, tempten, sizeof(tempten));
    } else {
      __s21_write_exponent(b, exp_a);
      __s21_mul_intfield(b_extended, tenpower, tempten, intfield_size << 1);
      s21_memcpy(b_extended, tempten, sizeof(tempten));
    }
  }
  return rval;
}

void *s21_memset(void *data, char value, unsigned int size) {
  for (unsigned int i = 0; i < size; ++i) {
    ((char *)data)[i] = value;
  }
  return data;
};

void *s21_memcpy(void *dest, const void *src, unsigned int size) {
  for (unsigned int i = 0; i < size; ++i) {
    // printf("Copying %u to byte %u\n", ((char *)src)[i], i);
    ((char *)dest)[i] = ((char *)src)[i];
  }
  return dest;
}

S21_STATIC_KEYWORD unsigned int __s21_left_shift_intfield_one(
    const unsigned int value[], unsigned int result[],
    unsigned int intfield_size) {
  unsigned int carry = 0;
  unsigned int i = 0;
  do {
    unsigned int temp = value[i] << 1 | carry;
    carry = value[i] >> 31;
    result[i] = temp;
  } while (i++ < intfield_size - 1);
  return OK;
}

// Function to shift a decimal to the left by a given amount
S21_STATIC_KEYWORD unsigned int __s21_left_shift_intfield(
    const unsigned int value[], unsigned int shift, unsigned int result[],
    unsigned int intfield_size) {
  s21_memcpy(result, value, intfield_size * sizeof(result[0]));
  while (shift--)
    __s21_left_shift_intfield_one(result, result, S21_DECIMAL_SIZE_IN_INTS - 1);
  return OK;
}
S21_STATIC_KEYWORD unsigned int __s21_right_shift_intfield_one(
    const unsigned int value[], unsigned int result[],
    unsigned int intfield_size) {
  // Initialize the carry
  unsigned int carry = 0;
  // Loop through all but the last data unit
  unsigned int i = intfield_size - 1;
  do {
    // Shift the value to the right by one, accounting for the carry
    unsigned int temp = value[i] >> 1 | carry;
    // Update the carry
    carry = (value[i] & 1) << 31;
    // Update the result
    result[i] = temp;
  } while (i--);
  // Always succeeds
  return OK;
}

S21_STATIC_KEYWORD unsigned int __s21_right_shift_intfield(
    const unsigned int value[], unsigned int shift, unsigned int result[],
    unsigned int intfield_size) {
  // Copy the value onto the result
  s21_memcpy(result, value, intfield_size * sizeof(result[0]));
  // Shift the value to the right by one while the shift is not 0 and the
  // return value is OK (might always be OK, but just in case it changes in
  // the future)
  while (shift--) __s21_right_shift_intfield_one(result, result, intfield_size);
  // Return the return value
  return OK;
}

S21_STATIC_KEYWORD
unsigned int __s21_read_bits(const unsigned int value[],
                             const unsigned int bit_offset,
                             const unsigned int bit_count) {
  unsigned int rval = 0xDEADBEEF;  // This value is never used
  unsigned int unit = bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS,
               offset = bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS;
  if (offset / S21_DECIMAL_UNIT_SIZE_IN_BITS ==
      (offset + bit_count - 1) / S21_DECIMAL_UNIT_SIZE_IN_BITS)
    rval = (value[unit] >> offset) & ((1 << bit_count) - 1);
  else
    rval =
        (value[unit] >> offset) |
        ((value[unit + 1] &
          ((1 << (bit_count - (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset))) - 1))
         << (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset));
  return rval;
}

S21_STATIC_KEYWORD int __s21_write_bits(unsigned int value[],
                                        const unsigned int data,
                                        const unsigned int bit_offset,
                                        const unsigned int bit_count) {
  unsigned int unit = bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS,
               offset = bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS;
  if (offset / S21_DECIMAL_UNIT_SIZE_IN_BITS ==
      (offset + bit_count - 1) / S21_DECIMAL_UNIT_SIZE_IN_BITS) {
    value[unit] &= bit_count == S21_DECIMAL_UNIT_SIZE_IN_BITS
                       ? 0
                       : ~(((1 << bit_count) - 1) << offset);
    value[unit] |= ((bit_count == S21_DECIMAL_UNIT_SIZE_IN_BITS
                         ? ~(unsigned int)(0)
                         : (((1 << bit_count) - 1) << offset)) &
                    (data << offset));
  } else {
    value[unit] &= ((1 << offset) - 1);
    value[unit] |= (~((1 << offset) - 1) & (data << offset));
    value[unit + 1] &=
        ~((1 << (bit_count - (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset))) - 1);
    value[unit + 1] |= (data >> (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset));
  }
  return OK;
}

S21_STATIC_KEYWORD int __s21_toggle_bit(unsigned int value[],
                                        const unsigned int bit_offset) {
  int rval = -bit_offset;
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0) {
    value[bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS] ^=
        (1 << (bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS));
    rval = OK;
  }
  return rval;
}

S21_STATIC_KEYWORD int __s21_get_top_bit(const unsigned int value[],
                                         unsigned int intfield_size) {
  unsigned int i = (intfield_size * sizeof(value[0]) << 3) - 1;
  while (i >= 0) {
    if (__s21_read_bits(value, i, 1)) return i;
    --i;
  }
}
