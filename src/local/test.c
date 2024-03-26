#include <stdint.h>
#include <stdio.h>

#include "../s21_decimal.h"

void s21_set_and_print_bit(s21_decimal *const value, const uint32_t bit,
                           S21_FLAGS flags) {
  printf("\n\nSetting bit %u\n", bit);
  s21_write_bits(value->bits, 1, bit, 1);
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
void s21_clear_and_print_bit(s21_decimal *const value, const uint32_t bit,
                             S21_FLAGS flags) {
  printf("\n\nClearing bit %u\n", bit);
  s21_write_bits(value->bits, 1, bit, 1);
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
// void s21_toggle_and_print_bit(s21_decimal *const value,
// const uint32_t bit) {
// printf("\n\nToggling bit %u\n", bit);
// if (!__s21_toggle_bit(value->bits, bit))
// s21_print_hex_bin(value->bits, S21_DECIMAL_SIZE_IN_INTS,
// s21_get_exponent(*value) | (s21_is_decimal_negative(*value)<<15)); else
// printf("Failed to toggle bit %u", bit);
// }
void s21_write_and_print_exponent(s21_decimal *const value, const uint32_t exp,
                                  S21_FLAGS flags) {
  printf("\n\nWriting exponent %u\n", exp);
  s21_set_exponent(*value, exp);
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
void s21_read_and_print_exponent(const s21_decimal *const value) {
  printf("Exponent: %u\n", s21_get_exponent(*value));
}

void s21_shift_left_and_print(s21_decimal *value, const uint32_t amount,
                              S21_FLAGS flags) {
  printf("\n\nShifting left by %u\n", amount);
  s21_left_shift_intfield(value->bits, amount, value->bits, 3, &(uint32_t){0});
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
void s21_shift_right_and_print(s21_decimal *value, const uint32_t amount,
                               S21_FLAGS flags) {
  printf("Shifting right by %u\n", amount);
  s21_right_shift_intfield(value->bits, amount, value->bits, 3, &(uint32_t){0});
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
void s21_rotate_left_and_print(s21_decimal *value, const uint32_t amount,
                               S21_FLAGS flags) {
  printf("\n\nRotating...\n");
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
  printf("by %u\n", amount);
  s21_left_rotate_intfield(value->bits, amount, value->bits, 3);
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
void s21_rotate_right_and_print(s21_decimal *value, const uint32_t amount,
                                S21_FLAGS flags) {
  printf("\n\nRotating...\n");
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
  printf("by %u\n", amount);
  s21_right_rotate_intfield(value->bits, amount, value->bits, 3);
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
}
void s21_add_and_print(s21_decimal *value, const s21_decimal *other,
                       uint32_t *temp, S21_FLAGS flags) {
  s21_decimal tarray = {0};
  printf("\nAdding\n");
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
  printf("+\n");
  s21_print_hex_bin(
      other->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*other) | (s21_is_decimal_negative(*other) << 15),
      flags);
  printf("=");
  *temp = s21_add(*value, *other, &tarray);
  *value = tarray;
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);

  if (*temp == TOO_BIG)
    printf("Too big!\n");
  else if (*temp == OK) {
  } else
    printf("Unknown\n");
}

void s21_sub_and_print(s21_decimal *value, const s21_decimal *other,
                       uint32_t *temp, S21_FLAGS flags) {
  s21_decimal tarray = {0};
  printf("\nSubtracting\n");
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
  printf("-\n");
  s21_print_hex_bin(
      other->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*other) | (s21_is_decimal_negative(*other) << 15),
      flags);
  printf("=");
  *temp = s21_sub(*value, *other, &tarray);
  *value = tarray;
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);

  if (*temp == TOO_SMALL)
    printf("Too small!\n");
  else if (*temp == OK) {
  } else
    printf("Unknown\n");
}

void s21_mul_and_print(s21_decimal *value, const s21_decimal *other,
                       uint32_t *temp, S21_FLAGS flags) {
  printf("\nMultiplying\n");
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
  printf("*\n");
  s21_print_hex_bin(
      other->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*other) | (s21_is_decimal_negative(*other) << 15),
      flags);
  printf("=");
  *temp = s21_mul(*value, *other, value);
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);

  if (*temp == OK) {
  } else if (*temp == TOO_BIG)
    printf("Too big!\n");
  else if (*temp == TOO_SMALL)
    printf("Too small!\n");
  else
    printf("Unknown\n");
}

void s21_write_bits_and_print(s21_decimal *container, const uint32_t from,
                              const uint32_t amount, uint32_t data,
                              S21_FLAGS flags) {
  printf("\n\nWriting ");
  s21_print_bits(&data, 0, 32);
  printf(" to bits %u:%u\n", from, from + amount - 1);
  s21_write_bits(container->bits, data, from, amount);
  s21_print_hex_bin(container->bits, S21_DECIMAL_SIZE_IN_INTS,
                    s21_get_exponent(*container) |
                        (s21_is_decimal_negative(*container) << 15),
                    flags);
}
void s21_compare_and_print(const s21_decimal *value, const s21_decimal *other,
                           S21_FLAGS flags) {
  printf("\n\nComparing\n");
  s21_print_hex_bin(
      value->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*value) | (s21_is_decimal_negative(*value) << 15),
      flags);
  printf("VS\n");
  s21_print_hex_bin(
      other->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*other) | (s21_is_decimal_negative(*other) << 15),
      flags);
  printf("..\n");
  if (s21_is_greater_or_equal(*value, *other))
    printf("Greater or equal!\n");
  else
    printf("Not greater or equal!\n");
  if (s21_is_less_or_equal(*value, *other))
    printf("Less or equal!\n");
  else
    printf("Not less or equal!\n");
  if (s21_is_greater(*value, *other))
    printf("Greater!\n");
  else
    printf("Not greater!\n");
  if (s21_is_less(*value, *other))
    printf("Less!\n");
  else
    printf("Not less!\n");
  if (s21_is_equal(*value, *other))
    printf("Equal!\n");
  else
    printf("Not equal!\n");
}
// void s21_div_and_print(s21_decimal *value, const s21_decimal *other,
// uint32_t *temp) {
// s21_decimal tarray = {0};
// printf("\n\nDividing\n");
// s21_print_hex_bin(value->bits, S21_DECIMAL_SIZE_IN_INTS,
// s21_get_exponent(*value) | (s21_is_decimal_negative(*value)<<15));
// printf("/\n"); s21_print_hex_bin(other->bits, S21_DECIMAL_SIZE_IN_INTS,
// s21_get_exponent(*other) | (s21_is_decimal_negative(*other)<<15));
// printf("=\n"); *temp = s21_div(*value, *other, &tarray); *value = tarray;
// s21_print_hex_bin(value->bits, S21_DECIMAL_SIZE_IN_INTS,
// s21_get_exponent(*value) | (s21_is_decimal_negative(*value)<<15));
//
// if (*temp == OK) {
// } else if (*temp == TOO_BIG)
// printf("Too big!\n");
// else if (*temp == TOO_SMALL)
// printf("Too small!\n");
// else
// printf("Unknown\n");
// }
void s21_div_and_print(s21_decimal *dividend, s21_decimal *divisor,
                       S21_FLAGS flags) {
  printf("\n\nDividing\n");
  s21_print_hex_bin(
      dividend->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*dividend) | (s21_is_decimal_negative(*dividend) << 15),
      flags);
  printf("/\n");
  s21_print_hex_bin(
      divisor->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*divisor) | (s21_is_decimal_negative(*divisor) << 15),
      flags);
  printf("=\n");
  s21_div(*dividend, *divisor, dividend);
  s21_print_hex_bin(
      dividend->bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(*dividend) | (s21_is_decimal_negative(*dividend) << 15),
      flags);
}
int main() {
  s21_decimal dec, ced, ded;
  s21_memset(dec.bits, 0, sizeof(dec.bits));
  s21_memset(ced.bits, 0, sizeof(ced.bits));
  s21_memset(ded.bits, 0, sizeof(ded.bits));
  for (uint32_t i = 0; i < S21_DECIMAL_SIZE_IN_BITS; ++i) {
    uint32_t temp = 0;
    printf("%u\n", temp);
    s21_print_hex_bin(
        dec.bits, S21_DECIMAL_SIZE_IN_INTS,
        s21_get_exponent(dec) | (s21_is_decimal_negative(dec) << 15),
        PRINT_ALL_SERVICE);
  }
  for (uint32_t i = 0; i < S21_DECIMAL_SIZE_IN_BITS; ++i) {
    uint32_t temp = 0;
    printf("%u\n", temp);
    s21_print_hex_bin(
        dec.bits, S21_DECIMAL_SIZE_IN_INTS,
        s21_get_exponent(dec) | (s21_is_decimal_negative(dec) << 15),
        PRINT_ALL_SERVICE);
  }
  printf("Size of s21_decimal: %lu\n", sizeof(dec));
  printf("Size of s21_decimal.bits: %lu\n", sizeof(dec.bits));
  for (uint32_t i = 0; i < sizeof(dec) / sizeof(dec.bits[0]); ++i)
    printf("Size of s21_decimal.bits[%d]: %lu, number inside is %u\n", i,
           sizeof(dec.bits[i]), dec.bits[i]);
  s21_set_and_print_bit(&dec, 0, PRINT_ALL_SERVICE);
  uint32_t temp = 0;
  while (!temp) {
    ced = dec;
    s21_add_and_print(&dec, &ced, &temp, PRINT_ALL_SERVICE);
  }
  s21_write_bits_and_print(&dec, 0, 32, 1 << 31 | 1, PRINT_ALL_SERVICE);
  s21_write_and_print_exponent(&dec, 13, PRINT_ALL_SERVICE);
  s21_read_and_print_exponent(&dec);
  s21_shift_left_and_print(&dec, 10, PRINT_ALL_SERVICE);
  s21_shift_right_and_print(&dec, 10, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 16, 32, 1 << 31 | 1, PRINT_ALL_SERVICE);
  s21_shift_left_and_print(&dec, 10, PRINT_ALL_SERVICE);
  s21_shift_right_and_print(&dec, 10, PRINT_ALL_SERVICE);
  s21_read_bits_and_print(dec.bits, 0, 128);
  printf("\n");
  s21_write_bits_and_print(&dec, 32, 32, 1 << 31 | 1, PRINT_ALL_SERVICE);
  s21_shift_right_and_print(&dec, 10, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 55, 30, 536870913, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 54, 1, 1, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 51, 1, 1, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 65, 6, 42, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 52, 1, 1, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 63, 2, 3, PRINT_ALL_SERVICE);
  s21_shift_left_and_print(&dec, 20, PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 96, 7, 89655232, PRINT_ALL_SERVICE);
  s21_read_bits_and_print(dec.bits, 65, 32);
  printf("\n");
  s21_add_and_print(&dec, &ced, &(uint32_t){0}, PRINT_ALL_SERVICE);
  s21_add_and_print(&dec, &ced, &(uint32_t){0}, PRINT_ALL_SERVICE);
  s21_print_hex_bin(
      dec.bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(dec) | (s21_is_decimal_negative(dec) << 15),
      PRINT_ALL_SERVICE);
  s21_write_bits_and_print(&dec, 43, 8, 150, PRINT_ALL_SERVICE);
  s21_shift_left_and_print(&dec, 50, PRINT_ALL_SERVICE);
  s21_decimal dummy;
  s21_memset(dummy.bits, 0, sizeof(dummy.bits));
  s21_print_hex_bin(
      dummy.bits, S21_DECIMAL_SIZE_IN_INTS,
      s21_get_exponent(dummy) | (s21_is_decimal_negative(dummy) << 15),
      PRINT_ALL_SERVICE);
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 1] = 0x80000000;
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 2] = 1488;
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 3] = 911;
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 4] = 10;
  s21_add_and_print(&dec, &dummy, &(uint32_t){0}, PRINT_BIN_DEC_SERVICE);
  s21_shift_right_and_print(&dec, 50, PRINT_ALL_SERVICE);
  s21_mul_and_print(&dec, &(s21_decimal){{10, 0, 0, 0x80000000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_add_and_print(&dec, &(s21_decimal){{0, 0, 69420, 0}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&dec, &(s21_decimal){{50, 0, 0, 0x80000000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_mul_and_print(&dec, &(s21_decimal){{10, 0, 0, 0x80000000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{876, 0, 0, 0x80000000}},
                    &(s21_decimal){{1874919424, 2328306, 0, 0x80000000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 1] = 0x80000000;
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 2] = 1488;
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 3] = 911;
  dummy.bits[S21_DECIMAL_SIZE_IN_INTS - 4] = 10;
  s21_mul_and_print(&dummy, &(s21_decimal){{10, 0, 0, 0x800D0000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{111111111, 0, 0, 0x800D0000}},
                    &(s21_decimal){{111111111, 0, 0, 0x800D0000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{111111111, 0, 0, 0xD0000}},
                    &(s21_decimal){{111111111, 0, 0, 0xD0000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{4294967295, 0, 0, 0xD0000}},
                    &(s21_decimal){{4294967295, 0, 0, 0xD0000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{4095, 0, 0, 0x10000}},
                    &(s21_decimal){{4095, 0, 0, 0xD0000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  printf("\n");
  printf("uint32_t powers_of_ten[29][6] = {\n");

  for (uint32_t exp = 0; exp < 29; ++exp) {
    // uint32_t i;
    // i = 5;
    // printf("{");
    // do {
    //   printf("%10u, ", powers_of_ten[exp][i]);
    // } while (i--);
    // printf("},\n");
    s21_print_hex_bin(powers_of_ten[exp], S21_DOUBLE_MANTISSA_SIZE, 0,
                      PRINT_ALL_SERVICE);
    printf(",\n");
  }
  printf("};\n");
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0}},
                    &(s21_decimal){{69420, 0, 0, 0x80000000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_shift_left_and_print(&(s21_decimal){{1490, 0, 0, 4 | (10 << 27)}}, 47,
                           PRINT_ALL_SERVICE);
  s21_shift_right_and_print(&(s21_decimal){{0, 48824320, 0, 4 | (10 << 27)}},
                            47, PRINT_ALL_SERVICE);
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0x20000}},
                    &(s21_decimal){{69420, 0, 0, 0x80000000}}, &(uint32_t){0},
                    PRINT_BIN_DEC_SERVICE);
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0x20000}},
                    &(s21_decimal){{69420, 0, 0, 0x80010000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0x80020000}},
                    &(s21_decimal){{69420, 0, 0, 0x40000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_compare_and_print(&(s21_decimal){{69420, 0, 0, 0x80050000}},
                        &(s21_decimal){{6942, 0, 0, 0x80040000}},
                        PRINT_DEC_SERVICE);
  s21_compare_and_print(&(s21_decimal){{1488, 0, 0, 0x30000}},
                        &(s21_decimal){{1488001, 0, 0, 0x60000}},
                        PRINT_DEC_SERVICE);
  s21_compare_and_print(&(s21_decimal){{1489, 0, 0, 0x30000}},
                        &(s21_decimal){{1488999, 0, 0, 0x60000}},
                        PRINT_DEC_SERVICE);
  s21_compare_and_print(&(s21_decimal){{1488, 0, 0, 0x30000}},
                        &(s21_decimal){{1488000, 0, 0, 0x60000}},
                        PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{7, 0, 0, 0x0}},
                    &(s21_decimal){{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{~0, ~0, ~0, 0x80000000}},
                    &(s21_decimal){{~0, ~0, ~0, 0x0}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{1, 0, 0, 0x0}},
                    &(s21_decimal){{~0, ~0, ~0, 0x0}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_add_and_print(&(s21_decimal){{6942, 0, 0, 0x80020000}},
                    &(s21_decimal){{694200, 0, 0, 0x40000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_sub_and_print(&(s21_decimal){{6942, 0, 0, 0x80020000}},
                    &(s21_decimal){{694200, 0, 0, 0x80040000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_sub_and_print(&(s21_decimal){{1488, 0, 0, 0x80020000}},
                    &(s21_decimal){{694200, 0, 0, 0x80040000}}, &(uint32_t){0},
                    PRINT_BIN_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{5, 0, 0, 0x10000}},
                    &(s21_decimal){{~0, ~0, ~0, 0x0}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{5, 0, 0, 0x10000}},
                    &(s21_decimal){{5, 0, 0, 0x0}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  // s21_div_and_print(&(s21_decimal){{~0, ~0, ~0, 0x0}},
  // &(s21_decimal){{2, 0, 0, 0x0}}, &(uint32_t){0});
  // s21_div_and_print(&(s21_decimal){{14880, 0, 0, 0x0}},
  // &(s21_decimal){{10, 0, 0, 0x0}}, &(uint32_t){0});
  // s21_is_decimal_divisible_by_10((uint32_t[]){622, 0, 0, 0x0}, 3);
  // s21_is_decimal_divisible_by_10((uint32_t[]){620, 0, 0, 0x0}, 3);
  // s21_is_decimal_divisible_by_10((uint32_t[]){~0 - 5, ~0, ~0, 0x0}, 3);
  // s21_is_decimal_divisible_by_10((uint32_t[]){1000000000, 0, 0, 0x0}, 3);
  // s21_rotate_left_and_print(
  //     &(s21_decimal){{0x00000000, 0, 0x80000000, 0x00000000}}, 1,
  //     PRINT_ALL_SERVICE);
  // s21_rotate_right_and_print(&(s21_decimal){{1, 0, 0, 0x00000000}}, 1,
  //                            PRINT_ALL_SERVICE);
  // s21_rotate_left_and_print(
  //     &(s21_decimal){{0x00000000, 0, 0xB0000000, 0x00000000}}, 2,
  //     PRINT_ALL_SERVICE);
  // s21_rotate_right_and_print(&(s21_decimal){{3, 0, 0, 0x00000000}}, 3,
  //                            PRINT_ALL_SERVICE);
  s21_sub_and_print(&(s21_decimal){{~0, ~0, ~0, 0x0}},
                    &(s21_decimal){{~1, ~0, ~0, 0x0}}, &(uint32_t){0},
                    PRINT_BIN_DEC_SERVICE);
  // s21_sub_and_print(&(s21_decimal){{0, 0, 0, 0x0}},
  //                   &(s21_decimal){{1, 0, 0, 0x0}}, &(uint32_t){0},
  //                   PRINT_DEC_SERVICE);
  // s21_div_and_print(&(s21_decimal){{14810, 0, 0, 0x0}},
  //                   &(s21_decimal){{2, 0, 0, 0x0}}, PRINT_DEC);
  // s21_div_and_print(&(s21_decimal){{14820, 0, 0, 0x0}},
  //                   &(s21_decimal){{1, 0, 0, 0x0}}, PRINT_DEC);
  // s21_div_and_print(&(s21_decimal){{610, 0, 0, 0x0}},
  //                   &(s21_decimal){{610, 0, 0, 0x0}}, PRINT_DEC);
  // s21_div_and_print(&(s21_decimal){{610, 0, 0, 0x0}},
  //                   &(s21_decimal){{10, 0, 0, 0x0}}, PRINT_DEC);
  // s21_div_and_print(&(s21_decimal){{679, 0, 0, 0x0}},
  //                   &(s21_decimal){{10, 0, 0, 0x0}}, PRINT_DEC);
  // // subtract unsigned 1 from unsigned 0 and print
  // printf("%u\n", (uint32_t)0 - (uint32_t)1);
  // uint32_t tsize = 6, rem[6] = {0}, quo[6] = {0}, test[tsize];
  // printf("Quotient: ");
  // for (uint32_t i = 5; i < 6; --i) printf("%u ", quo[i]);
  // printf("\nRemainder: ");
  // for (uint32_t i = 5; i < 6; --i) printf("%u ", rem[i]);
  // s21_memset(test, 0, sizeof(test));
  // printf("\n");
  // test[0] = 678;
  // s21_mul_intfield(test, powers_of_ten[27], test, 6);
  // s21_div_intfield((uint32_t[3]){~0, ~0, ~0}, powers_of_ten[9], quo, rem, 3);
  // printf("Quotient: ");
  // for (uint32_t i = 5; i < 6; --i) printf("%u ", quo[i]);
  // printf("\nRemainder: ");
  // for (uint32_t i = 5; i < 6; --i) printf("%u ", rem[i]);
  // printf("\n");
  // s21_memset(quo, 0, sizeof(quo));
  // s21_memset(rem, 0, sizeof(rem));
  // s21_div_intfield((uint32_t[3]){3047500985, 1266874889, 4},
  // powers_of_ten[9],
  //                  quo, rem, 3);
  // printf("Quotient: ");
  // for (uint32_t i = 5; i < 6; --i) printf("%u ", quo[i]);
  // printf("\nRemainder: ");
  // for (uint32_t i = 5; i < 6; --i) printf("%u ", rem[i]);
  // uint8_t str[31] = {0};
  // printf("Le result: '%s'\n", s21_dtoa((uint32_t[3]){~0, ~0, ~0}, 28, 3,
  // str));
  s21_decimal d1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal d2 = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}};
  s21_mul_and_print(&d1, &d2, &(uint32_t){0}, PRINT_BIN_DEC_SERVICE);
  d1.bits[0] = 0xFFFFFFFF;
  d1.bits[1] = 0xFFFFFFFF;
  d1.bits[2] = 0xFFFFFFFF;
  d1.bits[3] = 0x00000000;
  d1.bits[0] = 0;
  d1.bits[1] = 0;
  d1.bits[2] = 0;
  d1.bits[3] = 0x00000000;
  // s21_mul_intfield(
  //     (uint32_t[4]){0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000},
  //     (uint32_t[4]){0x0000000A, 0x00000000, 0x00000000, 0x00000000}, d1.bits,
  //     4);
  s21_print_hex_bin(d1.bits, 4, 0, PRINT_ALL);
  printf("\n");
  d1.bits[0] = 0;
  d1.bits[1] = 0;
  d1.bits[2] = 0;
  d1.bits[3] = 0x00000000;
  s21_div_intfield(
      (uint32_t[4]){0xFFFFFFF6, 0xFFFFFFFF, 0xFFFFFFFE, 0x00000009},
      (uint32_t[4]){0x0000000A, 0x00000000, 0x00000000, 0x00000000}, d1.bits,
      NULL, 4);
  s21_print_hex_bin(d1.bits, 4, 0, PRINT_ALL);
  printf("\n");
  s21_div_and_print(&(s21_decimal){{111111111, 0, 0, 0xD0000}},
                    &(s21_decimal){{111111111, 0, 0, 0xD0000}},
                    PRINT_DEC_SERVICE);
  s21_div_and_print(&(s21_decimal){{3, 0, 0, 0x00000}},
                    &(s21_decimal){{7, 0, 0, 0x00000}}, PRINT_DEC_SERVICE);
  s21_div_and_print(&(s21_decimal){{1, 0, 0, 0x10000}},
                    &(s21_decimal){{11, 0, 0, 0x40000}}, PRINT_DEC_SERVICE);
  s21_div_and_print(&(s21_decimal){{5, 0, 0, 0x10000}},
                    &(s21_decimal){{14, 0, 0, 0x10000}}, PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{~0, ~0, ~0, 0x00000}},
                    &(s21_decimal){{5, 0, 0, 0x10000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{5, 0, 0, 0x10000}},
                    &(s21_decimal){{~0, ~0, ~0, 0x00000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{1337, 0, 0, 0x80050000}},
                    &(s21_decimal){{1488, 0, 0, 0x80040000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  s21_div_and_print(&(s21_decimal){{1337, 0, 0, 0x80050000}},
                    &(s21_decimal){{1488, 0, 0, 0x80040000}},
                    PRINT_DEC_SERVICE);
  s21_div_and_print(&(s21_decimal){{0xb71571c7, 0x3f28c, 0, 0x80050000}},
                    &(s21_decimal){{0xb71571c7, 0x3f28c, 0, 0x00050000}},
                    PRINT_DEC_SERVICE);
  s21_mul_and_print(&(s21_decimal){{0xb71571c7, 0x3f28c, 0, 0x80000000}},
                    &(s21_decimal){{0xb71571c7, 0x3f28c, 0, 0x00000000}},
                    &(uint32_t){0}, PRINT_DEC_SERVICE);
  s21_div_and_print(&(s21_decimal){{23976, 0, 0, 0x60000}},
                    &(s21_decimal){{270, 0, 0, 0x20000}}, PRINT_DEC_SERVICE);
  // s21_div_and_print(&(s21_decimal){{111111111, 0, 0, 0xD0000}},
  //                   &(s21_decimal){{111111110, 0, 0, 0xD0000}},
  //                   PRINT_DEC_SERVICE);
  // s21_div_and_print(&(s21_decimal){{111111111, 0, 0, 0xD0000}},
  //                   &(s21_decimal){{111111112, 0, 0, 0xD0000}},
  //                   PRINT_DEC_SERVICE);
  s21_add_and_print(&(s21_decimal){{1000000009, 0, 0, 0xD0000}},
                    &(s21_decimal){{999999991, 0, 0, 0xD0000}}, &(uint32_t){0},
                    PRINT_DEC_SERVICE);
  // 11100011100011101010100011101101001100010100111111
  // 3817777389 50495
  // 4238552517 94663
  // 11111100101000110010110111000101010111000111000111
  // s21_mul_and_print(&d2, &d1, &(uint32_t){0}, PRINT_DEC_SERVICE); 23976/270
  // for (uint32_t i = 0; i < 29; ++i) {
  //   s21_print_hex_bin(powers_of_ten[i], 3, 0, PRINT_ALL ^ PRINT_HEX);
  //   printf("\n");
  // }
  //
}
