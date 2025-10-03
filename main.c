#include "src/iter.h"


void print_permutation(const permutation_t *p) {
    printf("[");
    for (unsigned i = 0; i < p->size; i++) {
        printf("%u", p->elements[i]);
        if (i < p->size - 1) printf(", ");
    }
    printf("]");
}

int main() {
    unsigned n;
    printf("Введите n: ");
    scanf("%d", &n);
    
    iterator_t iter;
    iterator_init(&iter, n);
    
    do {
        const permutation_t *p = iterator_value(&iter);
        print_permutation(p);
        printf("\n");
        
        iterator_next(&iter);
    } while (iterator_has_next(&iter));
    
    iterator_destroy(&iter);
    return 0;
}