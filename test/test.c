#include <check.h>
#include "../src/iterator.h"


START_TEST(test_init) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 3);

    ck_assert_int_eq(it->value.n, 3);
    ck_assert_int_eq(it->value.mask, 0);
    ck_assert_int_eq(it->state, 0);
    ck_assert_int_eq(it->total, (1U << 3));

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

    ck_assert_int_eq(count, (1U << 2)); // должно быть 4 подмножества

    iterator_destroy(it);
}
END_TEST

// Тест правильности масок
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
        ck_assert_uint_eq(s->mask, expected_masks[idx]);
        idx++;
    }

    ck_assert_int_eq(idx, (1U << 3));

    iterator_destroy(it);
}
END_TEST

// Тест destroy
START_TEST(test_destroy) {
    iterator_t *it = malloc(sizeof(iterator_t));
    ck_assert_ptr_nonnull(it);

    iterator_init(it, 4);
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
