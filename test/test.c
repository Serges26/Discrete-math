#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "boolean_forms.h"

/* f(x1, x2) = x1 XOR x2, вектор 0,1,1,0 => "0110" */
START_TEST(test_xor_2vars) {
    const char *vec = "0110";
    BooleanForms f;
    int rc = build_boolean_forms(vec, &f);
    ck_assert_int_eq(rc, BF_OK);

    ck_assert_str_eq(f.pdnf, "(!x1 & x2) | (x1 & !x2)");
    ck_assert_str_eq(f.pcnf, "(x1 | x2) & (!x1 | !x2)");

    free_boolean_forms(&f);
}
END_TEST

/* Тождественный ноль для двух переменных: "0000" */
START_TEST(test_zero_function) {
    const char *vec = "0000";
    BooleanForms f;
    int rc = build_boolean_forms(vec, &f);
    ck_assert_int_eq(rc, BF_OK);

    ck_assert_str_eq(f.pdnf, "0");
    ck_assert_str_eq(
        f.pcnf,
        "(x1 | x2) & (x1 | !x2) & (!x1 | x2) & (!x1 | !x2)"
    );

    free_boolean_forms(&f);
}
END_TEST

/* Тождественная единица для двух переменных: "1111" */
START_TEST(test_one_function) {
    const char *vec = "1111";
    BooleanForms f;
    int rc = build_boolean_forms(vec, &f);
    ck_assert_int_eq(rc, BF_OK);

    /* Четыре минтерма в СДНФ (порядок строго задан) */
    ck_assert_str_eq(
        f.pdnf,
        "(!x1 & !x2) | (!x1 & x2) | (x1 & !x2) | (x1 & x2)"
    );
    /* СКНФ для тожд. 1 — 1 (пустая конъюнкция) */
    ck_assert_str_eq(f.pcnf, "1");

    free_boolean_forms(&f);
}
END_TEST

/* Неверные символы во входном векторе */
START_TEST(test_bad_char) {
    const char *vec = "01a0";
    BooleanForms f;
    int rc = build_boolean_forms(vec, &f);
    ck_assert_int_eq(rc, BF_ERR_BAD_CHAR);
}
END_TEST

/* Длина не степень двойки */
START_TEST(test_bad_length) {
    const char *vec = "010"; /* длина 3 */
    BooleanForms f;
    int rc = build_boolean_forms(vec, &f);
    ck_assert_int_eq(rc, BF_ERR_BAD_LENGTH);
}
END_TEST

/* NULL-аргументы */
START_TEST(test_null_args) {
    BooleanForms f;
    int rc1 = build_boolean_forms(NULL, &f);
    ck_assert_int_eq(rc1, BF_ERR_BAD_ARGS);

    int rc2 = build_boolean_forms("01", NULL);
    ck_assert_int_eq(rc2, BF_ERR_BAD_ARGS);
}
END_TEST

Suite *boolean_suite(void) {
    Suite *s = suite_create("boolean_forms");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_xor_2vars);
    tcase_add_test(tc_core, test_zero_function);
    tcase_add_test(tc_core, test_one_function);
    tcase_add_test(tc_core, test_bad_char);
    tcase_add_test(tc_core, test_bad_length);
    tcase_add_test(tc_core, test_null_args);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = boolean_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
