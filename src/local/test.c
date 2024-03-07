#include <stdint.h>
#include <stdio.h>

#include "../s21_decimal.h"

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

// void s21_memprint_bits(const void *data, uint32_t bits) {
// uint64_t i = bits - 1;
// while (i < bits) {
// uint64_t bit = (*((uint64_t *)data) >> i) & 1;
// printf("%s%llu\x1B[0m", bit ? "\x1B[32m" : "\x1B[31m", bit);
// if (i == 0) break;
// --i;
// }
// }

void s21_print_bits(uint32_t data[], const uint32_t from,
                    const uint32_t amount) {
  uint32_t i = from + amount - 1;
  while (i < from + amount) {
    uint32_t bit = __s21_read_bit(data, i);
    printf("%s%u\x1B[0m", bit ? "\x1B[32m" : "\x1B[31m", bit);
    if (i == from) break;
    --i;
  }
}

void s21_read_bits_and_print(s21_decimal value, const uint32_t from,
                             const uint32_t amount, const uint8_t newlines) {
  uint32_t width = len_of_int(S21_DECIMAL_SIZE_IN_BITS);
  printf("Bits %*u to %*u: ", width, from, width, from + amount - 1);
  s21_print_bits(value.uint_data, from, amount);
  if (newlines) printf("\n");
}

void s21_print_hex_bin(const s21_decimal value) {
  for (int i = 0; i < S21_DECIMAL_SIZE_IN_INTS; ++i) {
    s21_read_bits_and_print(value, i * 32, 32, 0);
    printf(" 0x%08X %-10u%c", value.uint_data[i], value.uint_data[i],
           i < S21_DECIMAL_SIZE_IN_INTS - 1 ? '\n' : ' ');
  }
  // Print sign and exponent
  printf(" * %c1e-%u\n", __s21_is_decimal_negative(value) ? '-' : '+',
         __s21_get_exponent(value));
}
void s21_set_and_print_bit(s21_decimal *const value, const uint32_t bit) {
  printf("\n\nSetting bit %u\n", bit);
  __s21_write_bits(value->uint_data, 1, bit, 1);
  s21_print_hex_bin(*value);
}
void s21_clear_and_print_bit(s21_decimal *const value, const uint32_t bit) {
  printf("\n\nClearing bit %u\n", bit);
  __s21_write_bits(value->uint_data, 1, bit, 1);
  s21_print_hex_bin(*value);
}
// void s21_toggle_and_print_bit(s21_decimal *const value,
// const uint32_t bit) {
// printf("\n\nToggling bit %u\n", bit);
// if (!__s21_toggle_bit(value->uint_data, bit))
// s21_print_hex_bin(*value);
// else
// printf("Failed to toggle bit %u", bit);
// }
void s21_write_and_print_exponent(s21_decimal *const value,
                                  const uint32_t exp) {
  printf("\n\nWriting exponent %u\n", exp);
  s21_print_hex_bin(*value);
}
void s21_read_and_print_exponent(const s21_decimal *const value) {
  printf("Exponent: %u\n", __s21_get_exponent(*value));
}

void s21_shift_left_and_print(s21_decimal *value, const uint32_t amount) {
  printf("\n\nShifting left by %u\n", amount);
  __s21_left_shift_intfield(value->uint_data, amount, value->uint_data, 3);
  s21_print_hex_bin(*value);
}
void s21_shift_right_and_print(s21_decimal *value, const uint32_t amount) {
  printf("Shifting right by %u\n", amount);
  __s21_right_shift_intfield(value->uint_data, amount, value->uint_data, 3);
  s21_print_hex_bin(*value);
}
void s21_add_and_print(s21_decimal *value, const s21_decimal *other,
                       uint32_t *temp) {
  s21_decimal tarray = {0};
  printf("\n\nAdding\n");
  s21_print_hex_bin(*value);
  printf("+\n");
  s21_print_hex_bin(*other);
  printf("=\n");
  *temp = s21_add(*value, *other, &tarray);
  *value = tarray;
  s21_print_hex_bin(*value);

  if (*temp == TOO_BIG)
    printf("Too big!\n");
  else if (*temp == OK) {
  } else
    printf("Unknown\n");
}

void s21_mul_and_print(s21_decimal *value, const s21_decimal *other,
                       uint32_t *temp) {
  printf("\n\nMultiplying\n");
  s21_print_hex_bin(*value);
  printf("*\n");
  s21_print_hex_bin(*other);
  printf("=\n");
  *temp = s21_mul(*value, *other, value);
  s21_print_hex_bin(*value);

  if (*temp == OK) {
  } else if (*temp == TOO_BIG)
    printf("Too big!\n");
  else if (*temp == TOO_SMALL)
    printf("Too small!\n");
  else
    printf("Unknown\n");
}

void s21_write_bits_and_print(s21_decimal *container, const uint32_t from,
                              const uint32_t amount, uint32_t data) {
  printf("\n\nWriting ");
  s21_print_bits(&data, 0, 32);
  printf(" to bits %u:%u\n", from, from + amount - 1);
  __s21_write_bits(container->uint_data, data, from, amount);
  s21_print_hex_bin(*container);
}
void s21_compare_and_print(const s21_decimal *value, const s21_decimal *other) {
  printf("\n\nComparing\n");
  s21_print_hex_bin(*value);
  printf("VS\n");
  s21_print_hex_bin(*other);
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
int main() {
  s21_decimal dec, ced, ded;

  s21_memset(dec.uint_data, 0, sizeof(dec.uint_data));
  s21_memset(ced.uint_data, 0, sizeof(ced.uint_data));
  s21_memset(ded.uint_data, 0, sizeof(ded.uint_data));
  for (uint32_t i = 0; i < S21_DECIMAL_SIZE_IN_BITS; ++i) {
    uint32_t temp = 0;
    printf("%u\n", temp);
    s21_print_hex_bin(dec);
  }
  for (uint32_t i = 0; i < S21_DECIMAL_SIZE_IN_BITS; ++i) {
    uint32_t temp = 0;
    printf("%u\n", temp);
    s21_print_hex_bin(dec);
  }
  printf("Size of s21_decimal: %lu\n", sizeof(dec));
  printf("Size of s21_decimal.uint_data: %lu\n", sizeof(dec.uint_data));
  for (uint32_t i = 0; i < sizeof(dec) / sizeof(dec.uint_data[0]); ++i)
    printf("Size of s21_decimal.uint_data[%d]: %lu, number inside is %u\n", i,
           sizeof(dec.uint_data[i]), dec.uint_data[i]);
  s21_set_and_print_bit(&dec, 0);
  uint32_t temp = 0;
  while (!temp) {
    ced = dec;
    s21_add_and_print(&dec, &ced, &temp);
  }
  s21_write_bits_and_print(&dec, 0, 32, 1 << 31 | 1);
  s21_write_and_print_exponent(&dec, 13);
  s21_read_and_print_exponent(&dec);
  s21_shift_left_and_print(&dec, 10);
  s21_shift_right_and_print(&dec, 10);
  s21_write_bits_and_print(&dec, 16, 32, 1 << 31 | 1);
  s21_shift_left_and_print(&dec, 10);
  s21_shift_right_and_print(&dec, 10);
  s21_read_bits_and_print(dec, 0, 128, 1);
  s21_write_bits_and_print(&dec, 32, 32, 1 << 31 | 1);
  s21_shift_right_and_print(&dec, 10);
  s21_write_bits_and_print(&dec, 55, 30, 536870913);
  s21_write_bits_and_print(&dec, 54, 1, 1);
  s21_write_bits_and_print(&dec, 51, 1, 1);
  s21_write_bits_and_print(&dec, 65, 6, 42);
  s21_write_bits_and_print(&dec, 52, 1, 1);
  s21_write_bits_and_print(&dec, 63, 2, 3);
  s21_shift_left_and_print(&dec, 20);
  s21_write_bits_and_print(&dec, 96, 7, 89655232);
  s21_read_bits_and_print(dec, 65, 32, 1);
  s21_add_and_print(&dec, &ced, &(uint32_t){0});
  s21_add_and_print(&dec, &ced, &(uint32_t){0});
  s21_print_hex_bin(dec);
  s21_write_bits_and_print(&dec, 43, 8, 150);
  s21_shift_left_and_print(&dec, 50);
  s21_decimal dummy;
  s21_memset(dummy.uint_data, 0, sizeof(dummy.uint_data));
  s21_print_hex_bin(dummy);
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] = 0x80000000;
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 2] = 1488;
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 3] = 911;
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 4] = 10;
  s21_add_and_print(&dec, &dummy, &(uint32_t){0});
  s21_shift_right_and_print(&dec, 50);
  s21_mul_and_print(&dec, &(s21_decimal){{10, 0, 0, 0x80000000}},
                    &(uint32_t){0});
  s21_add_and_print(&dec, &(s21_decimal){{0, 0, 69420, 0}}, &(uint32_t){0});
  s21_mul_and_print(&dec, &(s21_decimal){{50, 0, 0, 0x80000000}},
                    &(uint32_t){0});
  s21_mul_and_print(&dec, &(s21_decimal){{10, 0, 0, 0x80000000}},
                    &(uint32_t){0});
  s21_mul_and_print(&(s21_decimal){{876, 0, 0, 0x80000000}},
                    &(s21_decimal){{1874919424, 2328306, 0, 0x80000000}},
                    &(uint32_t){0});
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 1] = 0x80000000;
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 2] = 1488;
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 3] = 911;
  dummy.uint_data[S21_DECIMAL_SIZE_IN_INTS - 4] = 10;
  s21_mul_and_print(&dummy, &(s21_decimal){{10, 0, 0, 0x800D0000}},
                    &(uint32_t){0});
  s21_mul_and_print(&(s21_decimal){{111111111, 0, 0, 0x800D0000}},
                    &(s21_decimal){{111111111, 0, 0, 0x800D0000}},
                    &(uint32_t){0});
  s21_mul_and_print(&(s21_decimal){{111111111, 0, 0, 0xD0000}},
                    &(s21_decimal){{111111111, 0, 0, 0xD0000}}, &(uint32_t){0});
  s21_mul_and_print(&(s21_decimal){{4294967295, 0, 0, 0xD0000}},
                    &(s21_decimal){{4294967295, 0, 0, 0xD0000}},
                    &(uint32_t){0});
  s21_mul_and_print(&(s21_decimal){{4095, 0, 0, 0x10000}},
                    &(s21_decimal){{4095, 0, 0, 0xD0000}}, &(uint32_t){0});
  printf("\n");
  printf("uint32_t powers_of_ten[29][6] = {\n");

  for (uint32_t exp = 0; exp < 29; ++exp) {
    uint32_t i;
    i = 5;
    printf("{");
    do {
      printf("%10u, ", powers_of_ten[exp][i]);
    } while (i--);
    printf("},\n");
  }
  printf("};\n");
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0}},
                    &(s21_decimal){{69420, 0, 0, 0x80000000}}, &(uint32_t){0});
  s21_shift_left_and_print(&(s21_decimal){{1490, 0, 0, 4 | (10 << 27)}}, 47);
  s21_shift_right_and_print(&(s21_decimal){{0, 48824320, 0, 4 | (10 << 27)}},
                            47);
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0x20000}},
                    &(s21_decimal){{69420, 0, 0, 0x80000000}}, &(uint32_t){0});
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0x20000}},
                    &(s21_decimal){{69420, 0, 0, 0x80010000}}, &(uint32_t){0});
  s21_add_and_print(&(s21_decimal){{67932, 0, 0, 0x20000}},
                    &(s21_decimal){{69420, 0, 0, 0x80040000}}, &(uint32_t){0});
  s21_compare_and_print(&(s21_decimal){{69420, 0, 0, 0x80050000}},
                        &(s21_decimal){{6942, 0, 0, 0x80040000}});
  s21_compare_and_print(&(s21_decimal){{1488, 0, 0, 0x30000}},
                        &(s21_decimal){{1488001, 0, 0, 0x60000}});
  s21_compare_and_print(&(s21_decimal){{1489, 0, 0, 0x30000}},
                        &(s21_decimal){{1488999, 0, 0, 0x60000}});
  s21_compare_and_print(&(s21_decimal){{1488, 0, 0, 0x30000}},
                        &(s21_decimal){{1488000, 0, 0, 0x60000}});
  // 1111111111111111111111111111111000000000000000000000000000000001
  // 1111111111111111111111111111111000000000000000000000000000000001
  //  CLEAR_AND_PRINT_BIT(dec, 127);536 870 913
  //  printf("\n");
  //  if (s21_toggle_bit(&dec, -1)) printf("%X %X\n", s21_toggle_bit(&dec,
  //  -1), -1); printf("%d\n", ~-1);
}
