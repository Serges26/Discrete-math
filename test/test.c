#include <stdlib.h>
#include <check.h>
#include "../include/graph.h"

// вспомогательная функция для удобства сборки графа
static Graph *make_graph(size_t n, const size_t *pairs, size_t pair_count) {
    size_t *u = malloc(pair_count * sizeof(size_t));
    size_t *v = malloc(pair_count * sizeof(size_t));
    ck_assert_ptr_nonnull(u);
    ck_assert_ptr_nonnull(v);

    for (size_t i = 0; i < pair_count; ++i) {
        u[i] = pairs[2 * i + 0];
        v[i] = pairs[2 * i + 1];
    }

    Graph *g = graph_create(n, u, v, pair_count);

    free(u);
    free(v);
    return g;
}

// Тест 1: путь (линия 0-1-2-3-4), диаметр = 4
START_TEST(test_line_graph) {
    size_t edges[] = {
        0,1,
        1,2,
        2,3,
        3,4,
    };
    Graph *g = make_graph(5, edges, 4);
    ck_assert_ptr_nonnull(g);

    size_t d = graph_diameter(g);
    ck_assert_uint_eq(d, 4);

    graph_destroy(g);
}
END_TEST

// Тест 2: звезда (0 центр, остальные листья), диаметр = 2
START_TEST(test_star_graph) {
    size_t edges[] = {
        0,1,
        0,2,
        0,3,
        0,4,
    };
    Graph *g = make_graph(5, edges, 4);
    ck_assert_ptr_nonnull(g);

    size_t d = graph_diameter(g);
    ck_assert_uint_eq(d, 2);

    graph_destroy(g);
}
END_TEST

// Тест 3: одна вершина без рёбер, диаметр = 0
START_TEST(test_single_vertex) {
    size_t edges[] = { };
    Graph *g = make_graph(1, edges, 0);
    ck_assert_ptr_nonnull(g);

    size_t d = graph_diameter(g);
    ck_assert_uint_eq(d, 0);

    graph_destroy(g);
}
END_TEST

// Тест 4: несвязный граф (0-1 и 2-3), graph_diameter() по нашей логике вернёт 0
START_TEST(test_disconnected_graph) {
    size_t edges[] = {
        0,1,
        2,3,
    };
    Graph *g = make_graph(4, edges, 2);
    ck_assert_ptr_nonnull(g);

    size_t d = graph_diameter(g);
    ck_assert_uint_eq(d, 0);

    graph_destroy(g);
}
END_TEST

// Тест 5: петли и кратные рёбра не ломают программу
// граф фактически: 0-1-2, диаметр = 2
START_TEST(test_loops_and_multiedges) {
    size_t edges[] = {
        0,0,  // петля, должна быть проигнорирована
        0,1,
        1,0,  // дубликат ребра
        1,2,
    };
    Graph *g = make_graph(3, edges, 4);
    ck_assert_ptr_nonnull(g);

    size_t d = graph_diameter(g);
    ck_assert_uint_eq(d, 2);

    graph_destroy(g);
}
END_TEST

// Тест 6: некорректный ввод (ребро с вершиной за пределами n)
// graph_create() должен вернуть NULL
START_TEST(test_bad_input) {
    size_t u[] = {0, 5};
    size_t v[] = {1, 6};
    Graph *g = graph_create(3, u, v, 2);
    ck_assert_ptr_null(g);
}
END_TEST

// Тест 7: большой линейный граф на 10000 вершин
// Диаметр должен быть n-1 = 9999
START_TEST(test_large_line) {
    const size_t n = 10000;
    size_t m = n - 1;

    size_t *u = malloc(m * sizeof(size_t));
    size_t *v = malloc(m * sizeof(size_t));
    ck_assert_ptr_nonnull(u);
    ck_assert_ptr_nonnull(v);

    for (size_t i = 0; i < m; ++i) {
        u[i] = i;
        v[i] = i + 1;
    }

    Graph *g = graph_create(n, u, v, m);
    ck_assert_ptr_nonnull(g);

    size_t d = graph_diameter(g);
    ck_assert_uint_eq(d, n - 1);

    graph_destroy(g);
    free(u);
    free(v);
}
END_TEST


Suite *graph_suite(void) {
    Suite *s = suite_create("Graph");

    TCase *tc_core = tcase_create("core");
    

    tcase_add_test(tc_core, test_line_graph);
    tcase_add_test(tc_core, test_star_graph);
    tcase_add_test(tc_core, test_single_vertex);
    tcase_add_test(tc_core, test_disconnected_graph);
    tcase_add_test(tc_core, test_loops_and_multiedges);
    tcase_add_test(tc_core, test_bad_input);
    tcase_add_test(tc_core, test_large_line);

    suite_add_tcase(s, tc_core);
    return s;
}


int main(void) {
    int failed = 0;
    Suite *s = graph_suite();
    SRunner *sr = srunner_create(s);

  
    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
