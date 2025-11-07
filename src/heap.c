#include "../include/heap.h"


static void swap_size_t(size_t *a, size_t *b) {
    size_t t = *a; *a = *b; *b = t;
}

MinHeap *heap_create(size_t capacity) {
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->data = (size_t *)malloc((capacity + 1) * sizeof(size_t));
    if (!h->data) {
        free(h);
        return NULL;
    }
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void heap_free(MinHeap *h) {
    if (h) {
        free(h->data);
        free(h);
    }
}

int heap_push(MinHeap *h, size_t x) {
    if (h->size == h->capacity) return -1;
    h->data[++h->size] = x;
    size_t i = h->size;
    while (i > 1) {
        size_t p = i / 2;
        if (h->data[p] <= h->data[i]) break;
        swap_size_t(&h->data[p], &h->data[i]);
        i = p;
    }
    return 0;
}

size_t heap_pop(MinHeap *h) {
    size_t res = h->data[1];
    h->data[1] = h->data[h->size--];
    size_t i = 1;
    for (;;) {
        size_t l = 2 * i;
        size_t r = 2 * i + 1;
        size_t smallest = i;
        if (l <= h->size && h->data[l] < h->data[smallest]) smallest = l;
        if (r <= h->size && h->data[r] < h->data[smallest]) smallest = r;
        if (smallest == i) break;
        swap_size_t(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return res;
}

int heap_empty(const MinHeap *h) {
    return h->size == 0;
}
