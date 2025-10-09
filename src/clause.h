#ifndef CLAUSE_H
#define CLAUSE_H


#include <stdlib.h>
#include <limits.h>

typedef struct Clause {
    int i, j, k;
} clause_t;

int is_sat(const clause_t *clauses, unsigned size);

#endif
