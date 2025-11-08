#include "../include/graph.h"
#include "../include/internal.h"


typedef struct {
    size_t *data;
    size_t head, tail, cap;
} Queue;

static int q_init(Queue *q, size_t cap) {
    q->data = (size_t *)malloc(cap * sizeof(size_t));
    if (!q->data) return 0;
    q->head = q->tail = 0;
    q->cap = cap;
    return 1;
}

static void q_destroy(Queue *q) {
    free(q->data);
    q->data = NULL;
    q->cap = q->head = q->tail = 0;
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

Graph *graph_create(size_t n, const size_t *edges_u, const size_t *edges_v, size_t m) {
    if (n == 0) return NULL;
    if (!edges_u || !edges_v) return NULL;

    for (size_t i = 0; i < m; ++i) {
        if (edges_u[i] >= n || edges_v[i] >= n) {
            return NULL;
        }
    }

    Graph *g = (Graph *)calloc(1, sizeof(Graph));
    if (!g) return NULL;

    g->n = n;
    g->m = m;

    size_t *deg = (size_t *)calloc(n, sizeof(size_t));
    if (!deg) {
        free(g);
        return NULL;
    }
    for (size_t i = 0; i < m; ++i) {
        deg[edges_u[i]]++;
        deg[edges_v[i]]++;
    }

    g->offset = (size_t *)calloc(n + 1, sizeof(size_t));
    if (!g->offset) {
        free(deg);
        free(g);
        return NULL;
    }
    for (size_t i = 1; i <= n; ++i) {
        g->offset[i] = g->offset[i - 1] + deg[i - 1];
    }

    g->adj = (size_t *)calloc(g->offset[n], sizeof(size_t));
    if (!g->adj) {
        free(deg);
        free(g->offset);
        free(g);
        return NULL;
    }

    size_t *pos = (size_t *)calloc(n, sizeof(size_t));
    if (!pos) {
        free(deg);
        free(g->adj);
        free(g->offset);
        free(g);
        return NULL;
    }

    for (size_t i = 0; i < m; ++i) {
        size_t u = edges_u[i];
        size_t v = edges_v[i];
        g->adj[g->offset[u] + pos[u]++] = v;
        g->adj[g->offset[v] + pos[v]++] = u;
    }

    free(pos);
    free(deg);
    return g;
}

void graph_destroy(Graph *g) {
    if (!g) return;
    free(g->adj);
    free(g->offset);
    free(g);
}

static size_t bfs_from_vertex(const Graph *g,
                              size_t start,
                              size_t *dist,
                              int *has_unreachable) {
    const size_t INF = (size_t)-1;
    Queue q;

    for (size_t i = 0; i < g->n; ++i) {
        dist[i] = INF;
    }

    if (!q_init(&q, g->n)) {
        *has_unreachable = 1;
        return 0;
    }

    dist[start] = 0;
    q_push(&q, start);

    size_t max_dist = 0;

    while (!q_empty(&q)) {
        size_t v = q_pop(&q);
        size_t dv = dist[v];

        if (dv > max_dist) max_dist = dv;

        size_t begin = g->offset[v];
        size_t end   = g->offset[v + 1];
        for (size_t i = begin; i < end; ++i) {
            size_t to = g->adj[i];
            if (dist[to] == INF) {
                dist[to] = dv + 1;
                q_push(&q, to);
            }
        }
    }

    q_destroy(&q);

    *has_unreachable = 0;
    for (size_t i = 0; i < g->n; ++i) {
        if (dist[i] == INF) {
            *has_unreachable = 1;
            break;
        }
    }

    return max_dist;
}

size_t graph_diameter(const Graph *g) {
    if (!g) return 0;
    if (g->n == 0) return 0;

    size_t *dist = (size_t *)malloc(g->n * sizeof(size_t));
    if (!dist) return 0;

    size_t diameter = 0;

    for (size_t v = 0; v < g->n; ++v) {
        int has_unreachable = 0;
        size_t maxd = bfs_from_vertex(g, v, dist, &has_unreachable);
        if (has_unreachable) {
            diameter = 0;
            break;
        }
        if (maxd > diameter) diameter = maxd;
    }

    free(dist);
    return diameter;
}
