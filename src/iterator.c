#include <stdlib.h>
#include <string.h>
#include "iterator.h"

static void make_smallest(iterator_t *it) {
  // Самая маленькая по лексикографическому порядку: n '(' затем n ')'
  unsigned n = it->n;
  for (unsigned i = 0; i < n; ++i) it->value.data[i] = '(';
  for (unsigned i = 0; i < n; ++i) it->value.data[n + i] = ')';
  it->value.data[2 * n] = '\0';
  it->value.len = 2 * n;
  it->finished = 0;
}

// Построить следующую CBS в лексикографическом порядке.
// Если её нет — помечаем finished=1.
static void build_next(iterator_t *it) {
  if (it->finished) return;
  const unsigned n = it->n;
  const unsigned total = 2 * n;
  char *s = it->value.data;

  // Префиксные счётчики '(' и ')' для каждого индекса i (позиция i — до символа s[i])
  unsigned *pref_open = (unsigned*)calloc(total + 1, sizeof(unsigned));
  unsigned *pref_close = (unsigned*)calloc(total + 1, sizeof(unsigned));
  for (unsigned i = 0; i < total; ++i) {
    pref_open[i + 1]  = pref_open[i]  + (s[i] == '(');
    pref_close[i + 1] = pref_close[i] + (s[i] == ')');
  }

  int found = 0;
  // Ищем справа позицию i, где можно заменить ( -> )
  for (int i = (int)total - 1; i >= 0; --i) {
    if (s[i] != '(') continue;

    unsigned opens_before  = pref_open[i];
    unsigned closes_before = pref_close[i];

    // После переворота: '(' убирается, ставим ')'
    unsigned opens  = opens_before;          // так как вместо '(' теперь не открываем
    unsigned closes = closes_before + 1;

    if (closes > opens) continue; 
    if (opens > n || closes > n) continue;

    // Приняли позицию i
    s[i] = ')';

    // Минимальный суффикс: все оставшиеся '(' затем все ')'
    unsigned opens_left  = n - opens;
    unsigned closes_left = n - closes;

    unsigned k = i + 1;
    for (unsigned t = 0; t < opens_left; ++t) s[k++] = '(';
    for (unsigned t = 0; t < closes_left; ++t) s[k++] = ')';
    s[total] = '\0';

    found = 1;
    break;
  }

  free(pref_open);
  free(pref_close);

  if (!found) it->finished = 1;
}

void iterator_init(iterator_t *it, unsigned n) {
  memset(it, 0, sizeof(*it));
  it->n = n;
  it->value.len = 2 * n;
  it->value.data = (char*)malloc(it->value.len + 1);
  it->choice = NULL;   
  it->pos = it->open = it->close = 0;

  if (n == 0) {
    it->value.data[0] = '\0';
    it->finished = 0;
  } else {
    make_smallest(it);
  }
}

const cbs_t *iterator_value(const iterator_t *it) {
  if (!it || it->finished) return NULL;
  return &it->value;
}

int iterator_has_next(const iterator_t *it) {
  return (it && !it->finished) ? 1 : 0;
}

void iterator_next(iterator_t *it) {
  if (!it || it->finished) return;
  if (it->n == 0) { it->finished = 1; return; }
  build_next(it);
}

void iterator_destroy(iterator_t *it) {
  if (!it) return;
  free(it->value.data);
  free(it->choice); 
  memset(it, 0, sizeof(*it));
}
