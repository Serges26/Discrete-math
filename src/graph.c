#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"
#include "../include/internal.h"

static int cmp_size_t(const void *a, const void *b) {
    size_t x = *(const size_t *)a;
    size_t y = *(const size_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

Graph *graph_create(size_t n,
                    const size_t *edges_u,
                    const size_t *edges_v,
                    size_t m) {
    if (n == 0) return NULL;
    if (!edges_u || !edges_v) return NULL;

    // Проверка, что все индексы допустимы
    for (size_t i = 0; i < m; ++i) {
        if (edges_u[i] >= n || edges_v[i] >= n) {
            return NULL;
        }
    }

    Graph *g = (Graph *)calloc(1, sizeof(Graph));
    if (!g) return NULL;

    g->n = n;
    g->m = m;

    // Подсчёт степеней вершин
    size_t *deg = (size_t *)calloc(n, sizeof(size_t));
    if (!deg) {
        free(g);
        return NULL;
    }
    for (size_t i = 0; i < m; ++i) {
        size_t a = edges_u[i];
        size_t b = edges_v[i];
        if (a == b) {
            continue;
        }
        deg[a]++;
        deg[b]++;
    }

    // offset: префиксные суммы
    g->offset = (size_t *)malloc((n + 1) * sizeof(size_t));
    if (!g->offset) {
        free(deg);
        free(g);
        return NULL;
    }

    g->offset[0] = 0;
    for (size_t v = 0; v < n; ++v) {
        g->offset[v + 1] = g->offset[v] + deg[v];
    }

    size_t total_adj = g->offset[n];
    g->adj = (size_t *)malloc(total_adj * sizeof(size_t));
    if (!g->adj) {
        free(g->offset);
        free(deg);
        free(g);
        return NULL;
    }

    // временный указатель на текущую позицию записи соседа
    size_t *cur = (size_t *)calloc(n, sizeof(size_t));
    if (!cur) {
        free(g->adj);
        free(g->offset);
        free(deg);
        free(g);
        return NULL;
    }

    // заполнение списков смежности
    for (size_t i = 0; i < m; ++i) {
        size_t a = edges_u[i];
        size_t b = edges_v[i];
        if (a == b) continue;
        size_t pa = g->offset[a] + cur[a]++;
        size_t pb = g->offset[b] + cur[b]++;
        g->adj[pa] = b;
        g->adj[pb] = a;
    }

    // можно отсортировать соседей каждой вершины
    for (size_t v = 0; v < n; ++v) {
        size_t begin = g->offset[v];
        size_t end   = g->offset[v + 1];
        qsort(g->adj + begin, end - begin, sizeof(size_t), cmp_size_t);
    }

    free(cur);
    free(deg);
    return g;
}

void graph_destroy(Graph *g) {
    if (!g) return;
    free(g->offset);
    free(g->adj);
    free(g);
}

// Простая очередь для BFS
typedef struct {
    size_t *data;
    size_t head;
    size_t tail;
    size_t cap;
} Queue;

static int q_init(Queue *q, size_t cap) {
    q->data = (size_t *)malloc(cap * sizeof(size_t));
    if (!q->data) return 0;
    q->head = 0;
    q->tail = 0;
    q->cap = cap;
    return 1;
}

static void q_destroy(Queue *q) {
    free(q->data);
}

static int q_empty(const Queue *q) {
    return q->head == q->tail;
}

static void q_push(Queue *q, size_t v) {
    q->data[q->tail++] = v;
}

static size_t q_pop(Queue *q) {
    return q->data[q->head++];
}

size_t bfs_farthest(const Graph *g,
                    size_t start,
                    size_t *dist_out,
                    size_t *max_dist_out) {
    const size_t INF = (size_t)-1;

    for (size_t i = 0; i < g->n; ++i) {
        dist_out[i] = INF;
    }

    Queue q;
    if (!q_init(&q, g->n)) {
        // при ошибке памяти вернём start с расстоянием 0
        *max_dist_out = 0;
        return start;
    }

    dist_out[start] = 0;
    q_push(&q, start);

    size_t farthest_v = start;
    size_t farthest_d = 0;

    while (!q_empty(&q)) {
        size_t v = q_pop(&q);
        size_t dv = dist_out[v];

        if (dv > farthest_d) {
            farthest_d = dv;
            farthest_v = v;
        }

        size_t begin = g->offset[v];
        size_t end   = g->offset[v + 1];
        for (size_t i = begin; i < end; ++i) {
            size_t to = g->adj[i];
            if (dist_out[to] == INF) {
                dist_out[to] = dv + 1;
                q_push(&q, to);
            }
        }
    }

    q_destroy(&q);
    *max_dist_out = farthest_d;
    return farthest_v;
}

int graph_is_connected(const Graph *g) {
    if (!g) return 0;
    if (g->n == 0) return 0;
    size_t *dist = (size_t *)malloc(g->n * sizeof(size_t));
    if (!dist) return 0;

    size_t maxd;
    bfs_farthest(g, 0, dist, &maxd);

    // проверяем, что все достижимы
    int ok = 1;
    for (size_t i = 0; i < g->n; ++i) {
        if (dist[i] == (size_t)-1) {
            ok = 0;
            break;
        }
    }

    free(dist);
    return ok;
}

size_t graph_diameter(const Graph *g) {
    if (!g) return 0;
    if (g->n == 0) return 0;

    // граф должен быть связный, иначе диаметр в классическом смысле не определён
    if (!graph_is_connected(g)) {
        return 0;
    }

    size_t *dist = (size_t *)malloc(g->n * sizeof(size_t));
    if (!dist) {
        return 0;
    }

    // 1) BFS из произвольной вершины (0) -> получаем самую дальнюю u
    size_t maxd1;
    size_t u = bfs_farthest(g, 0, dist, &maxd1);

    // 2) BFS из u -> получаем диаметр
    size_t maxd2;
    bfs_farthest(g, u, dist, &maxd2);

    free(dist);
    return maxd2;
}
