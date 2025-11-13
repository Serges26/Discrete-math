#include <check.h>
#include <stdlib.h>
#include "graph.h"

START_TEST(path3_suppresses_to_edge){
    size_t u[]={0,1};
    size_t v[]={1,2};
    Graph* g = graph_from_edges(3, u, v, 2);
    ck_assert_ptr_nonnull(g);
    ck_assert_uint_eq(graph_degree(g,1), 2);
    ck_assert_uint_eq(graph_simplify(g), 1);
    ck_assert(!graph_alive(g,1));
    ck_assert(graph_has_edge(g,0,2));
    ck_assert_uint_eq(graph_edge_count(g), 1);
    graph_free(g);
}
END_TEST

START_TEST(path4_suppresses_chain){
    size_t u[]={0,1,2};
    size_t v[]={1,2,3};
    Graph* g = graph_from_edges(4, u, v, 3);
    ck_assert_uint_eq(graph_simplify(g), 2);
    ck_assert(!graph_alive(g,1));
    ck_assert(!graph_alive(g,2));
    ck_assert(graph_has_edge(g,0,3));
    ck_assert_uint_eq(graph_edge_count(g), 1);
    graph_free(g);
}
END_TEST

START_TEST(triangle_parallel_edge_expected){
    size_t u[]={0,1,2};
    size_t v[]={1,2,0};
    Graph* g = graph_from_edges(3, u, v, 3);
    ck_assert_uint_eq(graph_simplify(g), 2);
    graph_free(g);
}
END_TEST

 START_TEST(loop_case_u_eq_w){
    Graph* g = graph_create(3);
    graph_add_edge(g, 2, 0);
    graph_add_edge(g, 2, 0);

    ck_assert_uint_eq(graph_degree(g,2), 2);

    ck_assert_uint_eq(graph_simplify(g), 1);

    bool loop_on_0 = graph_has_edge(g, 0, 0);
    bool loop_on_2 = graph_has_edge(g, 2, 2);
    ck_assert(loop_on_0 || loop_on_2);

    graph_free(g);
}
END_TEST



Suite* graph_suite(void){
    Suite* s = suite_create("graph");
    TCase* tc = tcase_create("core");
    tcase_add_test(tc, path3_suppresses_to_edge);
    tcase_add_test(tc, path4_suppresses_chain);
    tcase_add_test(tc, triangle_parallel_edge_expected);
    tcase_add_test(tc, loop_case_u_eq_w);
    suite_add_tcase(s, tc);
    return s;
}

int main(void){
    int number_failed;
    Suite* s = graph_suite();
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
