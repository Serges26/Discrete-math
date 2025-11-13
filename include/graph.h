#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdbool.h>

typedef struct Graph Graph;

Graph* graph_create(size_t n);
Graph* graph_from_edges(size_t n, const size_t* u, const size_t* v, size_t m);
int graph_add_edge(Graph* g, size_t u, size_t v);
size_t graph_vertex_count(const Graph* g);
bool graph_alive(const Graph* g, size_t v);
size_t graph_degree(const Graph* g, size_t v);
bool graph_has_edge(const Graph* g, size_t u, size_t v);
size_t graph_simplify(Graph* g);
size_t graph_edge_count(const Graph* g);
void graph_free(Graph* g);


#endif 
