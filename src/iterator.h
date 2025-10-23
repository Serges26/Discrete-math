#ifndef CBS_ITERATOR_H_
#define CBS_ITERATOR_H_

#include "cbs.h"



typedef struct CbsIterator {
  cbs_t value;

  unsigned n;
  unsigned pos;
  unsigned open;
  unsigned close;
  unsigned char *choice;  
  int finished;
} iterator_t;


void iterator_init(iterator_t *i, unsigned n);
const cbs_t *iterator_value(const iterator_t *i);
int iterator_has_next(const iterator_t *i);
void iterator_next(iterator_t *i);
void iterator_destroy(iterator_t *i);



#endif  