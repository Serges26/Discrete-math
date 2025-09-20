
// File: test_join.c
#include <check.h>
#include "../src/join.h"

START_TEST(test_join_basic)
{
    struct Pair r_pairs[] = { {1,2}, {2,3} };
    struct Pair s_pairs[] = { {2,4}, {3,5} };
    struct Relation *r = relation_from_pairs(r_pairs, 2);
    struct Relation *s = relation_from_pairs(s_pairs, 2);

    struct Relation *t = join_binary(r, s);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(t->size, 2);
    ck_assert(relation_contains(t, (struct Pair){1,4}));
    ck_assert(relation_contains(t, (struct Pair){2,5}));

    /* original relations unchanged */
    ck_assert_int_eq(r->size, 2);
    ck_assert_int_eq(s->size, 2);

    relation_free(r);
    relation_free(s);
    relation_free(t);
}
END_TEST

START_TEST(test_join_empty_inputs)
{
    struct Relation *r = relation_empty();
    struct Relation *s = relation_empty();
    struct Relation *t = join_binary(r, s);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(t->size, 0);
    relation_free(r);
    relation_free(s);
    relation_free(t);
}
END_TEST

START_TEST(test_join_one_empty)
{
    struct Pair r_pairs[] = { {1,2} };
    struct Relation *r = relation_from_pairs(r_pairs, 1);
    struct Relation *s = relation_empty();
    struct Relation *t = join_binary(r, s);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(t->size, 0);
    relation_free(r);
    relation_free(s);
    relation_free(t);
}
END_TEST

START_TEST(test_join_duplicates)
{
    /* duplicates in input should not produce duplicates in output */
    struct Pair r_pairs[] = { {1,2}, {1,2} };
    struct Pair s_pairs[] = { {2,3}, {2,3} };
    struct Relation *r = relation_from_pairs(r_pairs, 2);
    struct Relation *s = relation_from_pairs(s_pairs, 2);
    struct Relation *t = join_binary(r, s);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(t->size, 1);
    ck_assert(relation_contains(t, (struct Pair){1,3}));
    relation_free(r);
    relation_free(s);
    relation_free(t);
}
END_TEST

START_TEST(test_join_no_overlap)
{
    struct Pair r_pairs[] = { {1,2} };
    struct Pair s_pairs[] = { {3,4} };
    struct Relation *r = relation_from_pairs(r_pairs, 1);
    struct Relation *s = relation_from_pairs(s_pairs, 1);
    struct Relation *t = join_binary(r, s);
    ck_assert_ptr_nonnull(t);
    ck_assert_int_eq(t->size, 0);
    relation_free(r);
    relation_free(s);
    relation_free(t);
}
END_TEST

START_TEST(test_join_null_inputs)
{
    struct Relation *r = NULL;
    struct Relation *s = NULL;
    struct Relation *t = join_binary(r, s);
    ck_assert_ptr_null(t);
}
END_TEST

Suite * join_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("join");
    tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_join_basic);
    tcase_add_test(tc_core, test_join_empty_inputs);
    tcase_add_test(tc_core, test_join_one_empty);
    tcase_add_test(tc_core, test_join_duplicates);
    tcase_add_test(tc_core, test_join_no_overlap);
    tcase_add_test(tc_core, test_join_null_inputs);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    SRunner *sr = srunner_create(join_suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}


