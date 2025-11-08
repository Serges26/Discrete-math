#ifndef GRAPH_H_
#define GRAPH_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Структура графа (неориентированный, невзвешенный)
typedef struct {
    size_t n;        // кол-во вершин
    size_t m;        // кол-во рёбер (неориентированных пар)
    size_t *offset;  // размер n+1: offset[v]..offset[v+1)-1 это соседи v
    size_t *adj;     // размер 2*m: списки смежности подряд
} Graph;

// Создать граф по списку рёбер.
Graph *graph_create(size_t n,
                    const size_t *edges_u,
                    const size_t *edges_v,
                    size_t m);


void graph_destroy(Graph *g);


// Возвращает 0 при ошибке (например, граф несвязный или NULL).
// Если граф валиден и связен, возвращает диаметр>=0.
size_t graph_diameter(const Graph *g);

#endif
