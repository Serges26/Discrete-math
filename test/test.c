#include <check.h>
#include <stdlib.h>
#include "zhegalkin.h"


START_TEST(test_and_two_vars) {
    int f[4] = {0, 0, 0, 1};
    ZhegalkinAnf anf = {0};

    ck_assert_int_eq(zhegalkin_from_truth(f, 2, &anf), 0);
    ck_assert_uint_eq(anf.size, 4);

    ck_assert_uint_eq(anf.coeffs[0], 0);
    ck_assert_uint_eq(anf.coeffs[1], 0);
    ck_assert_uint_eq(anf.coeffs[2], 0);
    ck_assert_uint_eq(anf.coeffs[3], 1);

    char *s = zhegalkin_to_string(&anf);
    ck_assert_ptr_nonnull(s);
    ck_assert_str_eq(s, "x0x1");

    free(s);
    zhegalkin_free(&anf);
}
END_TEST

START_TEST(test_xor_two_vars) {
    int f[4] = {0, 1, 1, 0};
    ZhegalkinAnf anf = {0};

    ck_assert_int_eq(zhegalkin_from_truth(f, 2, &anf), 0);

    ck_assert_uint_eq(anf.coeffs[0], 0);
    ck_assert_uint_eq(anf.coeffs[1], 1);
    ck_assert_uint_eq(anf.coeffs[2], 1); 
    ck_assert_uint_eq(anf.coeffs[3], 0);

    char *s = zhegalkin_to_string(&anf);
    ck_assert_ptr_nonnull(s);
    ck_assert_str_eq(s, "x0 + x1");

    free(s);
    zhegalkin_free(&anf);
}
END_TEST

START_TEST(test_const_one) {
    int f[8];
    for (int i = 0; i < 8; ++i) f[i] = 1;

    ZhegalkinAnf anf = {0};
    ck_assert_int_eq(zhegalkin_from_truth(f, 3, &anf), 0);

    ck_assert_uint_eq(anf.coeffs[0], 1);
    for (size_t i = 1; i < anf.size; ++i) {
        ck_assert_uint_eq(anf.coeffs[i], 0);
    }

    char *s = zhegalkin_to_string(&anf);
    ck_assert_ptr_nonnull(s);
    ck_assert_str_eq(s, "1");

    free(s);
    zhegalkin_free(&anf);
}
END_TEST

START_TEST(test_const_zero) {
    int f[8] = {0};

    ZhegalkinAnf anf = {0};
    ck_assert_int_eq(zhegalkin_from_truth(f, 3, &anf), 0);

    for (size_t i = 0; i < anf.size; ++i) {
        ck_assert_uint_eq(anf.coeffs[i], 0);
    }

    char *s = zhegalkin_to_string(&anf);
    ck_assert_ptr_nonnull(s);
    ck_assert_str_eq(s, "0");

    free(s);
    zhegalkin_free(&anf);
}
END_TEST

START_TEST(test_bad_args) {
    ZhegalkinAnf anf = {0};
    int f[2] = {0, 1};

    ck_assert_int_ne(zhegalkin_from_truth(NULL, 1, &anf), 0);
    ck_assert_int_ne(zhegalkin_from_truth(f, 1, NULL), 0);

    ck_assert_ptr_eq(zhegalkin_to_string(NULL), NULL);
}
END_TEST

Suite *zhegalkin_suite(void) {
    Suite *s = suite_create("zhegalkin");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_and_two_vars);
    tcase_add_test(tc, test_xor_two_vars);
    tcase_add_test(tc, test_const_one);
    tcase_add_test(tc, test_const_zero);
    tcase_add_test(tc, test_bad_args);

    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = zhegalkin_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
