#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdlib.h>

typedef struct {
    size_t *data;
    size_t size;
    size_t capacity;
} MinHeap;

MinHeap *heap_create(size_t capacity);
void heap_free(MinHeap *h);
int heap_push(MinHeap *h, size_t x);
size_t heap_pop(MinHeap *h);
int heap_empty(const MinHeap *h);

#endif
