#ifndef JOIN_H
#define JOIN_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Pair { int first; int second; };
struct Relation { struct Pair *pairs; unsigned size; };

/* Создать отношение, скопировав `pairs[0..n-1]` */
struct Relation *relation_from_pairs(const struct Pair *pairs, unsigned n);

/* Создать пустое отношение (size == 0) */
struct Relation *relation_empty(void);

/* Освободить память отношения, созданного с помощью relation_from_pairs или relation_empty */
void relation_free(struct Relation *r);

/* Проверить, содержит ли отношение указанную пару */
bool relation_contains(const struct Relation *r, struct Pair p);

/* Композиция двух бинарных отношений: вернуть новое отношение T = r ∘ s такое, что
(a,c) ∈ T тогда и только тогда, когда ∃ b: (a,b) ∈ r и (b,c) ∈ s
Если r или s равны NULL, вернуть NULL. */
struct Relation *join_binary(struct Relation *r, struct Relation *s);

#endif
 