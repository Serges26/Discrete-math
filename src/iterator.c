#include "iterator.h"

// создать пустой список битов длиной n
static bit_node_t* create_bit_list(unsigned n) {
    bit_node_t *head = NULL, *tail = NULL;
    for (unsigned i = 0; i < n; i++) {
        bit_node_t *node = (bit_node_t*)malloc(sizeof(bit_node_t));
        node->bit = 0;
        node->next = NULL;
        if (!head) head = node;
        else tail->next = node;
        tail = node;
    }
    return head;
}

// обновить список битов по state (номер маски)
static void update_mask_from_state(bit_node_t *mask, unsigned long long state, unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        mask->bit = (state >> i) & 1ULL;
        mask = mask->next;
    }
}

void iterator_init(iterator_t *i, unsigned n) {
    if (!i) return;
    i->value.n = n;
    i->value.mask = create_bit_list(n);
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
        update_mask_from_state(i->value.mask, i->state, i->value.n);
        i->state++;
    }
}

void iterator_destroy(iterator_t *i) {
    if (!i) return;
    bit_node_t *cur = i->value.mask;
    while (cur) {
        bit_node_t *next = cur->next;
        free(cur);
        cur = next;
    }
    free(i);
}
