#include <check.h>
#include "../src/iterator.h"

// Вспомогательная функция: преобразует список битов в число (для удобной проверки)
static unsigned long long mask_to_ull(const set_t *s) {
    unsigned long long result = 0ULL;
    bit_node_t *cur = s->mask;
    unsigned j = 0;
    while (cur) {
        if (cur->bit == 1) {
            result |= (1ULL << j);
        }
        cur = cur->next;
        j++;
    }
    return result;
}



START_TEST(test_init) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 3);

    ck_assert_int_eq(it->value.n, 3);
    ck_assert_int_eq(mask_to_ull(&it->value), 0ULL);
    ck_assert_int_eq(it->state, 0ULL);
    ck_assert_int_eq(it->total, (1ULL << 3));

    iterator_destroy(it);
}
END_TEST

START_TEST(test_iteration) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 2);

    unsigned count = 0;
    while (iterator_has_next(it)) {
        iterator_next(it);
        const set_t *s = iterator_value(it);
        ck_assert_ptr_nonnull(s);
        count++;
    }

    ck_assert_int_eq(count, (1U << 2));

    iterator_destroy(it);
}
END_TEST

START_TEST(test_masks) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 3);

    unsigned long long expected_masks[] = {
        0b000, 0b001, 0b010, 0b011,
        0b100, 0b101, 0b110, 0b111
    };

    unsigned idx = 0;
    while (iterator_has_next(it)) {
        iterator_next(it);
        const set_t *s = iterator_value(it);
        unsigned long long mask_val = mask_to_ull(s);
        ck_assert_uint_eq(mask_val, expected_masks[idx]);
        idx++;
    }

    ck_assert_int_eq(idx, (1U << 3));

    iterator_destroy(it);
}
END_TEST

START_TEST(test_destroy) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 4);
    iterator_destroy(it);
}
END_TEST




START_TEST(test_init_null) {
    iterator_init(NULL, 5);
}
END_TEST


START_TEST(test_destroy_null) {
    iterator_destroy(NULL); 
}
END_TEST


START_TEST(test_next_after_end) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 1);

   
    iterator_next(it);
    iterator_next(it);

    
    unsigned long long before = mask_to_ull(&it->value);
    iterator_next(it);
    unsigned long long after = mask_to_ull(&it->value);

    ck_assert_uint_eq(before, after);

    iterator_destroy(it);
}
END_TEST


START_TEST(test_mask_content) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 4);

   
    iterator_next(it);
    ck_assert_uint_eq(mask_to_ull(&it->value), 0b0000);

    iterator_next(it);
    ck_assert_uint_eq(mask_to_ull(&it->value), 0b0001);

    iterator_next(it);
    ck_assert_uint_eq(mask_to_ull(&it->value), 0b0010);

    iterator_destroy(it);
}
END_TEST


Suite *iterator_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Iterator");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_iteration);
    tcase_add_test(tc_core, test_masks);
    tcase_add_test(tc_core, test_destroy);
    tcase_add_test(tc_core, test_init_null);
    tcase_add_test(tc_core, test_destroy_null);
    tcase_add_test(tc_core, test_next_after_end);
    tcase_add_test(tc_core, test_mask_content);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = iterator_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
