#include "src/iterator.h"



void set_print(const set_t *s) {
    printf("{ ");
    for (unsigned j = 0; j < s->n; j++) {
        if (s->mask & (1ULL << j)) {
            printf("%u ", j);
        }
    }
    printf("}");
}

int main() {
    unsigned n;

    printf("Введите число n: ");
    if (scanf("%u", &n) != 1) {
        printf("Ошибка ввода!\n");
        return 1;
    }

    iterator_t *it = malloc(sizeof(iterator_t));
    if (!it) {
        printf("Ошибка: не удалось выделить память\n");
        return 1;
    }

    iterator_init(it, n);

    printf("Все подмножества множества {0, 1, ..., %u}:\n", n - 1);

    while (iterator_has_next(it)) {
        iterator_next(it);
        const set_t *s = iterator_value(it);
        set_print(s);
        printf("\n");
    }

    iterator_destroy(it);
    return 0;
}
