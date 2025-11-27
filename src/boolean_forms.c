#include "boolean_forms.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static int is_power_of_two(size_t len, size_t *nvars) {
    size_t k = 0;
    size_t tmp = len;

    if (len < 2) {
        return 0;
    }
    while (tmp > 1 && (tmp % 2u) == 0u) {
        tmp /= 2u;
        k++;
    }
    if (tmp != 1u) {
        return 0;
    }

    if (nvars) {
        *nvars = k;
    }
    return 1;
}

static int append_str(char **str, size_t *len, size_t *cap, const char *suffix) {
    size_t add = strlen(suffix);

    if (*len + add + 1 > *cap) {
        size_t new_cap = (*cap == 0) ? 64u : *cap * 2u;
        while (*len + add + 1 > new_cap) {
            new_cap *= 2u;
        }
        char *tmp = (char *)realloc(*str, new_cap);
        if (!tmp) {
            return -1;
        }
        *str = tmp;
        *cap = new_cap;
    }

    memcpy(*str + *len, suffix, add);
    *len += add;
    (*str)[*len] = '\0';

    return 0;
}

int build_boolean_forms(const char *vector, BooleanForms *out) {
    if (!vector || !out) {
        return BF_ERR_BAD_ARGS;
    }

    out->pdnf = NULL;
    out->pcnf = NULL;

    size_t len = strlen(vector);
    if (len == 0) {
        return BF_ERR_BAD_LENGTH;
    }

    
    for (size_t i = 0; i < len; i++) {
        if (vector[i] != '0' && vector[i] != '1') {
            return BF_ERR_BAD_CHAR;
        }
    }

    
    size_t nvars = 0;
    if (!is_power_of_two(len, &nvars)) {
        return BF_ERR_BAD_LENGTH;
    }

    char *pdnf = NULL;
    size_t pdnf_len = 0, pdnf_cap = 0;

    char *pcnf = NULL;
    size_t pcnf_len = 0, pcnf_cap = 0;

    int has_one = 0;
    int has_zero = 0;

   
    for (size_t row = 0; row < len; row++) {
        int value = (vector[row] == '1');

        if (value) {
          
            has_one = 1;

            if (pdnf_len > 0) {
                if (append_str(&pdnf, &pdnf_len, &pdnf_cap, " | ") < 0) {
                    goto nomem;
                }
            }
            if (append_str(&pdnf, &pdnf_len, &pdnf_cap, "(") < 0) {
                goto nomem;
            }

            for (size_t v = 0; v < nvars; v++) {
                if (v > 0) {
                    if (append_str(&pdnf, &pdnf_len, &pdnf_cap, " & ") < 0) {
                        goto nomem;
                    }
                }

                int var_val = (int)((row >> (nvars - 1 - v)) & 1u);
                if (var_val == 0) {
                    if (append_str(&pdnf, &pdnf_len, &pdnf_cap, "!") < 0) {
                        goto nomem;
                    }
                }

                char buf[32];
                (void)snprintf(buf, sizeof(buf), "x%zu", v + 1);
                if (append_str(&pdnf, &pdnf_len, &pdnf_cap, buf) < 0) {
                    goto nomem;
                }
            }

            if (append_str(&pdnf, &pdnf_len, &pdnf_cap, ")") < 0) {
                goto nomem;
            }
        } else {
            
            has_zero = 1;

            if (pcnf_len > 0) {
                if (append_str(&pcnf, &pcnf_len, &pcnf_cap, " & ") < 0) {
                    goto nomem;
                }
            }
            if (append_str(&pcnf, &pcnf_len, &pcnf_cap, "(") < 0) {
                goto nomem;
            }

            for (size_t v = 0; v < nvars; v++) {
                if (v > 0) {
                    if (append_str(&pcnf, &pcnf_len, &pcnf_cap, " | ") < 0) {
                        goto nomem;
                    }
                }

                int var_val = (int)((row >> (nvars - 1 - v)) & 1u);
                if (var_val == 1) {
                    if (append_str(&pcnf, &pcnf_len, &pcnf_cap, "!") < 0) {
                        goto nomem;
                    }
                }

                char buf[32];
                (void)snprintf(buf, sizeof(buf), "x%zu", v + 1);
                if (append_str(&pcnf, &pcnf_len, &pcnf_cap, buf) < 0) {
                    goto nomem;
                }
            }

            if (append_str(&pcnf, &pcnf_len, &pcnf_cap, ")") < 0) {
                goto nomem;
            }
        }
    }

    /* Если нет ни одной единицы: функция тождественно 0, СДНФ = 0 */
    if (!has_one) {
        if (append_str(&pdnf, &pdnf_len, &pdnf_cap, "0") < 0) {
            goto nomem;
        }
    }

    /* Если нет ни одного нуля: функция тождественно 1, СКНФ = 1 */
    if (!has_zero) {
        if (append_str(&pcnf, &pcnf_len, &pcnf_cap, "1") < 0) {
            goto nomem;
        }
    }

    out->pdnf = pdnf;
    out->pcnf = pcnf;

    return BF_OK;

nomem:
    free(pdnf);
    free(pcnf);
    out->pdnf = NULL;
    out->pcnf = NULL;
    return BF_ERR_NO_MEMORY;
}

void free_boolean_forms(BooleanForms *f) {
    if (!f) {
        return;
    }
    free(f->pdnf);
    free(f->pcnf);
    f->pdnf = NULL;
    f->pcnf = NULL;
}
