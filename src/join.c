#include "join.h"
#include <string.h>

struct Relation *relation_from_pairs(const struct Pair *pairs, unsigned n) {
    struct Relation *res = malloc(sizeof(*res));
    if (!res) return NULL;
    if (n == 0) {
        res->pairs = NULL;
        res->size = 0;
        return res;
    }
    res->pairs = malloc(n * sizeof(struct Pair));
    if (!res->pairs) { free(res); return NULL; }
    memcpy(res->pairs, pairs, n * sizeof(struct Pair));
    res->size = n;
    return res;
}

struct Relation *relation_empty(void) {
    struct Relation *res = malloc(sizeof(*res));
    if (!res) return NULL;
    res->pairs = NULL;
    res->size = 0;
    return res;
}

void relation_free(struct Relation *r) {
    if (!r) return;
    free(r->pairs);
    free(r);
}

bool relation_contains(const struct Relation *r, struct Pair p) {
    if (!r) return false;
    for (unsigned i = 0; i < r->size; ++i) {
        if (r->pairs[i].first == p.first && r->pairs[i].second == p.second) return true;
    }
    return false;
}

struct Relation *join_binary(struct Relation *r, struct Relation *s) {
    if (!r || !s) return NULL;

    /* if either is empty, result is empty */
    if (r->size == 0 || s->size == 0) return relation_empty();

    /* worst-case capacity */
    unsigned cap = r->size * s->size;
    struct Pair *tmp = malloc(cap * sizeof(struct Pair));
    if (!tmp) return NULL;
    unsigned out = 0;

    for (unsigned i = 0; i < r->size; ++i) {
        int a = r->pairs[i].first;
        int b = r->pairs[i].second;
        for (unsigned j = 0; j < s->size; ++j) {
            if (s->pairs[j].first != b) continue;
            int c = s->pairs[j].second;
            bool found = false;
            for (unsigned k = 0; k < out; ++k) {
                if (tmp[k].first == a && tmp[k].second == c) { found = true; break; }
            }
            if (!found) {
                tmp[out].first = a;
                tmp[out].second = c;
                out++;
            }
        }
    }

    struct Relation *res = malloc(sizeof(*res));
    if (!res) { free(tmp); return NULL; }
    if (out == 0) {
        free(tmp);
        res->pairs = NULL;
        res->size = 0;
        return res;
    }

    struct Pair *sh = realloc(tmp, out * sizeof(struct Pair));
    if (!sh) {
        res->pairs = tmp;
        res->size = out;
        return res;
    }
    res->pairs = sh;
    res->size = out;
    return res;
}
