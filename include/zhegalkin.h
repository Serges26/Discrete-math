#ifndef ZHEGALKIN_H
#define ZHEGALKIN_H

#include <stddef.h>

typedef struct {
    size_t nvars;        // число переменных
    size_t size;         // равно 2^nvars
    unsigned char *coeffs;
} ZhegalkinAnf;

int zhegalkin_from_truth(const int *f, size_t nvars, ZhegalkinAnf *out);
void zhegalkin_free(ZhegalkinAnf *anf);
char *zhegalkin_to_string(const ZhegalkinAnf *anf);

#endif
