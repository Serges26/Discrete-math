#ifndef INTERNAL_H_
#define INTERNAL_H_

#include <stddef.h>
#include "graph.h"

// Выполнить BFS от стартовой вершины start.
// dist_out должно быть выделено с размером g->n
// сюда запишутся расстояния, или (size_t)-1 если недостижимо.
// Возвращает индекс вершины, которая оказалась дальше всех от start,
// а также через max_dist_out возвращает её расстояние.
size_t bfs_farthest(const Graph *g,
                    size_t start,
                    size_t *dist_out,
                    size_t *max_dist_out);

// Проверяет, что граф связен (одна компонента).
// Использует BFS от 0. Возвращает 1 если связен, иначе 0.
int graph_is_connected(const Graph *g);

#endif
