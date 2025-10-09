#include "clause.h"

static int razmer_max_index(const clause_t *klauzy, unsigned razmer) {
    int max_index = 0;
    for (unsigned idx = 0; idx < razmer; ++idx) {
        int a = klauzy[idx].i >= 0 ? klauzy[idx].i : -klauzy[idx].i;
        int b = klauzy[idx].j >= 0 ? klauzy[idx].j : -klauzy[idx].j;
        int c = klauzy[idx].k >= 0 ? klauzy[idx].k : -klauzy[idx].k;
        if (a > max_index) max_index = a;
        if (b > max_index) max_index = b;
        if (c > max_index) max_index = c;
    }
    return max_index;
}

static int ocenit_literal(int lit, const int *peremennaya) {
    int idx = lit >= 0 ? lit : -lit;
    int znach = peremennaya[idx];
    if (znach == 0) return -1; 
    int prod = (lit >= 0) ? znach : -znach;
    return (prod == 1) ? 1 : 0;
}


static int ocenit_klauzu(const clause_t *cl, const int *peremennaya) {
    int resi = ocenit_literal(cl->i, peremennaya);
    if (resi == 1) return 1;
    int resj = ocenit_literal(cl->j, peremennaya);
    if (resj == 1) return 1;
    int resk = ocenit_literal(cl->k, peremennaya);
    if (resk == 1) return 1;
    
    if (resi == -1 || resj == -1 || resk == -1) return -1;
    return 0;
}


static int poisk_rekursivnyi(int var_idx, int n, const clause_t *klauzy, unsigned razmer, int *peremennaya) {
    
    for (unsigned ci = 0; ci < razmer; ++ci) {
        int oc = ocenit_klauzu(&klauzy[ci], peremennaya);
        if (oc == 0) return 0;
    }
    if (var_idx > n) return 1;

    peremennaya[var_idx] = 1;
    if (poisk_rekursivnyi(var_idx + 1, n, klauzy, razmer, peremennaya)) return 1;
    peremennaya[var_idx] = -1;
    if (poisk_rekursivnyi(var_idx + 1, n, klauzy, razmer, peremennaya)) return 1;
    peremennaya[var_idx] = 0;
    return 0;
}

int is_sat(const clause_t *clauses, unsigned size) {
    if (clauses == NULL || size == 0) return 1;
    int n = razmer_max_index(clauses, size);
    if (n <= 0) return 1;
    
    int *peremennaya = (int *)calloc((size_t)n + 1, sizeof(int));
    if (peremennaya == NULL) return 0;
    
 
    int result = poisk_rekursivnyi(1, n, clauses, size, peremennaya);
    free(peremennaya);
    return result;
}
