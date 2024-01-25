#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

START_TEST(s21_add_test) {}
END_TEST

START_TEST(s21_sub_test) {}
END_TEST

START_TEST(s21_mul_test) {}
END_TEST

START_TEST(s21_div_test) {}
END_TEST

START_TEST(s21_is_less_test) {}
END_TEST

START_TEST(s21_is_less_or_equal_test) {}
END_TEST

START_TEST(s21_is_greater_test) {}
END_TEST

START_TEST(s21_is_greater_or_equal_test) {}
END_TEST

START_TEST(s21_is_equal_test) {}
END_TEST

START_TEST(s21_is_not_equal_test) {}
END_TEST

START_TEST(s21_from_int_to_decimal_test) {}
END_TEST

START_TEST(s21_from_float_to_decimal_test) {}
END_TEST

START_TEST(s21_from_decimal_to_int_test) {}
END_TEST

START_TEST(s21_from_decimal_to_float_test) {}
END_TEST

START_TEST(s21_floor_test) {}
END_TEST

START_TEST(s21_round_test) {}
END_TEST

START_TEST(s21_truncate_test) {}
END_TEST

START_TEST(s21_negate_test) {}
END_TEST

int main(void) {
  Suite *suite = suite_create("core");
  TCase *tc = tcase_create("core");
  SRunner *sr = srunner_create(suite);

  suite_add_tcase(suite, tc);

  tcase_add_test(s21_add_test);
  tcase_add_test(s21_sub_test);
  tcase_add_test(s21_mul_test);
  tcase_add_test(s21_div_test);
  tcase_add_test(s21_is_less_test);
  tcase_add_test(s21_is_less_or_equal_test);
  tcase_add_test(s21_is_greater_test);
  tcase_add_test(s21_is_greater_or_equal_test);
  tcase_add_test(s21_is_equal_test);
  tcase_add_test(s21_is_not_equal_test);
  tcase_add_test(s21_from_int_to_decimal_test);
  tcase_add_test(s21_from_float_to_decimal_test);
  tcase_add_test(s21_from_decimal_to_int_test);
  tcase_add_test(s21_from_decimal_to_float_test);
  tcase_add_test(s21_floor_test);
  tcase_add_test(s21_round_test);
  tcase_add_test(s21_truncate_test);
  tcase_add_test(s21_negate_test);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  srunner_free(sr);

  return 0;
}
