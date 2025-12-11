#include "coloring.h"
#include <stdlib.h>
#include <string.h>

#define ADJ(g, i, j) ((g)->adj[(i) * (g)->n + (j)])


Graph *graph_create(size_t n) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (!g) return NULL;
    g->n = n;
    if (n == 0) {
        g->adj = NULL;
        return g;
    }
    g->adj = (unsigned char *)calloc(n * n, sizeof(unsigned char));
    if (!g->adj) {
        free(g);
        return NULL;
    }
    return g;
}

int graph_add_edge(Graph *g, size_t u, size_t v) {
    if (!g || !g->adj) return -1;
    if (u >= g->n || v >= g->n || u == v) return -1;
    ADJ(g, u, v) = 1;
    ADJ(g, v, u) = 1;
    return 0;
}

void graph_free(Graph *g) {
    if (!g) return;
    free(g->adj);
    g->adj = NULL;
    g->n = 0;
    free(g);
}


static int build_degeneracy_order(const Graph *g, size_t *order) {
    size_t n = g->n;
    if (n == 0) return 0;

    int *deg = (int *)malloc(n * sizeof(int));
    int *removed = (int *)calloc(n, sizeof(int));
    if (!deg || !removed) {
        free(deg);
        free(removed);
        return -1;
    }

    for (size_t i = 0; i < n; ++i) {
        int d = 0;
        for (size_t j = 0; j < n; ++j) {
            if (ADJ(g, i, j)) d++;
        }
        deg[i] = d;
    }

    for (size_t k = 0; k < n; ++k) {
        int found = 0;
        size_t v = 0;
        for (size_t i = 0; i < n; ++i) {
            if (!removed[i] && deg[i] <= 5) {
                found = 1;
                v = i;
                break;
            }
        }
        if (!found) {
            free(deg);
            free(removed);
            return -1;  // не планарный / ошибка
        }
        order[k] = v;
        removed[v] = 1;
        for (size_t j = 0; j < n; ++j) {
            if (!removed[j] && ADJ(g, v, j)) {
                deg[j]--;
            }
        }
    }

    free(deg);
    free(removed);
    return 0;
}


static void bfs_kempe(const Graph *g, const int *colors,
                      int c1, int c2, size_t from, unsigned char *visited) {
    size_t n = g->n;
    size_t *queue = (size_t *)malloc(n * sizeof(size_t));
    if (!queue) return; // в худшем случае просто ничего не сделаем

    size_t head = 0, tail = 0;
    visited[from] = 1;
    queue[tail++] = from;

    while (head < tail) {
        size_t v = queue[head++];
        for (size_t u = 0; u < n; ++u) {
            if (!ADJ(g, v, u)) continue;
            if (visited[u]) continue;
            if (colors[u] != c1 && colors[u] != c2) continue;
            visited[u] = 1;
            queue[tail++] = u;
        }
    }

    free(queue);
}

static void swap_colors_in_component(int *colors, size_t n,
                                     int c1, int c2,
                                     const unsigned char *visited) {
    for (size_t i = 0; i < n; ++i) {
        if (!visited[i]) continue;
        if (colors[i] == c1)
            colors[i] = c2;
        else if (colors[i] == c2)
            colors[i] = c1;
    }
}

int graph_five_color(const Graph *g, int *colors) {
    if (!g || !colors) return -1;

    size_t n = g->n;
    if (n == 0) return 0;

    size_t *order = (size_t *)malloc(n * sizeof(size_t));
    if (!order) return -2;

    if (build_degeneracy_order(g, order) != 0) {
        free(order);
        return -2;
    }

    for (size_t i = 0; i < n; ++i) colors[i] = -1;

    for (size_t idx = n; idx-- > 0; ) {
        size_t v = order[idx];

        int used[5] = {0, 0, 0, 0, 0};
        int neighbors[5];    
        int neigh_count = 0;

        for (size_t u = 0; u < n; ++u) {
            if (!ADJ(g, v, u)) continue;
            int cu = colors[u];
            if (cu < 0 || cu >= 5) continue; 
            if (!used[cu]) {
                used[cu] = 1;
                neighbors[neigh_count++] = u;
            }
        }

        if (neigh_count < 5) {
        
            int c;
            for (c = 0; c < 5; ++c) {
                if (!used[c]) {
                    colors[v] = c;
                    break;
                }
            }
            if (colors[v] == -1) {
                free(order);
                return -2; 
            }
            continue;
        }

       
        int neighbor_of_color[5] = {-1, -1, -1, -1, -1};
        for (int i2 = 0; i2 < 5; ++i2) {
            int u = neighbors[i2];
            int cu = colors[u];
            if (cu >= 0 && cu < 5)
                neighbor_of_color[cu] = u;
        }

        int success = 0;
       
        for (int c1 = 0; c1 < 5 && !success; ++c1) {
            for (int c2 = c1 + 1; c2 < 5 && !success; ++c2) {
                int u1 = neighbor_of_color[c1];
                int u2 = neighbor_of_color[c2];
                if (u1 < 0 || u2 < 0) continue;

                unsigned char *visited =
                    (unsigned char *)calloc(n, sizeof(unsigned char));
                if (!visited) {
                    free(order);
                    return -2;
                }

                bfs_kempe(g, colors, c1, c2, (size_t)u1, visited);
                int connected = visited[u2];

                if (!connected) {
                    
                    swap_colors_in_component(colors, n, c1, c2, visited);
                    free(visited);
                    colors[v] = c1; 
                    success = 1;
                } else {
                    free(visited);
                }
            }
        }

        if (!success) {

            free(order);
            return -2;
        }
    }

    free(order);
    return 0;
}
