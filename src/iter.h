#ifndef ITER_H
#define ITER_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Permutation {
    unsigned *elements;
    unsigned size;
} permutation_t;

typedef struct PermutationIterator {
    permutation_t value;
    unsigned n;
    int *directions;
    int has_next;
} iterator_t;

void iterator_init(iterator_t *i, unsigned n);
const permutation_t *iterator_value(const iterator_t *i);
int iterator_has_next(const iterator_t *i);
void iterator_next(iterator_t *i);
void iterator_destroy(iterator_t *i);


int is_mobile(iterator_t *i, unsigned pos);


#endif