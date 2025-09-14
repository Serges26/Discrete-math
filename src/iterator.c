#include "iterator.h"


void iterator_init(iterator_t *i, unsigned n) {
    if (!i) return;

    i->value.n = n;
    i->value.mask = 0ULL;
    i->state = 0ULL;
    i->total = 1ULL << n;  // всего 2^n подмножеств
}


const set_t *iterator_value(const iterator_t *i) {
    return &i->value;
}


int iterator_has_next(const iterator_t *i) {
    return i->state < i->total;
}


void iterator_next(iterator_t *i) {
    if (i->state < i->total) {
        i->value.mask = i->state;
        i->state++;
    }
}

void iterator_destroy(iterator_t *i) {
    free(i);
}
