#include "s21_decimal.h"

// TASK FUNCTIONS
// WIP
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) {
  unsigned int carry = 0, rval = OK;
  for (int i = 0; i < S21_DECIMAL_SIZE_IN_INTS; ++i) {
    unsigned int temp = v1.uint_data[i] + v2.uint_data[i] + carry;
    carry = temp < v1.uint_data[i] || temp < v2.uint_data[i];
    result->uint_data[i] = temp;
    if (__s21_read_bit(*result, (S21_DECIMAL_SIZE_IN_INTS - 1) << 5)) {
      rval = TOO_BIG;
      break;
    }
  }
  return rval;
}
// TODO
// int s21_sub(s21_decimal v1, s21_decimal v2, s21_decimal *result) {}
// int s21_mul(s21_decimal v1, s21_decimal v2, s21_decimal *result) {}
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

// Function to initialize(and memset) a decimal to 0
s21_decimal s21_decimal_init(void) {
  // Initialize the return value
  s21_decimal rval;
  // Memset all the bytes to 0
  for (int i = 0; i < sizeof(s21_decimal); ++i) {
    ((char *)&rval)[i] = 0;
  }
  // Return the return value
  return rval;
};

// Function to shift a decimal to the left by one
S21_STATIC_KEYWORD unsigned int __s21_left_shift_one(s21_decimal value,
                                                     s21_decimal *result) {
  // Initialize the carry and the return value
  unsigned int carry = 0, rval = OK;
  // Loop through all but the last data unit in reverse order
  for (int i = S21_DECIMAL_SIZE_IN_INTS - 2; i >= 0; --i) {
    // Check if the result is too big
    if (__s21_read_bit(*result, 96)) {
      // Set the return value to TOO_BIG and break the loop
      rval = TOO_BIG;
      break;
    }
    // Shift the value to the left by one, accounting for the carry
    unsigned int temp = value.uint_data[i] << 1 | carry;
    // Update the carry
    carry = value.uint_data[i] >> 31;
    // Update the result
    result->uint_data[i] = temp;
  }
  // Return the return value
  return rval;
}

// Function to shift a decimal to the left by a given amount
S21_STATIC_KEYWORD unsigned int __s21_left_shift(s21_decimal value,
                                                 unsigned int shift,
                                                 s21_decimal *result) {
  // Initialize the return value
  unsigned int rval = OK;
  // Shift the value to the left while the shift is not 0 and the return value
  // is OK
  while (shift-- && !rval) {
    // Shift the value to the left by one
    rval = __s21_left_shift_one(value, result);
    // Update the value
    value = *result;
  }
  // Return the return value
  return rval;
}

S21_STATIC_KEYWORD unsigned int __s21_right_shift_one(s21_decimal value,
                                                      s21_decimal *result) {
  // Initialize the carry and the return value
  unsigned int carry = 0, rval = OK;
  // Loop through all but the last data unit
  for (int i = 0; i < S21_DECIMAL_SIZE_IN_INTS - 1; ++i) {
    // Shift the value to the right by one, accounting for the carry from the
    // previous iteration
    unsigned int temp = value.uint_data[i] >> 1 | carry;
    // Update the carry
    carry = (value.uint_data[i] & 1) << 31;
    // Update the result
    result->uint_data[i] = temp;
  }
  // Return the return value
  return rval;
}

S21_STATIC_KEYWORD unsigned int __s21_right_shift(s21_decimal value,
                                                  unsigned int shift,
                                                  s21_decimal *result) {
  // Initialize the return value
  unsigned int rval = OK;
  // Shift the value to the right while the shift is not 0 and the return value
  // is OK
  while (shift-- && rval == OK) {
    // Shift the value to the right by one
    rval = __s21_right_shift_one(value, result);
    // Update the value
    value = *result;
  }
  // Return the return value
  return rval;
}

S21_STATIC_KEYWORD
unsigned int __s21_read_bits(const s21_decimal value, unsigned int bit_offset,
                             unsigned int bit_count) {
  // Initialize the return value and the size of the data unit
  unsigned int rval = OK;
  // Check if the bit offset and the bit count are valid
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0 &&
      bit_count > 0 && bit_count <= S21_DECIMAL_SIZE_IN_BITS) {
    // Calculate the unit and the offset
    unsigned int unit = bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS,
                 offset = bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS;
    // Check if the bits are in the same data unit
    // If they are, then the return value is the bits from the offset
    if (offset / S21_DECIMAL_UNIT_SIZE_IN_BITS ==
        (offset + bit_count - 1) / S21_DECIMAL_UNIT_SIZE_IN_BITS) {
      // Operation explained:
      // 1. Get the bits from the offset
      // 2. Return the result
      rval = (value.uint_data[unit] >> offset) & ((1 << bit_count) - 1);
    }
    // If they are not, then the return value is the bits from the offset, and
    // some bits from the next data unit
    else {
      // Operation explained:
      // 1. Get the bits from the offset
      // 2. Get the bits from the next data unit
      // 3. Combine the bits from the offset and the bits from the next data
      // unit
      // 4. Return the result
      rval =
          (value.uint_data[unit] >> offset) |
          ((value.uint_data[unit + 1] &
            ((1 << (bit_count - (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset))) - 1))
           << (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset));
    }
  }
  return rval;
}

S21_STATIC_KEYWORD int __s21_write_bits(s21_decimal *value, unsigned int data,
                                        unsigned int bit_offset,
                                        unsigned int bit_count) {
  // initialize S21_DECIMAL_UNIT_SIZE_IN_BITS
  unsigned int rval = -bit_offset;
  // check if bit_offset is within the range of the decimal
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0 &&
      bit_count > 0 && bit_count <= S21_DECIMAL_SIZE_IN_BITS) {
    // calculate the unit and offset of the bit_offset
    unsigned int unit = bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS,
                 offset = bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS;
    // check if the bits to be written are within the same unit
    // if the bits to be written are within the same unit
    if (offset / S21_DECIMAL_UNIT_SIZE_IN_BITS ==
        (offset + bit_count - 1) / S21_DECIMAL_UNIT_SIZE_IN_BITS) {
      // clear the bits to be written
      value->uint_data[unit] &= ~(((1 << bit_count) - 1) << offset);
      // write the bits to the decimal
      value->uint_data[unit] |= (data << offset);
    }
    // if the bits to be written are not within the same unit
    else {
      // clear the bits of the first unit to be written
      value->uint_data[unit] &=
          ~((1 << (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset)) - 1);
      // write the bits to the first unit
      value->uint_data[unit] |= (data << offset);
      // clear the bits of the second unit to be written
      value->uint_data[unit + 1] &=
          ~((1 << (bit_count - (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset))) - 1);
      // write the bits to the second unit
      value->uint_data[unit + 1] |=
          (data >> (S21_DECIMAL_UNIT_SIZE_IN_BITS - offset));
    }
    rval = OK;
  }
  return rval;
}

S21_STATIC_KEYWORD int __s21_toggle_bit(s21_decimal *value,
                                        unsigned int bit_offset) {
  unsigned int rval = -bit_offset;
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0) {
    value->uint_data[bit_offset / S21_DECIMAL_UNIT_SIZE_IN_BITS] ^=
        (1 << (bit_offset % S21_DECIMAL_UNIT_SIZE_IN_BITS));
    rval = OK;
  }
  return rval;
}
