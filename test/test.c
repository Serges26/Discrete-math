
#include <check.h>
#include <stdlib.h>
#include <time.h>
#include "../include/tree.h"

// вспомогательная проверка: граф неориентированный 
static void assert_tree_undirected(const Tree *t) {
    for (size_t u = 1; u <= t->n; ++u) {
        for (size_t i = 0; i < t->deg[u]; ++i) {
            size_t v = t->adj[u][i];
            int found = 0;
            for (size_t j = 0; j < t->deg[v]; ++j) {
                if (t->adj[v][j] == u) {
                    found = 1;
                    break;
                }
            }
            ck_assert_msg(found, "edge %zu-%zu is not symmetric", u, v);
        }
    }
}


START_TEST(test_prufer_decode_small)
{
    size_t code[2] = {4, 4};
    Tree t;
    int rc = tree_from_prufer(4, code, &t);
    ck_assert_int_eq(rc, 0);

    ck_assert_int_eq(t.deg[4], 3);
    for (size_t i = 1; i <= 3; ++i) {
        ck_assert_int_eq(t.deg[i], 1);
    }

    assert_tree_undirected(&t);
    tree_free(&t);
}
END_TEST


START_TEST(test_prufer_n1)
{
    Tree t;
    int rc = tree_from_prufer(1, NULL, &t);
    ck_assert_int_eq(rc, 0);
    ck_assert_int_eq(t.n, 1);
    ck_assert_ptr_nonnull(t.deg);
    ck_assert_ptr_nonnull(t.adj);
    tree_free(&t);
}
END_TEST


START_TEST(test_random_n2)
{
    Tree t;
    int rc = tree_random(2, &t);
    ck_assert_int_eq(rc, 0);
    ck_assert_int_eq(t.n, 2);
    ck_assert_int_eq(t.deg[1] + t.deg[2], 2);
    assert_tree_undirected(&t);
    tree_free(&t);
}
END_TEST


START_TEST(test_prufer_invalid_value)
{
    size_t code[2] = {5, 99};
    Tree t;
    int rc = tree_from_prufer(4, code, &t);
    ck_assert_int_ne(rc, 0);
}
END_TEST
START_TEST(test_random_tree_edges_count)
{
    Tree t;
    int rc = tree_random(5, &t);
    ck_assert_int_eq(rc, 0);

    size_t sum_deg = 0;
    for (size_t i = 1; i <= t.n; ++i) {
        sum_deg += t.deg[i];
    }
    ck_assert_int_eq(sum_deg, 2 * (t.n - 1));
    assert_tree_undirected(&t);
    tree_free(&t);
}
END_TEST

START_TEST(test_prufer_example_5)
{
    size_t code[3] = {4, 4, 5};
    Tree t;
    int rc = tree_from_prufer(5, code, &t);
    ck_assert_int_eq(rc, 0);

    ck_assert_int_eq(t.deg[1], 1);
    ck_assert_int_eq(t.deg[2], 1);
    ck_assert_int_eq(t.deg[3], 1);
    ck_assert_int_eq(t.deg[4], 3);
    ck_assert_int_eq(t.deg[5], 2);

    assert_tree_undirected(&t);
    tree_free(&t);
}
END_TEST

START_TEST(test_random_many)
{
    for (int k = 0; k < 50; ++k) {
        Tree t;
        int rc = tree_random(8, &t);
        ck_assert_int_eq(rc, 0);

        size_t sum_deg = 0;
        for (size_t i = 1; i <= t.n; ++i) {
            sum_deg += t.deg[i];
        }
        ck_assert_int_eq(sum_deg, 2 * (t.n - 1));
        assert_tree_undirected(&t);
        tree_free(&t);
    }
}
END_TEST

START_TEST(test_bad_inputs)
{
    size_t code_dummy[1] = {1};
    Tree t;
    int rc;

    rc = tree_from_prufer(0, code_dummy, &t);
    ck_assert_int_ne(rc, 0);

    rc = tree_from_prufer(3, code_dummy, NULL);
    ck_assert_int_ne(rc, 0);
}
END_TEST


static Suite *tree_suite(void)
{
    Suite *s = suite_create("tree");

    
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_prufer_decode_small);
    tcase_add_test(tc_core, test_random_tree_edges_count);
    tcase_add_test(tc_core, test_prufer_example_5);
    tcase_add_test(tc_core, test_random_many);
    suite_add_tcase(s, tc_core);


    TCase *tc_edges = tcase_create("edges");
    tcase_add_test(tc_edges, test_prufer_n1);
    tcase_add_test(tc_edges, test_random_n2);
    tcase_add_test(tc_edges, test_prufer_invalid_value);
    tcase_add_test(tc_edges, test_bad_inputs);
    suite_add_tcase(s, tc_edges);

    return s;
}

int main(void)
{
    srand((unsigned)time(NULL));

    SRunner *sr = srunner_create(tree_suite());
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}