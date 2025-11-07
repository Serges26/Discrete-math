
#include "tree.h"
#include "heap.h"


static void tree_free_partial(Tree *t) {
    if (!t) return;
    if (t->adj) {
        for (size_t i = 1; i <= t->n; ++i) {
            free(t->adj[i]);
        }
    }
    free(t->adj);
    free(t->deg);
    t->adj = NULL;
    t->deg = NULL;
    t->n = 0;
}

int tree_from_prufer(size_t n, const size_t *code, Tree *out) {
    if (!out) return -1;
    if (n == 0) return -1;

    
    if (n == 1) {
        out->n = 1;
        out->deg = calloc(2, sizeof(size_t));
        out->adj = calloc(2, sizeof(size_t *));
        if (!out->deg || !out->adj) {
            tree_free_partial(out);
            return -1;
        }
        return 0;
    }

    // n = 2: одно ребро 1-2 
    if (n == 2) {
        out->n = 2;
        out->deg = calloc(3, sizeof(size_t));
        out->adj = calloc(3, sizeof(size_t *));
        if (!out->deg || !out->adj) {
            tree_free_partial(out);
            return -1;
        }
        out->deg[1] = 1;
        out->deg[2] = 1;
        out->adj[1] = malloc(sizeof(size_t));
        out->adj[2] = malloc(sizeof(size_t));
        if (!out->adj[1] || !out->adj[2]) {
            tree_free_partial(out);
            return -1;
        }
        out->adj[1][0] = 2;
        out->adj[2][0] = 1;
        return 0;
    }

    //n >= 3

    // считаем истинные степени по коду Прюфера
    size_t *deg = calloc(n + 1, sizeof(size_t));
    if (!deg) return -1;

    for (size_t i = 1; i <= n; ++i)
        deg[i] = 1;
    for (size_t i = 0; i < n - 2; ++i) {
        size_t v = code[i];
        if (v < 1 || v > n) {
            free(deg);
            return -1;
        }
        deg[v]++;  //в коде встретилась -> степень +1 
    }

    // 2 выделяем списки смежности по степеням
    size_t **adj = calloc(n + 1, sizeof(size_t *));
    if (!adj) {
        free(deg);
        return -1;
    }
    for (size_t i = 1; i <= n; ++i) {
        adj[i] = malloc(deg[i] * sizeof(size_t));
        if (!adj[i]) {
            for (size_t j = 1; j < i; ++j) free(adj[j]);
            free(adj);
            free(deg);
            return -1;
        }
    }

    // 3 рабочие степени — копия, её будем уменьшать
    size_t *work_deg = calloc(n + 1, sizeof(size_t));
    if (!work_deg) {
        for (size_t i = 1; i <= n; ++i) free(adj[i]);
        free(adj);
        free(deg);
        return -1;
    }
    for (size_t i = 1; i <= n; ++i)
        work_deg[i] = deg[i];

    // куда писать следующего соседа 
    size_t *cur = calloc(n + 1, sizeof(size_t));
    if (!cur) {
        free(work_deg);
        for (size_t i = 1; i <= n; ++i) free(adj[i]);
        free(adj);
        free(deg);
        return -1;
    }

    // 4 куча листьев по РАБОЧИМ степеням
    MinHeap *h = heap_create(n);
    if (!h) {
        free(cur);
        free(work_deg);
        for (size_t i = 1; i <= n; ++i) free(adj[i]);
        free(adj);
        free(deg);
        return -1;
    }

    for (size_t i = 1; i <= n; ++i) {
        if (work_deg[i] == 1) {
            heap_push(h, i);
        }
    }

    // 5. основной цикл: n-2 раза
    for (size_t i = 0; i < n - 2; ++i) {
        size_t leaf = heap_pop(h);
        size_t v    = code[i];

        // записать ребро leaf - v
        adj[leaf][cur[leaf]++] = v;
        adj[v][cur[v]++]       = leaf;

        work_deg[leaf]--;
        work_deg[v]--;
        if (work_deg[v] == 1) {
            heap_push(h, v);
        }
    }

    // 6 две последние вершины
    size_t u = heap_pop(h);
    size_t v = heap_pop(h);
    adj[u][cur[u]++] = v;
    adj[v][cur[v]++] = u;

    heap_free(h);
    free(cur);
    free(work_deg);

    out->n   = n;
    out->deg = deg;   // здесь лежат настоящие степени
    out->adj = adj;
    return 0;
}

int tree_random(size_t n, Tree *out) {
    if (!out) return -1;
    if (n == 0) return -1;

    if (n == 1 || n == 2) {
        return tree_from_prufer(n, NULL, out);
    }

    size_t *code = malloc((n - 2) * sizeof(size_t));
    if (!code) return -1;
    for (size_t i = 0; i < n - 2; ++i) {
        code[i] = (size_t)(rand() % (int)n) + 1; // 1..n
    }
    int rc = tree_from_prufer(n, code, out);
    free(code);
    return rc;
}

void tree_free(Tree *t) {
    if (!t) return;
    if (t->adj) {
        for (size_t i = 1; i <= t->n; ++i) {
            free(t->adj[i]);
        }
    }
    free(t->adj);
    free(t->deg);
    t->adj = NULL;
    t->deg = NULL;
    t->n = 0;
}
