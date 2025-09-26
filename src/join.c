#include "join.h"

struct Relation *relation_from_pairs(const struct Pair *pairs, unsigned n) {
    struct Relation *result = malloc(sizeof(*result));
    if (!result) return NULL;
    if (n == 0) {
        result->pairs = NULL;
        result->size = 0;
        return result;
    }
    result->pairs = malloc(n * sizeof(struct Pair));
    if (!result->pairs) { free(result); return NULL; }
    memcpy(result->pairs, pairs, n * sizeof(struct Pair));
    result->size = n;
    return result;
}

struct Relation *relation_empty(void) {
    struct Relation *result = malloc(sizeof(*result));
    if (!result) return NULL;
    result->pairs = NULL;
    result->size = 0;
    return result;
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

    if (r->size == 0 || s->size == 0) return relation_empty();

    unsigned cap = r->size * s->size;
    struct Pair *temp = malloc(cap * sizeof(struct Pair));
    if (!temp) return NULL;
    unsigned real_find = 0;

    for (unsigned i = 0; i < r->size; ++i) {
        int a = r->pairs[i].first;
        int b = r->pairs[i].second;
        for (unsigned j = 0; j < s->size; ++j) {
            if (s->pairs[j].first != b) continue;
            int c = s->pairs[j].second;
            bool found = false;
            for (unsigned k = 0; k < real_find; ++k) {
                if (temp[k].first == a && temp[k].second == c) { found = true; break; }
            }
            if (!found) {
                temp[real_find].first = a;
                temp[real_find].second = c;
                real_find++;
            }
        }
    }

    struct Relation *result = malloc(sizeof(*result));
    if (!result) { free(temp); return NULL; }
    if (real_find == 0) {
        free(temp);
        result->pairs = NULL;
        result->size = 0;
        return result;
    }

    struct Pair *del_unnec = realloc(temp, real_find * sizeof(struct Pair));
    if (!del_unnec) {
        result->pairs = temp;
        result->size = real_find;
        return result;
    }
    result->pairs = del_unnec;
    result->size = real_find;
    return result;
}
