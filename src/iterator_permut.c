#include "iter.h"

void iterator_init(iterator_t *i, unsigned n) {
    i->n = n;
    i->value.size = n;
    i->value.elements = malloc(n * sizeof(unsigned));
    i->directions = malloc(n * sizeof(int));
    i->has_next = 1;

    for (unsigned j = 0; j < n; j++) {
        i->value.elements[j] = j;
        i->directions[j] = -1; // Все смотрят влево
    }
}

const permutation_t *iterator_value(const iterator_t *i) {
    return &i->value;
}

int iterator_has_next(const iterator_t *i) {
    return i->has_next;
}


int is_mobile(iterator_t *i, unsigned pos) {
    if (i->directions[pos] == -1 && pos > 0) {
        return i->value.elements[pos] > i->value.elements[pos - 1];
    }
    if (i->directions[pos] == 1 && pos < i->n - 1) {
        return i->value.elements[pos] > i->value.elements[pos + 1];
    }
    return 0;
}

void iterator_next(iterator_t *i) {
    int mob_pos = -1;
    unsigned max_val = 0;
    
    for (unsigned j = 0; j < i->n; j++) {
        if (is_mobile(i, j)) {
            if (mob_pos == -1 || i->value.elements[j] > max_val) {
                mob_pos = j;
                max_val = i->value.elements[j];
            }
        }
    }
    
    if (mob_pos == -1) {
        i->has_next = 0;
        return;
    }
    

    int swap_pos = mob_pos + i->directions[mob_pos];
    

    unsigned temp_elem = i->value.elements[mob_pos];
    i->value.elements[mob_pos] = i->value.elements[swap_pos];
    i->value.elements[swap_pos] = temp_elem;
    

    int temp_dir = i->directions[mob_pos];
    i->directions[mob_pos] = i->directions[swap_pos];
    i->directions[swap_pos] = temp_dir;
    
  
    unsigned moved_val = i->value.elements[swap_pos];
    for (unsigned j = 0; j < i->n; j++) {
        if (i->value.elements[j] > moved_val) {
            i->directions[j] = -i->directions[j];
        }
    }
}

void iterator_destroy(iterator_t *i) {
    free(i->value.elements);
    free(i->directions);
}



