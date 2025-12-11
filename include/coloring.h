#ifndef COLORING_H
#define COLORING_H

#include <stddef.h>
typedef struct {
    size_t n;
    unsigned char *adj;
} Graph;

Graph *graph_create(size_t n);
int graph_add_edge(Graph *g, size_t u, size_t v);
void graph_free(Graph *g);


int graph_five_color(const Graph *g, int *colors);

#endif
