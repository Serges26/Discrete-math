#ifndef ITERATOR_H
#define ITERATOR_H


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Структура множества (битовая маска)
typedef struct {
    unsigned n;       // размер исходного множества {0,...,n-1}
    unsigned long long mask; // подмножество в виде битовой маски
} set_t;

// Итератор
typedef struct {
    set_t value;       // текущее подмножество
    unsigned long long state; // состояние итерации (номер маски)
    unsigned total;    // 2^n всего подмножеств
} iterator_t;

void iterator_init(iterator_t *i, unsigned n);
const set_t *iterator_value(const iterator_t *i);
int iterator_has_next(const iterator_t *i);
void iterator_next(iterator_t *i);
void iterator_destroy(iterator_t *i);

#endif
