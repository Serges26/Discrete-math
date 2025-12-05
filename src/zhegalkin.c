#include "zhegalkin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} StrBuf;

static int sb_init(StrBuf *b) {
    b->cap = 64;
    b->len = 0;
    b->data = (char *)malloc(b->cap);
    if (!b->data) return -1;
    b->data[0] = '\0';
    return 0;
}

static int sb_reserve(StrBuf *b, size_t need) {
    if (need <= b->cap) return 0;
    size_t new_cap = b->cap;
    while (new_cap < need) {
        new_cap *= 2;
    }
    char *tmp = (char *)realloc(b->data, new_cap);
    if (!tmp) return -1;
    b->data = tmp;
    b->cap = new_cap;
    return 0;
}

static int sb_append_char(StrBuf *b, char c) {
    if (sb_reserve(b, b->len + 2) != 0) return -1;
    b->data[b->len++] = c;
    b->data[b->len] = '\0';
    return 0;
}

static int sb_append_str(StrBuf *b, const char *s) {
    size_t sl = strlen(s);
    if (sb_reserve(b, b->len + sl + 1) != 0) return -1;
    memcpy(b->data + b->len, s, sl + 1);
    b->len += sl;
    return 0;
}

int zhegalkin_from_truth(const int *f, size_t nvars, ZhegalkinAnf *out) {
    if (!f || !out) return -1;

    size_t max_nvars = sizeof(size_t) * 8u - 1u; // ограничение по размеру
    if (nvars > max_nvars) return -2;

    size_t size = (nvars == 0) ? 1u : ((size_t)1u << nvars);

    unsigned char *a = (unsigned char *)malloc(size);
    if (!a) return -3;

    for (size_t i = 0; i < size; ++i) {
        a[i] = (f[i] != 0) ? 1u : 0u;
    }

    for (size_t k = 0; k < nvars; ++k) {
        size_t bit = (size_t)1u << k;
        for (size_t mask = 0; mask < size; ++mask) {
            if (mask & bit) {
                a[mask] ^= a[mask ^ bit];
            }
        }
    }

    out->nvars = nvars;
    out->size = size;
    out->coeffs = a;
    return 0;
}

void zhegalkin_free(ZhegalkinAnf *anf) {
    if (!anf) return;
    free(anf->coeffs);
    anf->coeffs = NULL;
    anf->nvars = 0;
    anf->size = 0;
}

char *zhegalkin_to_string(const ZhegalkinAnf *anf) {
    if (!anf || !anf->coeffs) return NULL;

    StrBuf b;
    if (sb_init(&b) != 0) return NULL;

    int first_term = 1;
    int has_term = 0;

    for (size_t mask = 0; mask < anf->size; ++mask) {
        if (!anf->coeffs[mask])
            continue;

        has_term = 1;

        if (!first_term) {
            if (sb_append_str(&b, " + ") != 0) {
                free(b.data);
                return NULL;
            }
        }
        first_term = 0;

        if (mask == 0) {
            
            if (sb_append_char(&b, '1') != 0) {
                free(b.data);
                return NULL;
            }
        } else {
            
            for (size_t v = 0; v < anf->nvars; ++v) {
                if (mask & ((size_t)1u << v)) {
                    if (sb_append_char(&b, 'x') != 0) {
                        free(b.data);
                        return NULL;
                    }
                    char buf[32];
                    snprintf(buf, sizeof(buf), "%zu", v);
                    if (sb_append_str(&b, buf) != 0) {
                        free(b.data);
                        return NULL;
                    }
                }
            }
        }
    }

    if (!has_term) {
        free(b.data);
        char *z = (char *)malloc(2);
        if (!z) return NULL;
        z[0] = '0';
        z[1] = '\0';
        return z;
    }

    return b.data;
}
