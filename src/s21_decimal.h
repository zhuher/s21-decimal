#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#ifndef S21_DECIMAL_SIZE_IN_INTS
#define S21_DECIMAL_SIZE_IN_INTS (4)
#endif

typedef struct {
  union {
    int int_data[S21_DECIMAL_SIZE_IN_INTS];
    unsigned int uint_data[S21_DECIMAL_SIZE_IN_INTS];
    unsigned char
        char_data[S21_DECIMAL_SIZE_IN_INTS * (sizeof(int) / sizeof(char))];
  };
} s21_decimal;

// ERROR CODES
#define FALSE 0;
#define TRUE 1;
#define OK 0;
#define TOO_BIG 1;
#define TOO_SMALL 2;
#define ZERO_DIVIDING 3;
#define CONVERT_ERROR 1;
#define CALC_ERROR 1;

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

static inline unsigned int s21_get_bit(const s21_decimal value,
                                       int bit_offset) {
  unsigned int data_unit_size = sizeof(value.uint_data[0]) << 3,
               rval = 0xDEADBEEF;
  if (bit_offset < sizeof(value.uint_data) << 3 && bit_offset >= 0)
    rval = (value.uint_data[bit_offset / data_unit_size] >>
            (bit_offset % data_unit_size)) &
           1;
  return rval;
}

static inline unsigned int s21_set_bit(s21_decimal *value, int bit_offset) {
  unsigned int data_unit_size = sizeof(value->uint_data[0]) << 3, rval = 0;
  if (bit_offset < sizeof(value->uint_data) << 3 && bit_offset >= 0)
    rval = value->uint_data[bit_offset / data_unit_size] |=
        (1 << (bit_offset % data_unit_size));
  return rval;
}

static inline unsigned int s21_clear_bit(s21_decimal *value, int bit_offset) {
  unsigned int data_unit_size = sizeof(value->uint_data[0]) << 3, rval = 1;
  if (bit_offset < sizeof(value->uint_data) << 3 && bit_offset >= 0)
    rval = value->uint_data[bit_offset / data_unit_size] &=
        ~(1 << (bit_offset % data_unit_size));
  return rval;
}

static inline unsigned int s21_toggle_bit(s21_decimal *value, int bit_offset) {
  unsigned int data_unit_size = sizeof(value->uint_data[0]) << 3,
               rval = 0xDEADBEEF;
  if (bit_offset < sizeof(value->uint_data) << 3 && bit_offset >= 0)
    rval = value->uint_data[bit_offset / data_unit_size] ^=
        (1 << (bit_offset % data_unit_size));
  return rval;
}

#ifdef EOF  // Declare functions that rely on stdio.h
static inline void s21_print_hex(const s21_decimal value) {
  for (int i = S21_DECIMAL_SIZE_IN_INTS - 1; i >= 0; i--) {
    printf("%08X%s", value.uint_data[i], i ? " " : "");
  }
}
static inline void s21_print_bits(const s21_decimal value, unsigned int from,
                                  unsigned int amount) {
  unsigned int i = from + amount - 1, bit = 0;
  while (i >= from && i < sizeof(value) << 3) {
    unsigned int bit = s21_get_bit(value, i);
    printf("%s%u\x1B[0m", bit ? "\x1B[32m" : "\x1B[31m", bit);
    if (i == from) break;
    --i;
  }
}

#define s21_print_binary(value) s21_print_bits(value, 0, sizeof(value) << 3)

static inline void s21_print_hex_bin(const s21_decimal value) {
  s21_print_hex(value);
  printf("\n");
  for (int j = 0; j < sizeof(int); ++j) {
    for (int i = S21_DECIMAL_SIZE_IN_INTS - 1; i >= 0; --i) {
      s21_print_bits(value, i * (sizeof(int) << 3) + (sizeof(int) << 1) * j,
                     (sizeof(int) << 1));
      if (i) printf(" ");
    }
    if (j < sizeof(int) - 1) printf("\n");
  }
}
#endif

#endif  // S21_DECIMAL_H
