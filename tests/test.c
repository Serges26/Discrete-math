#include <check.h>
#include <stdlib.h>

#include "../src/clause.h"


START_TEST(test_empty_clauses) {
    int res = is_sat(NULL, 0);
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_single_clause_positive) {
    clause_t kl[] = { {1, 2, 3} };
    int res = is_sat(kl, 1);
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_single_clause_negative) {
    clause_t kl[] = { {-1, -1, -1} };
    int res = is_sat(kl, 1);
    ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_contradiction_two_clauses_unsat) {
    clause_t kl[] = { {1,1,1}, {-1,-1,-1} };
    int res = is_sat(kl, 2);
    ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_multi_clause_sat) {
    clause_t kl[] = { {1, -2, 3}, {-1, 2, -3} };
    int res = is_sat(kl, 2);
    ck_assert_int_eq(res, 1);
}
END_TEST


START_TEST(test_high_index_var) {
    clause_t kl[] = { {5, -3, 1} };
    int res = is_sat(kl, 1);
    ck_assert_int_eq(res, 1);
}
END_TEST

Suite *sat_suite(void) {
    Suite *s = suite_create("sat");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_empty_clauses);
    tcase_add_test(tc_core, test_single_clause_positive);
    tcase_add_test(tc_core, test_single_clause_negative);
    tcase_add_test(tc_core, test_contradiction_two_clauses_unsat);
    tcase_add_test(tc_core, test_multi_clause_sat);
    tcase_add_test(tc_core, test_high_index_var);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = sat_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
