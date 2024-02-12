#include "s21_decimal.h"

// TASK FUNCTIONS
// WIP
int s21_add(s21_decimal v1, s21_decimal v2, s21_decimal *result) { return 0; }
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

S21_STATIC_KEYWORD unsigned int s21_get_bit(const s21_decimal value,
                                            int bit_offset) {
  unsigned int data_unit_size = sizeof(value.uint_data[0]) << 3,
               rval = 0xDEADBEEF;
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0)
    rval = (value.uint_data[bit_offset / data_unit_size] >>
            (bit_offset % data_unit_size)) &
           1;
  else
    rval = bit_offset;
  return rval;
}

S21_STATIC_KEYWORD unsigned int s21_set_bit(s21_decimal *value,
                                            int bit_offset) {
  unsigned int data_unit_size = sizeof(value->uint_data[0]) << 3, rval = 0;
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0) {
    rval = 1;
    value->uint_data[bit_offset / data_unit_size] |=
        (1 << (bit_offset % data_unit_size));
  } else
    rval = bit_offset;
  return rval;
}

S21_STATIC_KEYWORD unsigned int s21_clear_bit(s21_decimal *value,
                                              int bit_offset) {
  unsigned int data_unit_size = sizeof(value->uint_data[0]) << 3, rval = 1;
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0) {
    rval = 0;
    value->uint_data[bit_offset / data_unit_size] &=
        ~(1 << (bit_offset % data_unit_size));
  } else
    rval = bit_offset;
  return rval;
}

S21_STATIC_KEYWORD unsigned int s21_toggle_bit(s21_decimal *value,
                                               int bit_offset) {
  unsigned int data_unit_size = sizeof(value->uint_data[0]) << 3,
               rval = 0xDEADBEEF;
  if (bit_offset < S21_DECIMAL_SIZE_IN_BITS && bit_offset >= 0)
    rval = value->uint_data[bit_offset / data_unit_size] ^=
        (1 << (bit_offset % data_unit_size));
  else
    rval = bit_offset;
  return rval;
}
