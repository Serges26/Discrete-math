#include <check.h>
#include <stdlib.h>
#include "../src/iter.h"

// Вспомогательная функция для сравнения перестановок
int permutations_equal(const permutation_t *a, const permutation_t *b) {
    if (a->size != b->size) return 0;
    for (unsigned i = 0; i < a->size; i++) {
        if (a->elements[i] != b->elements[i]) return 0;
    }
    return 1;
}


START_TEST(test_iterator_init) {
    iterator_t iter;
    iterator_init(&iter, 3);
    
   
    const permutation_t *p = iterator_value(&iter);
    ck_assert_uint_eq(p->size, 3);
    ck_assert_uint_eq(p->elements[0], 0);
    ck_assert_uint_eq(p->elements[1], 1);
    ck_assert_uint_eq(p->elements[2], 2);
    
    ck_assert_int_eq(iterator_has_next(&iter), 1);
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_n1_permutations) {
    iterator_t iter;
    iterator_init(&iter, 1);
    
  
    const permutation_t *p1 = iterator_value(&iter);
    ck_assert_uint_eq(p1->size, 1);
    ck_assert_uint_eq(p1->elements[0], 0);
    
    ck_assert_int_eq(iterator_has_next(&iter), 1);
    iterator_next(&iter);
    ck_assert_int_eq(iterator_has_next(&iter), 0);
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_n2_permutations) {
    iterator_t iter;
    iterator_init(&iter, 2);
    
    permutation_t expected1 = {.size = 2, .elements = (unsigned[]){0, 1}};
    permutation_t expected2 = {.size = 2, .elements = (unsigned[]){1, 0}};
    

    const permutation_t *p1 = iterator_value(&iter);
    ck_assert(permutations_equal(p1, &expected1));
    
 
    iterator_next(&iter);
    const permutation_t *p2 = iterator_value(&iter);
    ck_assert(permutations_equal(p2, &expected2));
    

    iterator_next(&iter);
    ck_assert_int_eq(iterator_has_next(&iter), 0);
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_n3_count) {
    iterator_t iter;
    iterator_init(&iter, 3);
    
    int count = 0;
    do {
        const permutation_t *p = iterator_value(&iter);
        ck_assert_uint_eq(p->size, 3);
        count++;
        iterator_next(&iter);
    } while (iterator_has_next(&iter));
    
    ck_assert_int_eq(count, 6); // 3! = 6
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_n3_unique) {
    iterator_t iter;
    iterator_init(&iter, 3);
    
    unsigned seen[6][3];
    int count = 0;
    

    do {
        const permutation_t *p = iterator_value(&iter);
        for (int i = 0; i < 3; i++) {
            seen[count][i] = p->elements[i];
        }
        count++;
        iterator_next(&iter);
    } while (iterator_has_next(&iter));
    

    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 6; j++) {
            int different = 0;
            for (int k = 0; k < 3; k++) {
                if (seen[i][k] != seen[j][k]) {
                    different = 1;
                    break;
                }
            }
            ck_assert(different); 
        }
    }
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_is_mobile) {
    iterator_t iter;
    iterator_init(&iter, 3);
    
    // Начальное состояние: [0←, 1←, 2←]
    // Проверяем мобильность каждого элемента
    ck_assert_int_eq(is_mobile(&iter, 0), 0); // 0← не мобилен (нет соседа слева)
    ck_assert_int_eq(is_mobile(&iter, 1), 1); // 1← мобилен (1 > 0)
    ck_assert_int_eq(is_mobile(&iter, 2), 1); // 2← мобилен (2 > 1)
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_n0) {
    iterator_t iter;
    iterator_init(&iter, 0);
    
    const permutation_t *p = iterator_value(&iter);
    ck_assert_uint_eq(p->size, 0);
    ck_assert_int_eq(iterator_has_next(&iter), 1);
    
    iterator_next(&iter);
    ck_assert_int_eq(iterator_has_next(&iter), 0);
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_multiple_value_calls) {
    iterator_t iter;
    iterator_init(&iter, 2);
    
    const permutation_t *p1 = iterator_value(&iter);
    const permutation_t *p2 = iterator_value(&iter);
    
  
    ck_assert_ptr_eq(p1, p2);
    ck_assert(permutations_equal(p1, p2));
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_next_after_end) {
    iterator_t iter;
    iterator_init(&iter, 1);
    
  
    iterator_value(&iter);
    
   
    iterator_next(&iter);
    ck_assert_int_eq(iterator_has_next(&iter), 0);
    

    iterator_next(&iter);
    ck_assert_int_eq(iterator_has_next(&iter), 0);
    
    iterator_destroy(&iter);
}
END_TEST


START_TEST(test_direction_changes) {
    iterator_t iter;
    iterator_init(&iter, 3);
    
 
    ck_assert_int_eq(iter.directions[0], -1);
    ck_assert_int_eq(iter.directions[1], -1);
    ck_assert_int_eq(iter.directions[2], -1);
    
 
    iterator_next(&iter); // [0, 2, 1]
    
    ck_assert_int_eq(iter.directions[0], -1);
    ck_assert_int_eq(iter.directions[1], -1);
    ck_assert_int_eq(iter.directions[2], -1);
    
    iterator_next(&iter); // [2, 0, 1]
    
    ck_assert_int_eq(iter.directions[0], -1);
    ck_assert_int_eq(iter.directions[1], -1);
    ck_assert_int_eq(iter.directions[2], -1);
    
    iterator_destroy(&iter);
}
END_TEST


Suite *iter_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("PermutationIterator");
    tc_core = tcase_create("Core");

   
    tcase_add_test(tc_core, test_iterator_init);
    tcase_add_test(tc_core, test_n1_permutations);
    tcase_add_test(tc_core, test_n2_permutations);
    tcase_add_test(tc_core, test_n3_count);
    tcase_add_test(tc_core, test_n3_unique);
    tcase_add_test(tc_core, test_is_mobile);
    tcase_add_test(tc_core, test_n0);
    tcase_add_test(tc_core, test_multiple_value_calls);
    tcase_add_test(tc_core, test_next_after_end);
    tcase_add_test(tc_core, test_direction_changes);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = iter_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}