#ifndef JOIN_H
#define JOIN_H

#include <stdlib.h>
#include <stdbool.h>

struct Pair { int first; int second; };
struct Relation { struct Pair *pairs; unsigned size; };

/* Create a relation by copying `pairs[0..n-1]` */
struct Relation *relation_from_pairs(const struct Pair *pairs, unsigned n);

/* Create an empty relation (size == 0) */
struct Relation *relation_empty(void);

/* Free a relation created by relation_from_pairs or relation_empty */
void relation_free(struct Relation *r);

/* Check whether relation contains specific pair */
bool relation_contains(const struct Relation *r, struct Pair p);

/* Compose two binary relations: return new relation T = r ∘ s such that
(a,c) ∈ T iff ∃ b: (a,b) ∈ r and (b,c) ∈ s
If r or s is NULL, returns NULL. */
struct Relation *join_binary(struct Relation *r, struct Relation *s);

#endif // JOIN_H