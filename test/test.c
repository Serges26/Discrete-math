#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../src/iterator.h"

static int is_balanced(const char *s) {
  int bal = 0;
  for (const char *p = s; *p; ++p) {
    if (*p == '(') ++bal;
    else if (*p == ')') --bal;
    else return 0;
    if (bal < 0) return 0;
  }
  return bal == 0;
}

static unsigned long catalan(unsigned n) {
  unsigned long *C = calloc(n + 1, sizeof(unsigned long));
  C[0] = 1;
  for (unsigned i = 1; i <= n; ++i) {
    unsigned long s = 0;
    for (unsigned k = 0; k < i; ++k) s += C[k] * C[i - 1 - k];
    C[i] = s;
  }
  unsigned long ans = C[n];
  free(C);
  return ans;
}

START_TEST(test_iterator_n) {
  unsigned n = _i;
  iterator_t it;
  iterator_init(&it, n);

  unsigned long count = 0;
  char *prev = NULL;

  while (iterator_has_next(&it)) {
    const cbs_t *v = iterator_value(&it);
    ck_assert_ptr_nonnull(v);
    ck_assert_uint_eq(v->len, 2 * n);
    ck_assert_uint_eq(strlen(v->data), 2 * n);
    ck_assert(is_balanced(v->data));

    if (prev) ck_assert_msg(strcmp(prev, v->data) < 0, "Not lexicographically increasing");
    free(prev);
    prev = strdup(v->data);

    ++count;
    iterator_next(&it);
  }
  free(prev);
  ck_assert_uint_eq(count, catalan(n));
  ck_assert_ptr_null(iterator_value(&it));
  iterator_destroy(&it);
}
END_TEST

START_TEST(test_destroy_direct) {
  iterator_t it;
  iterator_init(&it, 2);
  iterator_destroy(&it);
}
END_TEST

Suite *iterator_suite(void) {
  Suite *s = suite_create("CBS Iterator");
  TCase *tc = tcase_create("Core");
  tcase_add_loop_test(tc, test_iterator_n, 0, 5);
  tcase_add_test(tc, test_destroy_direct);
  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  Suite *s = iterator_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_ENV);
  int nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
