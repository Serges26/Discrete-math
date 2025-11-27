#ifndef BOOLEAN_FORMS_H
#define BOOLEAN_FORMS_H

#include <stddef.h>

typedef struct {
    char *pdnf; /* совершенная ДНФ */
    char *pcnf; /* совершенная КНФ */
} BooleanForms;

/* Коды ошибок */
enum {
    BF_OK = 0,
    BF_ERR_BAD_ARGS   = 1, /* NULL-указатель и т.п. */
    BF_ERR_BAD_CHAR   = 2, /* символ, отличный от '0' или '1' */
    BF_ERR_BAD_LENGTH = 3, /* длина не является степенью двойки или < 2 */
    BF_ERR_NO_MEMORY  = 4  /* не хватило памяти */
};


int build_boolean_forms(const char *vector, BooleanForms *out);

void free_boolean_forms(BooleanForms *f);

#endif
