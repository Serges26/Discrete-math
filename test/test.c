#include <check.h>
#include <stdlib.h>
#include "coloring.h"


static void assert_proper_coloring(const Graph *g, const int *colors) {
    for (size_t i = 0; i < g->n; ++i) {
        ck_assert_int_ge(colors[i], 0);
        ck_assert_int_le(colors[i], 4);
        for (size_t j = 0; j < g->n; ++j) {
            if (i == j) continue;
            if (g->adj[i * g->n + j]) {
                ck_assert_int_ne(colors[i], colors[j]);
            }
        }
    }
}

/* Пустой граф  */
START_TEST(test_empty_graph) {
    Graph *g = graph_create(0);
    ck_assert_ptr_nonnull(g);

    int res = graph_five_color(g, NULL);
    ck_assert_int_eq(res, -1); /* неверные аргументы */

    graph_free(g);
}
END_TEST

/* 2. Одна вершина, нет рёбер */
START_TEST(test_single_vertex) {
    Graph *g = graph_create(1);
    ck_assert_ptr_nonnull(g);

    int colors[1];
    ck_assert_int_eq(graph_five_color(g, colors), 0);
    ck_assert_int_ge(colors[0], 0);
    ck_assert_int_le(colors[0], 4);

    graph_free(g);
}
END_TEST

START_TEST(test_path5) {
    Graph *g = graph_create(5);
    ck_assert_ptr_nonnull(g);

    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 1, 2);
    graph_add_edge(g, 2, 3);
    graph_add_edge(g, 3, 4);

    int colors[5];
    ck_assert_int_eq(graph_five_color(g, colors), 0);
    assert_proper_coloring(g, colors);

    graph_free(g);
}
END_TEST

START_TEST(test_cycle5) {
    Graph *g = graph_create(5);
    ck_assert_ptr_nonnull(g);

    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 1, 2);
    graph_add_edge(g, 2, 3);
    graph_add_edge(g, 3, 4);
    graph_add_edge(g, 4, 0);

    int colors[5];
    ck_assert_int_eq(graph_five_color(g, colors), 0);
    assert_proper_coloring(g, colors);

    graph_free(g);
}
END_TEST

/* 5. Полный граф K4, требует 4 цвета) */
START_TEST(test_k4) {
    Graph *g = graph_create(4);
    ck_assert_ptr_nonnull(g);

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = i + 1; j < 4; ++j)
            graph_add_edge(g, i, j);

    int colors[4];
    ck_assert_int_eq(graph_five_color(g, colors), 0);
    assert_proper_coloring(g, colors);

    graph_free(g);
}
END_TEST

START_TEST(test_wheel5) {
    Graph *g = graph_create(6);
    ck_assert_ptr_nonnull(g);

    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 1, 2);
    graph_add_edge(g, 2, 3);
    graph_add_edge(g, 3, 4);
    graph_add_edge(g, 4, 0);

    for (size_t i = 0; i < 5; ++i)
        graph_add_edge(g, 5, i);

    int colors[6];
    ck_assert_int_eq(graph_five_color(g, colors), 0);
    assert_proper_coloring(g, colors);

    graph_free(g);
}
END_TEST

START_TEST(test_bad_args) {
    Graph *g = graph_create(3);
    ck_assert_ptr_nonnull(g);

    int colors[3];
    ck_assert_int_eq(graph_five_color(NULL, colors), -1);
    ck_assert_int_eq(graph_five_color(g, NULL), -1);

    graph_free(g);
}
END_TEST

Suite *coloring_suite(void) {
    Suite *s = suite_create("coloring");
    TCase *tc = tcase_create("core");

    tcase_add_test(tc, test_empty_graph);
    tcase_add_test(tc, test_single_vertex);
    tcase_add_test(tc, test_path5);
    tcase_add_test(tc, test_cycle5);
    tcase_add_test(tc, test_k4);
    tcase_add_test(tc, test_wheel5);
    tcase_add_test(tc, test_bad_args);

    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    Suite *s = coloring_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int fails = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
