#include "src/join.h"

void print_relation(const struct Relation *r) {
    if (!r || r->size == 0) {
        printf("∅ (пустое отношение)\n");
        return;
    }
    printf("{ ");
    for (unsigned i = 0; i < r->size; i++) {
        printf("(%d,%d)", r->pairs[i].first, r->pairs[i].second);
        if (i + 1 < r->size) printf(", ");
    }
    printf(" }\n");
}

int main(void) {
    unsigned n, m;

  
    printf("Введите количество пар в отношении R: ");
    if (scanf("%u", &n) != 1) {
        printf("Ошибка ввода!\n");
        return 1;
    }
    struct Pair *pairs_r = malloc(n * sizeof(struct Pair));
    if (!pairs_r) {
        printf("Ошибка: не удалось выделить память\n");
        return 1;
    }
    for (unsigned i = 0; i < n; i++) {
        printf("Введите пару #%u (a b): ", i + 1);
        scanf("%d %d", &pairs_r[i].first, &pairs_r[i].second);
    }
    struct Relation *R = relation_from_pairs(pairs_r, n);
    free(pairs_r);


    printf("Введите количество пар в отношении S: ");
    if (scanf("%u", &m) != 1) {
        printf("Ошибка ввода!\n");
        relation_free(R);
        return 1;
    }
    struct Pair *pairs_s = malloc(m * sizeof(struct Pair));
    if (!pairs_s) {
        printf("Ошибка: не удалось выделить память\n");
        relation_free(R);
        return 1;
    }
    for (unsigned i = 0; i < m; i++) {
        printf("Введите пару #%u (a b): ", i + 1);
        scanf("%d %d", &pairs_s[i].first, &pairs_s[i].second);
    }
    struct Relation *S = relation_from_pairs(pairs_s, m);
    free(pairs_s);


    printf("\nОтношение R: ");
    print_relation(R);

    printf("Отношение S: ");
    print_relation(S);

    
    struct Relation *res = join_binary(R, S);
    printf("\nРезультат R∘S (join_binary): ");
    print_relation(res);


    relation_free(R);
    relation_free(S);
    relation_free(res);

    return 0;
}
