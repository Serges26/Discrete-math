#ifndef ITERATOR_H
#define ITERATOR_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Узел для списка (бит)
typedef struct bit_node {
    int bit;                 // 0 или 1
    struct bit_node *next;   // следующий бит
} bit_node_t;

// Структура множества (битовая маска в виде списка)
typedef struct {
    unsigned n;        // размер исходного множества {0,...,n-1}
    bit_node_t *mask;  // список битов длиной n
} set_t;

// Итератор
typedef struct {
    set_t value;               // текущее подмножество
    unsigned long long state;  // состояние итерации (номер маски)
    unsigned long long total;  // всего 2^n подмножеств
} iterator_t;

void iterator_init(iterator_t *i, unsigned n);
const set_t *iterator_value(const iterator_t *i);
int iterator_has_next(const iterator_t *i);
void iterator_next(iterator_t *i);
void iterator_destroy(iterator_t *i);

#endif
