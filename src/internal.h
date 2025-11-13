#ifndef INTERNAL_H
#define INTERNAL_H
#include <stddef.h>
#include <stdbool.h>

typedef struct Vec {
    size_t* a;
    size_t  n;
    size_t  cap;
} Vec;

static inline void vec_init(Vec* v){ v->a=NULL; v->n=0; v->cap=0; }
int  vec_push(Vec* v, size_t x);
bool vec_contains_alive(const Vec* v, const bool* alive, size_t x);
void vec_remove_first(Vec* v, size_t x);
void vec_free(Vec* v);

#endif
