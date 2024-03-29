#include <check.h>
#include <stdio.h>

#include "s21_decimal.h"

// ADD
START_TEST(add_test1) {
  // 79228162514264337593543950335 + 1 = err 1
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal d2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_add(d1, d2, &res) == 1, "add_test1 FAIL");
}
END_TEST

START_TEST(add_test2) {
  // -79228162514264337593543950335 + -1 = err 2
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal d2 = {{0x00000001, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_add(d1, d2, &res) == 2, "add_test2 FAIL");
}
END_TEST

START_TEST(add_test3) {
  // 7 + 2 = 9
  s21_decimal d1 = {{0x00000007, 0, 0, 0}};
  s21_decimal d2 = {{0x00000002, 0, 0, 0}};
  s21_decimal res;
  ck_assert_msg(s21_add(d1, d2, &res) == 0, "add_test3 FAIL");
  ck_assert_msg(res.bits[0] == 9, "add_test3 FAIL");
  ck_assert_msg(res.bits[1] == 0, "add_test3 FAIL");
  ck_assert_msg(res.bits[2] == 0, "add_test3 FAIL");
  ck_assert_msg(res.bits[3] == 0, "add_test3 FAIL");
}
END_TEST

START_TEST(add_test4) {
  // 10 + 1.0 = 11
  s21_decimal d1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x0000000A, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal res1;
  s21_decimal res2;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test4.1 FAIL");
  ck_assert_msg(res1.bits[0] == 11, "add_test4.1 FAIL");
  ck_assert_msg(res1.bits[1] == 0, "add_test4.1 FAIL");
  ck_assert_msg(res1.bits[2] == 0, "add_test4.1 FAIL");
  ck_assert_msg(res1.bits[3] == 0, "add_test4.1 FAIL");

  // 1.0 + 10 = 11
  ck_assert_msg(s21_add(d2, d1, &res2) == 0, "add_test4.2 FAIL");
  ck_assert_msg(res1.bits[0] == res2.bits[0], "add_test4.2 FAIL");
  ck_assert_msg(res1.bits[1] == res2.bits[1], "add_test4.2 FAIL");
  ck_assert_msg(res1.bits[2] == res2.bits[2], "add_test4.2 FAIL");
  ck_assert_msg(res1.bits[3] == res2.bits[3], "add_test4.2 FAIL");
}
END_TEST

START_TEST(add_test5) {
  s21_decimal d1 = {{0x00000009, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res1, res2;

  // 0.9 + 1 = 1.9
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test5.1 FAIL");
  ck_assert_msg(res1.bits[0] == 19, "add_test5.1 FAIL");
  ck_assert_msg(res1.bits[1] == 0, "add_test5.1 FAIL");
  ck_assert_msg(res1.bits[2] == 0, "add_test5.1 FAIL");
  ck_assert_msg(res1.bits[3] == 0x00010000, "add_test5.1 FAIL");

  // 1 + 0.9 = 1.9
  ck_assert_msg(s21_add(d2, d1, &res2) == 0, "add_test5.2 FAIL");
  ck_assert_msg(res2.bits[0] == 19, "add_test5.2 FAIL");
  ck_assert_msg(res2.bits[1] == 0, "add_test5.2 FAIL");
  ck_assert_msg(res2.bits[2] == 0, "add_test5.2 FAIL");
  ck_assert_msg(res2.bits[3] == 0x00010000, "add_test5.2 FAIL");
}
END_TEST

START_TEST(add_test6) {
  // 33.33333333333 + 0.000000000001 = 33.333333333331
  s21_decimal d1 = {{0x1a263555, 0x00000308, 0x00000000, 0x000b0000}};
  s21_decimal d2 = {{0x00000001, 0x00000000, 0x00000000, 0x000c0000}};
  s21_decimal res;
  ck_assert_msg(s21_add(d1, d2, &res) == 0, "add_test6 FAIL");
  ck_assert_msg(res.bits[0] == 0x057e1553, "add_test6 FAIL");
  ck_assert_msg(res.bits[1] == 0x00001e51, "add_test6 FAIL");
  ck_assert_msg(res.bits[2] == 0, "add_test6 FAIL");
  ck_assert_msg(res.bits[3] == 0x000c0000, "add_test6 FAIL");
}
END_TEST

START_TEST(add_test7) {
  // 33.333333333333333333333333333 + 0.0000000000000000000000000001
  // =  33.333333333333333333333333333
  s21_decimal d1 = {{0x35555555, 0xcf2607ee, 0x6bb4afe4, 0x001b0000}};
  s21_decimal d2 = {{0x00000001, 0x00000000, 0x00000000, 0x001c0000}};
  s21_decimal res;
  ck_assert_msg(s21_add(d1, d2, &res) == 0, "add_test7 FAIL");
  ck_assert_msg(res.bits[0] == 0x35555555, "add_test7 FAIL");
  ck_assert_msg(res.bits[1] == 0xcf2607ee, "add_test7 FAIL");
  ck_assert_msg(res.bits[2] == 0x6bb4afe4, "add_test7 FAIL");
  ck_assert_msg(res.bits[3] == 0x001b0000, "add_test7 FAIL");
}
END_TEST

START_TEST(add_test8) {
  // 10 + -10 = 0
  s21_decimal d1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x0000000A, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test8 FAIL");
  ck_assert_msg(res1.bits[0] == 0, "add_test8 FAIL");
  ck_assert_msg(res1.bits[1] == 0, "add_test8 FAIL");
  ck_assert_msg(res1.bits[2] == 0, "add_test8 FAIL");
  ck_assert_msg(res1.bits[3] == 0, "add_test8 FAIL");
}
END_TEST

START_TEST(add_test9) {
  // 10 + -79228162514264337593543950335 = -79228162514264337593543950325
  s21_decimal d1 = {{0x0000000A, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test9 FAIL");
  ck_assert_msg(res1.bits[0] == 0xfffffff5, "add_test9 FAIL");
  ck_assert_msg(res1.bits[1] == 0xffffffff, "add_test9 FAIL");
  ck_assert_msg(res1.bits[2] == 0xffffffff, "add_test9 FAIL");
  ck_assert_msg(res1.bits[3] == 0x80000000, "add_test9 FAIL");
}
END_TEST

START_TEST(add_test10) {
  // 0.0000000018446744073709551615 + -0.0000000018446744073709551615
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0x00000000, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0x00000000, 0x801c0000}};
  s21_decimal res1 = {0};

  for (int i = 0; i < 100; ++i) {
    ck_assert_msg(s21_add(res1, d1, &res1) == 0, "add_test10.1 FAIL");
  }

  for (int i = 0; i < 99; ++i) {
    ck_assert_msg(s21_add(res1, d2, &res1) == 0, "add_test10.2 FAIL");
  }

  ck_assert_msg(res1.bits[0] == 0xffffffff, "add_test10.3 FAIL");
  ck_assert_msg(res1.bits[1] == 0xffffffff, "add_test10.3 FAIL");
  ck_assert_msg(res1.bits[2] == 0x00000000, "add_test10.3 FAIL");
  ck_assert_msg(res1.bits[3] == 0x001c0000, "add_test10.3 FAIL");

  ck_assert_msg(s21_add(res1, d2, &res1) == 0, "add_test10.4 FAIL");
  ck_assert_msg(res1.bits[0] == 0x00000000, "add_test10.4 FAIL");
  ck_assert_msg(res1.bits[1] == 0x00000000, "add_test10.4 FAIL");
  ck_assert_msg(res1.bits[2] == 0x00000000, "add_test10.4 FAIL");
  ck_assert_msg(res1.bits[3] == 0x00000000, "add_test10.4 FAIL");

  ck_assert_msg(s21_add(res1, d2, &res1) == 0, "add_test10.5 FAIL");
  ck_assert_msg(res1.bits[0] == 0xffffffff, "add_test10.5 FAIL");
  ck_assert_msg(res1.bits[1] == 0xffffffff, "add_test10.5 FAIL");
  ck_assert_msg(res1.bits[2] == 0x00000000, "add_test10.5 FAIL");
  ck_assert_msg(res1.bits[3] == 0x801c0000, "add_test10.5 FAIL");
}
END_TEST

START_TEST(add_test11) {
  // 7.9228162514264337593543950335 + 7.9228162514264337593543950335
  // = 15.845632502852867518708790067
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x001c0000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test11 FAIL");
  ck_assert_msg(res1.bits[0] == 0x33333333, "add_test11 FAIL");
  ck_assert_msg(res1.bits[1] == 0x33333333, "add_test11 FAIL");
  ck_assert_msg(res1.bits[2] == 0x33333333, "add_test11 FAIL");
  ck_assert_msg(res1.bits[3] == 0x001b0000, "add_test11 FAIL");
}
END_TEST

START_TEST(add_test12) {
  // -7.9228162514264337593543950335 + -7.9228162514264337593543950335
  // = -15.845632502852867518708790067
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x801c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x801c0000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test12 FAIL");
  ck_assert_msg(res1.bits[0] == 0x33333333, "add_test12 FAIL");
  ck_assert_msg(res1.bits[1] == 0x33333333, "add_test12 FAIL");
  ck_assert_msg(res1.bits[2] == 0x33333333, "add_test12 FAIL");
  ck_assert_msg(res1.bits[3] == 0x801b0000, "add_test12 FAIL");
}
END_TEST

START_TEST(add_test13) {
  // 0.0000000000000005 + -0.00000000000000075 = -0.00000000000000025
  s21_decimal d1 = {{0x00000005, 0x00000000, 0x00000000, 0x00100000}};
  s21_decimal d2 = {{0x0000004b, 0x00000000, 0x00000000, 0x80110000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test13 FAIL");
  ck_assert_msg(res1.bits[0] == 0x00000019, "add_test13 FAIL");
  ck_assert_msg(res1.bits[1] == 0x00000000, "add_test13 FAIL");
  ck_assert_msg(res1.bits[2] == 0x00000000, "add_test13 FAIL");
  ck_assert_msg(res1.bits[3] == 0x80110000, "add_test13 FAIL");
}
END_TEST

START_TEST(add_test14) {
  // -0.0000000000000005 + 0.00000000000000075 = 0.00000000000000025
  s21_decimal d1 = {{0x00000005, 0x00000000, 0x00000000, 0x80100000}};
  s21_decimal d2 = {{0x0000004b, 0x00000000, 0x00000000, 0x00110000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test14 FAIL");
  ck_assert_msg(res1.bits[0] == 0x00000019, "add_test14 FAIL");
  ck_assert_msg(res1.bits[1] == 0x00000000, "add_test14 FAIL");
  ck_assert_msg(res1.bits[2] == 0x00000000, "add_test14 FAIL");
  ck_assert_msg(res1.bits[3] == 0x00110000, "add_test14 FAIL");
}
END_TEST

START_TEST(add_test15) {
  // 0.9999999884841547537349345279 + 74276402357122816493947453439 =
  // 74276402357122816493947453440
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0x204fce57, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xefffffff, 0x00000000}};
  s21_decimal res1;
  ck_assert_msg(s21_add(d1, d2, &res1) == 0, "add_test14 FAIL");
  ck_assert_msg(res1.bits[0] == 0x00000000, "add_test14 FAIL");
  ck_assert_msg(res1.bits[1] == 0x00000000, "add_test14 FAIL");
  ck_assert_msg(res1.bits[2] == 0xf0000000, "add_test14 FAIL");
  ck_assert_msg(res1.bits[3] == 0x00000000, "add_test14 FAIL");
}
END_TEST

// SUB
START_TEST(sub_test1) {
  // 0.0 - 0 = 0
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test1 FAIL");
  ck_assert_msg(res.bits[0] == 0, "sub_test1 FAIL");
  ck_assert_msg(res.bits[1] == 0, "sub_test1 FAIL");
  ck_assert_msg(res.bits[2] == 0, "sub_test1 FAIL");
  ck_assert_msg(res.bits[3] == 0, "sub_test1 FAIL");
}
END_TEST

START_TEST(sub_test2) {
  // 0.000000000000000000000000002 + 0.0000000000000000000000000010 =
  // 0.000000000000000000000000001
  s21_decimal d1 = {{0x00000002, 0x00000000, 0x00000000, 0x001b0000}};
  s21_decimal d2 = {{0x0000000a, 0x00000000, 0x00000000, 0x001c0000}};
  s21_decimal res;

  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test2 FAIL");
  ck_assert_msg(res.bits[0] == 1, "sub_test2 FAIL");
  ck_assert_msg(res.bits[1] == 0, "sub_test2 FAIL");
  ck_assert_msg(res.bits[2] == 0, "sub_test2 FAIL");
  ck_assert_msg(res.bits[3] == 0x001b0000, "sub_test2 FAIL");
}
END_TEST

START_TEST(sub_test3) {
  // 79228162514264337593543950335 - 7922816251426433759354395033 =  0
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test3 FAIL");
  ck_assert_msg(res.bits[0] == 0, "sub_test3 FAIL");
  ck_assert_msg(res.bits[1] == 0, "sub_test3 FAIL");
  ck_assert_msg(res.bits[2] == 0, "sub_test3 FAIL");
  ck_assert_msg(res.bits[3] == 0, "sub_test3 FAIL");
}
END_TEST

START_TEST(sub_test4) {
  // -79228162514264337593543950335 - -7922816251426433759354395033 = -0
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test4 FAIL");
  ck_assert_msg(res.bits[0] == 0, "sub_test4 FAIL");
  ck_assert_msg(res.bits[1] == 0, "sub_test4 FAIL");
  ck_assert_msg(res.bits[2] == 0, "sub_test4 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "sub_test4 FAIL");
}
END_TEST

START_TEST(sub_test5) {
  // -79228162514264337593543950335 - 79228162514264337593543950335 = err2
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 2, "sub_test5 FAIL");
}
END_TEST

START_TEST(sub_test6) {
  // 79228162514264337593543950335 - -7922816251426433759354395033 =  err1
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 1, "sub_test6 FAIL");
}
END_TEST

START_TEST(sub_test7) {
  // -2 - 2 = -4
  s21_decimal d1 = {{0x00000002, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test7 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000004, "sub_test7 FAIL");
  ck_assert_msg(res.bits[1] == 0, "sub_test7 FAIL");
  ck_assert_msg(res.bits[2] == 0, "sub_test7 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "sub_test7 FAIL");
}
END_TEST

START_TEST(sub_test8) {
  // -3.4 - 2 = -5.4
  s21_decimal d1 = {{0x00000022, 0x00000000, 0x00000000, 0x80010000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test8 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000036, "sub_test8 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "sub_test8 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test8 FAIL");
  ck_assert_msg(res.bits[3] == 0x80010000, "sub_test8 FAIL");
}
END_TEST

START_TEST(sub_test9) {
  // -3.4 - -2 = -1.4
  s21_decimal d1 = {{0x00000022, 0x00000000, 0x00000000, 0x80010000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test9 FAIL");
  ck_assert_msg(res.bits[0] == 0x0000000e, "sub_test9 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "sub_test9 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test9 FAIL");
  ck_assert_msg(res.bits[3] == 0x80010000, "sub_test9 FAIL");
}
END_TEST

START_TEST(sub_test10) {
  // 3.4 - -2 = 5.4
  s21_decimal d1 = {{0x00000022, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test10 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000036, "sub_test10 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "sub_test10 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test10 FAIL");
  ck_assert_msg(res.bits[3] == 0x00010000, "sub_test10 FAIL");
}
END_TEST

START_TEST(sub_test11) {
  // 3.4 - 2 = 1.4
  s21_decimal d1 = {{0x00000022, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test11 FAIL");
  ck_assert_msg(res.bits[0] == 0x0000000e, "sub_test11 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "sub_test11 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test11 FAIL");
  ck_assert_msg(res.bits[3] == 0x00010000, "sub_test11 FAIL");
}
END_TEST

START_TEST(sub_test12) {
  // 0 - -0 = 0
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test12 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "sub_test12 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "sub_test12 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test12 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "sub_test12 FAIL");
}
END_TEST

START_TEST(sub_test13) {
  // 8589934534.10 - 532353.15214412 = 8589402180.94785588
  s21_decimal d1 = {{0xffffe962, 0x000000c7, 0x00000000, 0x00020000}};
  s21_decimal d2 = {{0xd00d884c, 0x0000306a, 0x00000000, 0x00080000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test13 FAIL");
  ck_assert_msg(res.bits[0] == 0xd6d61434, "sub_test13 FAIL");
  ck_assert_msg(res.bits[1] == 0x0beb9193, "sub_test13 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test13 FAIL");
  ck_assert_msg(res.bits[3] == 0x00080000, "sub_test13 FAIL");
}
END_TEST

START_TEST(sub_test14) {
  // -8589934534.10 - -532353.15214412 = -8589402180.94785588
  s21_decimal d1 = {{0xffffe962, 0x000000c7, 0x00000000, 0x80020000}};
  s21_decimal d2 = {{0xd00d884c, 0x0000306a, 0x00000000, 0x80080000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test14 FAIL");
  ck_assert_msg(res.bits[0] == 0xd6d61434, "sub_test14 FAIL");
  ck_assert_msg(res.bits[1] == 0x0beb9193, "sub_test14 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test14 FAIL");
  ck_assert_msg(res.bits[3] == 0x80080000, "sub_test14 FAIL");
}
END_TEST

START_TEST(sub_test15) {
  // 8589934534.10 - -532353.15214412 = 8590466887.25214412
  s21_decimal d1 = {{0xffffe962, 0x000000c7, 0x00000000, 0x00020000}};
  s21_decimal d2 = {{0xd00d884c, 0x0000306a, 0x00000000, 0x80080000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test15 FAIL");
  ck_assert_msg(res.bits[0] == 0x76f124cc, "sub_test15 FAIL");
  ck_assert_msg(res.bits[1] == 0x0bebf269, "sub_test15 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test15 FAIL");
  ck_assert_msg(res.bits[3] == 0x00080000, "sub_test15 FAIL");
}
END_TEST

START_TEST(sub_test16) {
  // -8589934534.10 - 532353.15214412 = -8590466887.25214412
  s21_decimal d1 = {{0xffffe962, 0x000000c7, 0x00000000, 0x80020000}};
  s21_decimal d2 = {{0xd00d884c, 0x0000306a, 0x00000000, 0x00080000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test16 FAIL");
  ck_assert_msg(res.bits[0] == 0x76f124cc, "sub_test16 FAIL");
  ck_assert_msg(res.bits[1] == 0x0bebf269, "sub_test16 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "sub_test16 FAIL");
  ck_assert_msg(res.bits[3] == 0x80080000, "sub_test16 FAIL");
}
END_TEST

START_TEST(sub_test17) {
  // 79228162514264337593543950335 - 0.6 = 79228162514264337593543950334
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal d2 = {{0x00000006, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal res;
  ck_assert_msg(s21_sub(d1, d2, &res) == 0, "sub_test17 FAIL");
  ck_assert_msg(res.bits[0] == 0xfffffffe, "sub_test17 FAIL");
  ck_assert_msg(res.bits[1] == 0xffffffff, "sub_test17 FAIL");
  ck_assert_msg(res.bits[2] == 0xffffffff, "sub_test17 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "sub_test17 FAIL");
}
END_TEST

// MUL
START_TEST(mul_test1) {
  // 0.1 * 2 = 0.2
  s21_decimal d1 = {{0x00000001, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_mul(d1, d2, &res) == 0, "mul_test1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000002, "mul_test1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "mul_test1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "mul_test1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00010000, "mul_test1 FAIL");
}
END_TEST

START_TEST(mul_test2) {
  // -4294967295 * 2 = -8589934590
  s21_decimal d1 = {{0xffffffff, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_mul(d1, d2, &res) == 0, "mul_test2 FAIL");
  ck_assert_msg(res.bits[0] == 0xfffffffe, "mul_test2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000001, "mul_test2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "mul_test2 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "mul_test2 FAIL");
}
END_TEST

START_TEST(mul_test3) {
  // -79228162495817593524129366015 * 2 = err2
  s21_decimal d1 = {{0xffffffff, 0x00000000, 0xffffffff, 0x80000000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_mul(d1, d2, &res) == 2, "mul_test3 FAIL");
}
END_TEST

START_TEST(mul_test4) {
  // 3713820117856140824697372671 * 6189700196426901374495621119 = err1
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0x0bffffff, 0x00000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0x13ffffff, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_mul(d1, d2, &res) == 1, "mul_test4 FAIL");
}
END_TEST

START_TEST(mul_test5) {
  // 0.3713820117856140824697372671 * 0.6189700196426901374495621119 =
  // 0.229874331129883328753752054
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0x0bffffff, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0x13ffffff, 0x001c0000}};
  s21_decimal res;
  ck_assert_msg(s21_mul(d1, d2, &res) == 0, "mul_test5 FAIL");
  ck_assert_msg(res.bits[0] == 0x8a28d7f6, "mul_test5 FAIL");
  ck_assert_msg(res.bits[1] == 0x75e24097, "mul_test5 FAIL");
  ck_assert_msg(res.bits[2] == 0xbe25c8, "mul_test5 FAIL");
  ck_assert_msg(res.bits[3] == 0x1b0000, "mul_test5 FAIL");
}
END_TEST

// DIV
START_TEST(div_test1) {
  // 2 / 2 = 1
  s21_decimal d1 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000001, "div_test1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "div_test1 FAIL");
}
END_TEST

START_TEST(div_test2) {
  // 3 / 1.5 = 2
  s21_decimal d1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x0000000f, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000002, "div_test2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test2 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "div_test2 FAIL");
}
END_TEST

START_TEST(div_test3) {
  // 3 / 2 = 1.5
  s21_decimal d1 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test3 FAIL");
  ck_assert_msg(res.bits[0] == 0x0000000f, "div_test3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test3 FAIL");
  ck_assert_msg(res.bits[3] == 0x00010000, "div_test3 FAIL");
}
END_TEST

START_TEST(div_test4) {
  // 100 / 3 = 33.333333333333333333333333333
  s21_decimal d1 = {{0x00000064, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test4 FAIL");
  ck_assert_msg(res.bits[0] == 0x35555555, "div_test4 FAIL");
  ck_assert_msg(res.bits[1] == 0xcf2607ee, "div_test4 FAIL");
  ck_assert_msg(res.bits[2] == 0x6bb4afe4, "div_test4 FAIL");
  ck_assert_msg(res.bits[3] == 0x001b0000, "div_test4 FAIL");
}
END_TEST

START_TEST(div_test5) {
  // 66666666666666666666666666667 / 2 = 33333333333333333333333333334
  s21_decimal d1 = {{0x6aaaaaab, 0x9e4c0fdc, 0xd7695fc9, 0x00000000}};
  s21_decimal d2 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test5 FAIL");
  ck_assert_msg(res.bits[0] == 0x35555556, "div_test5 FAIL");
  ck_assert_msg(res.bits[1] == 0xcf2607ee, "div_test5 FAIL");
  ck_assert_msg(res.bits[2] == 0x6bb4afe4, "div_test5 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "div_test5 F5AIL");
}
END_TEST

START_TEST(div_test6) {
  // 21474836480 / 3 = 7158278826.6666666666666666667
  s21_decimal d1 = {{0x00000000, 0x00000005, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test6 FAIL");
  ck_assert_msg(res.bits[0] == 0xaaaaaaab, "div_test6 FAIL");
  ck_assert_msg(res.bits[1] == 0x9082aaaa, "div_test6 FAIL");
  ck_assert_msg(res.bits[2] == 0xe74be507, "div_test6 FAIL");
  ck_assert_msg(res.bits[3] == 0x00130000, "div_test6 FAIL");
}
END_TEST

START_TEST(div_test7) {
  // -2147483648.0 / 3 = -715827882.66666666666666666667
  s21_decimal d1 = {{0x00000000, 0x00000005, 0x00000000, 0x80010000}};
  s21_decimal d2 = {{0x00000003, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test7 FAIL");
  ck_assert_msg(res.bits[0] == 0xaaaaaaab, "div_test7 FAIL");
  ck_assert_msg(res.bits[1] == 0x9082aaaa, "div_test7 FAIL");
  ck_assert_msg(res.bits[2] == 0xe74be507, "div_test7 FAIL");
  ck_assert_msg(res.bits[3] == 0x80140000, "s21_divtest7 FAIL");
}
END_TEST

START_TEST(div_test8) {
  // -2147483648.0 / -3 = 715827882.66666666666666666667
  s21_decimal d1 = {{0x00000000, 0x00000005, 0x00000000, 0x80010000}};
  s21_decimal d2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test8 FAIL");
  ck_assert_msg(res.bits[0] == 0xaaaaaaab, "div_test8 FAIL");
  ck_assert_msg(res.bits[1] == 0x9082aaaa, "div_test8 FAIL");
  ck_assert_msg(res.bits[2] == 0xe74be507, "div_test8 FAIL");
  ck_assert_msg(res.bits[3] == 0x00140000, "div_test8 FAIL");
}
END_TEST

START_TEST(div_test9) {
  // 2147483648.0 / -3 = -715827882.66666666666666666667
  s21_decimal d1 = {{0x00000000, 0x00000005, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000003, 0x00000000, 0x00000000, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test9 FAIL");
  ck_assert_msg(res.bits[0] == 0xaaaaaaab, "div_test9 FAIL");
  ck_assert_msg(res.bits[1] == 0x9082aaaa, "div_test9 FAIL");
  ck_assert_msg(res.bits[2] == 0xe74be507, "div_test9 FAIL");
  ck_assert_msg(res.bits[3] == 0x80140000, "div_test9 FAIL");
}
END_TEST

START_TEST(div_test10) {
  // 7.9228162514264337593543950335 / 79228162514264337593543950335 =
  // 0.0000000000000000000000000001
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test10 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000001, "div_test10 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test10 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test10 FAIL");
  ck_assert_msg(res.bits[3] == 0x001c0000, "div_test10 FAIL");
}
END_TEST

START_TEST(div_test11) {
  // 7.9228162514264337593543950321 / 79228162514264337593543950335 =
  // 0.0000000000000000000000000001
  s21_decimal d1 = {{0xfffffff1, 0xffffffff, 0xffffffff, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test11 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000001, "div_test11 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test11 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test11 FAIL");
  ck_assert_msg(res.bits[3] == 0x001c0000, "div_test11 FAIL");
}
END_TEST

START_TEST(div_test12) {
  // -7.9228162514264337593543950321 / 79228162514264337593543950335 =
  // -0.0000000000000000000000000001
  s21_decimal d1 = {{0xfffffff1, 0xffffffff, 0xffffffff, 0x801c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test12 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000001, "div_test12 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test12 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test12 FAIL");
  ck_assert_msg(res.bits[3] == 0x801c0000, "div_test12 FAIL");
}
END_TEST

START_TEST(div_test13) {
  // -7.9228162514264337593543950321 / -79228162514264337593543950335 =
  // 0.0000000000000000000000000001
  s21_decimal d1 = {{0xfffffff1, 0xffffffff, 0xffffffff, 0x801c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test13 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000001, "div_test13 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test13 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test13 FAIL");
  ck_assert_msg(res.bits[3] == 0x001c0000, "div_test13 FAIL");
}
END_TEST

START_TEST(div_test14) {
  // 7.9228162514264337593543950321 / -79228162514264337593543950335 =
  // -0.0000000000000000000000000001
  s21_decimal d1 = {{0xfffffff1, 0xffffffff, 0xffffffff, 0x001c0000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test14 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000001, "div_test14 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test14 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test14 FAIL");
  ck_assert_msg(res.bits[3] == 0x801c0000, "div_test14 FAIL");
}
END_TEST

START_TEST(div_test15) {
  // 0 / 0 = err3
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 3, "div_test15.1 FAIL");
  // 0.0000000000000000000000000000 / 0 = err3
  d1.bits[3] = 0x001c0000;
  ck_assert_msg(s21_div(d1, d2, &res) == 3, "div_test15.2 FAIL");
  // 0.0000000000000000000000000000 / 0.0000000000000000000000000000 = err3
  d2.bits[3] = 0x001c0000;
  ck_assert_msg(s21_div(d1, d2, &res) == 3, "div_test15.3 FAIL");
  // 0.0000000000000000000000000000 / -0.0000000000000000000000000000 = err3
  d2.bits[3] = 0x801c0000;
  ck_assert_msg(s21_div(d1, d2, &res) == 3, "div_test15.4 FAIL");
  // -0.0000000000000000000000000000 / -0.0000000000000000000000000000 = err3
  d1.bits[3] = 0x801c0000;
  ck_assert_msg(s21_div(d1, d2, &res) == 3, "div_test15.5 FAIL");
}
END_TEST

START_TEST(div_test16) {
  // -0.0000000000000000000000000000 / -0.0000000000000000000000000001 = 0
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x801c0000}};
  s21_decimal d2 = {{0x0000001, 0x00000000, 0x00000000, 0x801c0000}};
  s21_decimal res;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test16.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "div_test16.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test16.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test16.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "div_test16.1 FAIL");

  // 0.0000000000000000000000000000 / -0.0000000000000000000000000001 = -0
  d1.bits[3] = 0x001c0000;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test16.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "div_test16.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test16.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test16.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "div_test16.2 FAIL");

  // 0.0000000000000000000000000000 / 0.0000000000000000000000000001 = 0
  d2.bits[3] = 0x001c0000;
  ck_assert_msg(s21_div(d1, d2, &res) == 0, "div_test16.3 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "div_test16.3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "div_test16.3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "div_test16.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "div_test16.3 FAIL");
}
END_TEST

// EQUAL
START_TEST(eq_test1) {
  // 0.0 vs 0
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal d2 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test1.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test1.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test1.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test1.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test1.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test1.6 FAIL");
}
END_TEST

START_TEST(eq_test2) {
  // 2 vs 4
  s21_decimal d1 = {{0x00000002, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal d2 = {{0x00000004, 0x00000000, 0x00000000, 0x00000000}};
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test2 FAIL");
}
END_TEST

START_TEST(eq_test3) {
  // -0.0000000000 vs 0.0000000000000000000000000000
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x800a0000}};
  s21_decimal d2 = {{0x00000000, 0x00000000, 0x00000000, 0x001c0000}};
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test3.1.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test3.1.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test3.1.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test3.1.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test3.1.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test3.1.6 FAIL");

  // -0.0000000000 vs -0
  d2.bits[3] = 0x80000000;
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test3.2.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test3.2.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test3.2.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test3.2.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test3.2.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test3.2.6 FAIL");

  // 0.0000000000 vs 0.0000000000000000000000000000
  d1.bits[3] = 0x000a0000;
  d2.bits[3] = 0x001c0000;
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test3.3.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test3.3.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test3.3.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test3.3.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test3.3.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test3.3.6 FAIL");

  // 0.0000000000 vs -0.0000000000000000000000000000
  d1.bits[3] = 0x000a0000;
  d2.bits[3] = 0x801c0000;
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test3.4.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test3.4.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test3.4.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test3.4.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test3.4.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test3.4.6 FAIL");
}
END_TEST

START_TEST(eq_test4) {
  // 85899345.00 vs 85899345.0
  s21_decimal d1 = {{0xffffffa4, 0x00000001, 0x00000000, 0x00020000}};
  s21_decimal d2 = {{0x3333332a, 0x00000000, 0x00000000, 0x00010000}};
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test4.1.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test4.1.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test4.1.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test4.1.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test4.1.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test4.1.6 FAIL");

  // -85899345.00 vs 85899345.0
  d1.bits[3] = 0x80020000;
  ck_assert_msg(s21_is_less(d1, d2), "eq_test4.2.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test4.2.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test4.2.3 FAIL");
  ck_assert_msg(!s21_is_greater_or_equal(d1, d2), "eq_test4.2.4 FAIL");
  ck_assert_msg(!s21_is_equal(d1, d2), "eq_test4.2.5 FAIL");
  ck_assert_msg(s21_is_not_equal(d1, d2), "eq_test4.2.6 FAIL");

  // -85899345.00 vs -85899345.0
  d2.bits[3] = 0x80010000;
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test4.3.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test4.3.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test4.3.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test4.3.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test4.3.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test4.3.6 FAIL");

  // 85899345.00 vs -85899345.0
  d1.bits[3] = 0x00020000;
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test4.4.1 FAIL");
  ck_assert_msg(!s21_is_less_or_equal(d1, d2), "eq_test4.4.2 FAIL");
  ck_assert_msg(s21_is_greater(d1, d2), "eq_test4.4.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test4.4.4 FAIL");
  ck_assert_msg(!s21_is_equal(d1, d2), "eq_test4.4.5 FAIL");
  ck_assert_msg(s21_is_not_equal(d1, d2), "eq_test4.4.6 FAIL");
}
END_TEST

START_TEST(eq_test5) {
  // 79228162514264337593543950335 vs 79228162514264337593543950335
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  s21_decimal d2 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00000000}};
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test5.1.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test5.1.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test5.1.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test5.1.4 FAIL");
  ck_assert_msg(s21_is_equal(d1, d2), "eq_test5.1.5 FAIL");
  ck_assert_msg(!s21_is_not_equal(d1, d2), "eq_test5.1.6 FAIL");

  // 79228162514264337593543950335 vs -79228162514264337593543950335
  d2.bits[3] = 0x80000000;
  ck_assert_msg(!s21_is_less(d1, d2), "eq_test5.2.1 FAIL");
  ck_assert_msg(!s21_is_less_or_equal(d1, d2), "eq_test5.2.2 FAIL");
  ck_assert_msg(s21_is_greater(d1, d2), "eq_test5.2.3 FAIL");
  ck_assert_msg(s21_is_greater_or_equal(d1, d2), "eq_test5.2.4 FAIL");
  ck_assert_msg(!s21_is_equal(d1, d2), "eq_test5.2.5 FAIL");
  ck_assert_msg(s21_is_not_equal(d1, d2), "eq_test5.2.6 FAIL");
}
END_TEST

START_TEST(eq_test6) {
  // 3.9614081257132168796771975167 vs 7.9228162514264337593543950321
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0x7fffffff, 0x001c0000}};
  s21_decimal d2 = {{0xfffffff1, 0xffffffff, 0xffffffff, 0x001c0000}};
  ck_assert_msg(s21_is_less(d1, d2), "eq_test6.1 FAIL");
  ck_assert_msg(s21_is_less_or_equal(d1, d2), "eq_test6.2 FAIL");
  ck_assert_msg(!s21_is_greater(d1, d2), "eq_test6.3 FAIL");
  ck_assert_msg(!s21_is_greater_or_equal(d1, d2), "eq_test6.4 FAIL");
  ck_assert_msg(!s21_is_equal(d1, d2), "eq_test6.5 FAIL");
  ck_assert_msg(s21_is_not_equal(d1, d2), "eq_test6.6 FAIL");
}
END_TEST

// CONV
START_TEST(conv_test1) {
  // s21_from_float_to_decimal
  s21_decimal d1;
  float f = 1.0;
  ck_assert_msg(s21_from_float_to_decimal(f, &d1) == 0, "conv_test1.1 FAIL");

  ck_assert_msg(d1.bits[0] == 0x00000001, "conv_test1.1 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.1 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.1 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00000000, "conv_test1.1 FAIL");

  ck_assert_msg(s21_from_float_to_decimal(-f, &d1) == 0, "conv_test1.2 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000001, "conv_test1.2 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.2 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.2 FAIL");
  ck_assert_msg(d1.bits[3] == 0x80000000, "conv_test1.2 FAIL");

  f = 5.5;
  ck_assert_msg(s21_from_float_to_decimal(f, &d1) == 0, "conv_test1.3 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000037, "conv_test1.3 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.3 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.3 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00010000, "conv_test1.3 FAIL");

  ck_assert_msg(s21_from_float_to_decimal(-f, &d1) == 0, "conv_test1.4 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000037, "conv_test1.4 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.4 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.4 FAIL");
  ck_assert_msg(d1.bits[3] == 0x80010000, "conv_test1.4 FAIL");

  f = 0.5;
  ck_assert_msg(s21_from_float_to_decimal(f, &d1) == 0, "conv_test1.5 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000005, "conv_test1.5 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.5 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.5 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00010000, "conv_test1.5 FAIL");

  ck_assert_msg(s21_from_float_to_decimal(-f, &d1) == 0, "conv_test1.6 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000005, "conv_test1.6 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.6 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.6 FAIL");
  ck_assert_msg(d1.bits[3] == 0x80010000, "conv_test1.6 FAIL");

  f = 545.342432432;
  ck_assert_msg(s21_from_float_to_decimal(f, &d1) == 0, "conv_test1.7 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00533670, "conv_test1.7 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.7 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.7 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00040000, "conv_test1.7 FAIL");

  ck_assert_msg(s21_from_float_to_decimal(-f, &d1) == 0, "conv_test1.8 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00533670, "conv_test1.8 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test1.8 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test1.8 FAIL");
  ck_assert_msg(d1.bits[3] == 0x80040000, "conv_test1.8 FAIL");
}
END_TEST

START_TEST(conv_test2) {
  // s21_from_int_to_decimal
  s21_decimal d1;
  int i = 0;
  ck_assert_msg(s21_from_int_to_decimal(i, &d1) == 0, "conv_test2.1 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000000, "conv_test2.1 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test2.1 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test2.1 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00000000, "conv_test2.1 FAIL");

  ck_assert_msg(s21_from_int_to_decimal(-i, &d1) == 0, "conv_test2.2 FAIL");
  ck_assert_msg(d1.bits[0] == 0x00000000, "conv_test2.2 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test2.2 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test2.2 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00000000, "conv_test2.2 FAIL");

  i = 2147483647;
  ck_assert_msg(s21_from_int_to_decimal(i, &d1) == 0, "conv_test2.3 FAIL");
  ck_assert_msg(d1.bits[0] == 0x7fffffff, "conv_test2.3 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test2.3 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test2.3 FAIL");
  ck_assert_msg(d1.bits[3] == 0x00000000, "conv_test2.3 FAIL");

  ck_assert_msg(s21_from_int_to_decimal(-i, &d1) == 0, "conv_test2.4 FAIL");
  ck_assert_msg(d1.bits[0] == 0x7fffffff, "conv_test2.4 FAIL");
  ck_assert_msg(d1.bits[1] == 0x00000000, "conv_test2.4 FAIL");
  ck_assert_msg(d1.bits[2] == 0x00000000, "conv_test2.4 FAIL");
  ck_assert_msg(d1.bits[3] == 0x80000000, "conv_test2.4 FAIL");
}
END_TEST

START_TEST(conv_test3) {
  // s21_from_decimal_to_int
  int ans = 0;

  // 0
  s21_decimal d1 = {0};
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "eq_test3.1 FAIL");
  ck_assert_msg(ans == 0, "conv_test3.1 FAIL");

  // -0
  d1.bits[3] = 0x80000000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "eq_test3.2 FAIL");
  ck_assert_msg(ans == 0, "conv_test3.2 FAIL");

  // -0.0000000000000000000000000000
  d1.bits[3] = 0x801c0000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "eq_test3.3 FAIL");
  ck_assert_msg(ans == 0, "conv_test3.3 FAIL");

  // 0.0000000000000000000000000000
  d1.bits[3] = 0x001c0000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "eq_test3.4 FAIL");
  ck_assert_msg(ans == 0, "conv_test3.4 FAIL");
}
END_TEST

START_TEST(conv_test4) {
  // s21_from_decimal_to_int
  int ans = 0;

  // -7.9228162514264337593543950335
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x801c0000}};
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "eq_test4.1 FAIL");
  ck_assert_msg(ans == -7, "conv_test4.1 FAIL");

  // 7.9228162514264337593543950335
  d1.bits[3] = 0x001c0000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "eq_test4.2 FAIL");
  ck_assert_msg(ans == 7, "conv_test4.2 FAIL");
}
END_TEST

START_TEST(conv_test5) {
  // s21_from_decimal_to_int
  int ans = 0;
  s21_decimal d1 = {0};

  // 2147483647
  d1.bits[0] = 0x7fffffff;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "conv_test5.1 FAIL");
  ck_assert_msg(ans == 2147483647, "conv_test5.1 FAIL");

  // -2147483647
  d1.bits[3] = 0x80000000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "conv_test5.2 FAIL");
  ck_assert_msg(ans == -2147483647, "conv_test5.2 FAIL");

  // -2147483648 = err1
  d1.bits[0] = 0x80000000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 1, "conv_test5.3 FAIL");

  // 2147483648 =  err1
  d1.bits[3] = 0x00000000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 1, "conv_test5.4 FAIL");

  // 214748364.8
  d1.bits[3] = 0x00010000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "conv_test5.5 FAIL");
  ck_assert_msg(ans == 2147483647 / 10, "conv_test5.5 FAIL");

  // -214748364.8
  d1.bits[3] = 0x80010000;
  ck_assert_msg(s21_from_decimal_to_int(d1, &ans) == 0, "conv_test5.6 FAIL");
  ck_assert_msg(ans == -2147483647 / 10, "conv_test5.6 FAIL");
}
END_TEST

START_TEST(conv_test6) {
  // s21_from_decimal_to_float
  float ans = 0.0;
  s21_decimal d1 = {0};

  // 0.0000000000000000000000000000
  d1.bits[3] = 0x001c0000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test6.1 FAIL");
  ck_assert_msg(ans == 0.0, "conv_test6.1 FAIL");

  // -0.0000000000000000000000000000
  d1.bits[3] = 0x801c0000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test6.2 FAIL");
  ck_assert_msg(ans == 0.0, "conv_test6.2 FAIL");

  // 6.4
  d1.bits[0] = 0x00000040;
  d1.bits[3] = 0x00010000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test6.3 FAIL");
  ck_assert_msg(ans == 6.4f, "conv_test6.3 FAIL");

  // -6.4
  d1.bits[3] = 0x80010000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test6.4 FAIL");
  ck_assert_msg(ans == -6.4f, "conv_test6.4 FAIL");
}
END_TEST

START_TEST(conv_test7) {
  // s21_from_decimal_to_float
  float ans = 0.0;

  // 1027837890029898849765134931
  s21_decimal d1 = {{0x25399253, 0xaaaaaaaa, 0x03523523, 0x00000000}};
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test7.1 FAIL");
  ck_assert_msg(ans == 1027837890029898849765134931.0f, "conv_test7.1 FAIL");

  // -1027837890029898849765134931
  d1.bits[3] = 0x80000000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test7.2 FAIL");
  ck_assert_msg(ans == -1027837890029898849765134931.0f, "conv_test7.2 FAIL");

  // 0.1027837890029898849765134931
  d1.bits[3] = 0x001c0000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test7.3 FAIL");
  ck_assert_msg(ans == 0.1027837890029898849765134931f, "conv_test7.3 FAIL");

  // -0.1027837890029898849765134931
  d1.bits[3] = 0x801c0000;
  ck_assert_msg(s21_from_decimal_to_float(d1, &ans) == 0, "conv_test7.4 FAIL");
  ck_assert_msg(ans == -0.1027837890029898849765134931f, "conv_test7.4 FAIL");
}
END_TEST

// ROUND
START_TEST(round_test1) {
  // 0
  s21_decimal d1 = {0};
  s21_decimal res;

  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test1.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test1.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test1.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test1.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test1.1 FAIL");

  ck_assert_msg(s21_round(d1, &res) == 0, "round_test1.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test1.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test1.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test1.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test1.2 FAIL");

  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test1.3 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test1.3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test1.3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test1.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test1.3 FAIL");

  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test1.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test1.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test1.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test1.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test1.4 FAIL");
}
END_TEST

START_TEST(round_test2) {
  // 0.0000000000000000000000000000
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x001c0000}};
  s21_decimal res;

  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test2.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test2.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test2.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test2.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test2.1 FAIL");

  ck_assert_msg(s21_round(d1, &res) == 0, "round_test2.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test2.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test2.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test2.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test2.2 FAIL");

  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test2.3 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test2.3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test2.3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test2.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x801c0000, "round_test2.3 FAIL");

  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test2.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test2.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test2.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test2.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test2.4 FAIL");
}
END_TEST

START_TEST(round_test3) {
  // -0.0000000000000000000000000000
  s21_decimal d1 = {{0x00000000, 0x00000000, 0x00000000, 0x801c0000}};
  s21_decimal res;

  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test3.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test3.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test3.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test3.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test3.1 FAIL");

  ck_assert_msg(s21_round(d1, &res) == 0, "round_test3.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test3.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test3.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test3.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test3.2 FAIL");

  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test3.3 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test3.3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test3.3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test3.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x001c0000, "round_test3.3 FAIL");

  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test3.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000000, "round_test3.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test3.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test3.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test3.4 FAIL");
}
END_TEST

START_TEST(round_test4) {
  // 6.5
  s21_decimal d1 = {{0x00000041, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal res;

  // 6
  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test4.1 FAIL");
  ck_assert_msg(res.bits[0] == 6, "round_test4.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test4.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test4.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test4.1 FAIL");

  // 7
  ck_assert_msg(s21_round(d1, &res) == 0, "round_test4.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000007, "round_test4.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test4.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test4.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test4.2 FAIL");

  // -6.5
  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test4.3 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000041, "round_test4.3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test4.3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test4.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x80010000, "round_test4.3 FAIL");

  // 6
  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test4.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000006, "round_test4.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test4.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test4.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test4.4 FAIL");
}
END_TEST

START_TEST(round_test5) {
  // -6.5
  s21_decimal d1 = {{0x00000041, 0x00000000, 0x00000000, 0x80010000}};
  s21_decimal res;

  // -6
  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test5.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000006, "round_test5.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test5.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test5.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test5.1 FAIL");

  // -7
  ck_assert_msg(s21_round(d1, &res) == 0, "round_test5.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000007, "round_test5.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test5.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test5.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test5.2 FAIL");

  // 6.5
  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test5.3 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000041, "round_test5.3 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test5.3 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test5.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x00010000, "round_test5.3 FAIL");

  // -7
  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test5.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000007, "round_test5.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test5.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test5.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test5.4 FAIL");
}
END_TEST

START_TEST(round_test6) {
  // 7.9228162514264337593543950335
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x001c0000}};
  s21_decimal res;

  // 7
  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test6.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000007, "round_test6.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test6.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test6.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test6.1 FAIL");

  // 8
  ck_assert_msg(s21_round(d1, &res) == 0, "round_test6.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000008, "round_test6.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test6.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test6.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test6.2 FAIL");

  // -7.9228162514264337593543950335
  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test6.3 FAIL");
  ck_assert_msg(res.bits[0] == 0xffffffff, "round_test6.3 FAIL");
  ck_assert_msg(res.bits[1] == 0xffffffff, "round_test6.3 FAIL");
  ck_assert_msg(res.bits[2] == 0xffffffff, "round_test6.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x801c0000, "round_test6.3 FAIL");

  // 7
  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test6.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000007, "round_test6.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test6.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test6.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x00000000, "round_test6.4 FAIL");
}
END_TEST

START_TEST(round_test7) {
  // -7.9228162514264337593543950335
  s21_decimal d1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x801c0000}};
  s21_decimal res;

  // -7
  ck_assert_msg(s21_truncate(d1, &res) == 0, "round_test7.1 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000007, "round_test7.1 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test7.1 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test7.1 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test7.1 FAIL");

  // -8
  ck_assert_msg(s21_round(d1, &res) == 0, "round_test7.2 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000008, "round_test7.2 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test7.2 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test7.2 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test7.2 FAIL");

  // 7.9228162514264337593543950335
  ck_assert_msg(s21_negate(d1, &res) == 0, "round_test7.3 FAIL");
  ck_assert_msg(res.bits[0] == 0xffffffff, "round_test7.3 FAIL");
  ck_assert_msg(res.bits[1] == 0xffffffff, "round_test7.3 FAIL");
  ck_assert_msg(res.bits[2] == 0xffffffff, "round_test7.3 FAIL");
  ck_assert_msg(res.bits[3] == 0x001c0000, "round_test7.3 FAIL");

  // -8
  ck_assert_msg(s21_floor(d1, &res) == 0, "round_test7.4 FAIL");
  ck_assert_msg(res.bits[0] == 0x00000008, "round_test7.4 FAIL");
  ck_assert_msg(res.bits[1] == 0x00000000, "round_test7.4 FAIL");
  ck_assert_msg(res.bits[2] == 0x00000000, "round_test7.4 FAIL");
  ck_assert_msg(res.bits[3] == 0x80000000, "round_test7.4 FAIL");
}
END_TEST

int main(void) {
  Suite *suite = suite_create("core");
  TCase *tc = tcase_create("core");
  SRunner *sr = srunner_create(suite);

  suite_add_tcase(suite, tc);

  tcase_add_test(tc, add_test1);
  tcase_add_test(tc, add_test2);
  tcase_add_test(tc, add_test3);
  tcase_add_test(tc, add_test4);
  tcase_add_test(tc, add_test5);
  tcase_add_test(tc, add_test6);
  tcase_add_test(tc, add_test7);
  tcase_add_test(tc, add_test8);
  tcase_add_test(tc, add_test9);
  tcase_add_test(tc, add_test10);
  tcase_add_test(tc, add_test11);
  tcase_add_test(tc, add_test12);
  tcase_add_test(tc, add_test13);
  tcase_add_test(tc, add_test14);
  tcase_add_test(tc, add_test15);
  tcase_add_test(tc, sub_test1);
  tcase_add_test(tc, sub_test2);
  tcase_add_test(tc, sub_test3);
  tcase_add_test(tc, sub_test4);
  tcase_add_test(tc, sub_test5);
  tcase_add_test(tc, sub_test6);
  tcase_add_test(tc, sub_test7);
  tcase_add_test(tc, sub_test8);
  tcase_add_test(tc, sub_test9);
  tcase_add_test(tc, sub_test10);
  tcase_add_test(tc, sub_test11);
  tcase_add_test(tc, sub_test12);
  tcase_add_test(tc, sub_test13);
  tcase_add_test(tc, sub_test14);
  tcase_add_test(tc, sub_test15);
  tcase_add_test(tc, sub_test16);
  tcase_add_test(tc, sub_test17);
  tcase_add_test(tc, mul_test1);
  tcase_add_test(tc, mul_test2);
  tcase_add_test(tc, mul_test3);
  tcase_add_test(tc, mul_test4);
  tcase_add_test(tc, mul_test5);
  tcase_add_test(tc, div_test1);
  tcase_add_test(tc, div_test2);
  tcase_add_test(tc, div_test3);
  tcase_add_test(tc, div_test4);
  tcase_add_test(tc, div_test5);
  tcase_add_test(tc, div_test6);
  tcase_add_test(tc, div_test7);
  tcase_add_test(tc, div_test8);
  tcase_add_test(tc, div_test9);
  tcase_add_test(tc, div_test10);
  tcase_add_test(tc, div_test11);
  tcase_add_test(tc, div_test12);
  tcase_add_test(tc, div_test13);
  tcase_add_test(tc, div_test14);
  tcase_add_test(tc, div_test15);
  tcase_add_test(tc, div_test16);
  tcase_add_test(tc, eq_test1);
  tcase_add_test(tc, eq_test2);
  tcase_add_test(tc, eq_test3);
  tcase_add_test(tc, eq_test4);
  tcase_add_test(tc, eq_test5);
  tcase_add_test(tc, eq_test6);
  tcase_add_test(tc, conv_test1);
  tcase_add_test(tc, conv_test2);
  tcase_add_test(tc, conv_test3);
  tcase_add_test(tc, conv_test4);
  tcase_add_test(tc, conv_test5);
  tcase_add_test(tc, conv_test6);
  tcase_add_test(tc, conv_test7);
  tcase_add_test(tc, round_test1);
  tcase_add_test(tc, round_test2);
  tcase_add_test(tc, round_test3);
  tcase_add_test(tc, round_test4);
  tcase_add_test(tc, round_test5);
  tcase_add_test(tc, round_test6);
  tcase_add_test(tc, round_test7);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  srunner_free(sr);

  return 0;
}
