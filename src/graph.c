#include "internal.h"
#include "graph.h"
#include <stdlib.h>

typedef struct Graph {
    size_t n;
    bool*  alive;   // жива ли вершина
    Vec*   adj;     // списки смежности
} Graph;


int vec_push(Vec* v, size_t x){
    if (v->n == v->cap){
        size_t nc = v->cap ? v->cap*2 : 4;
        size_t* na = (size_t*)realloc(v->a, nc * sizeof(size_t));
        if (!na) return -1;
        v->a = na; v->cap = nc;
    }
    v->a[v->n++] = x;
    return 0;
}
bool vec_contains_alive(const Vec* v, const bool* alive, size_t x){
    for (size_t i=0;i<v->n;i++){
        size_t y=v->a[i]; if (!alive[y]) continue;
        if (y==x) return true;
    }
    return false;
}
void vec_remove_first(Vec* v, size_t x){
    for (size_t i=0;i<v->n;i++){
        if (v->a[i]==x){ v->a[i]=v->a[v->n-1]; v->n--; return; }
    }
}
void vec_free(Vec* v){ free(v->a); v->a=NULL; v->n=v->cap=0; }


Graph* graph_create(size_t n){
    Graph* g = (Graph*)calloc(1,sizeof(Graph));
    if(!g) return NULL;
    g->n=n; g->alive=(bool*)malloc(n*sizeof(bool)); g->adj=(Vec*)malloc(n*sizeof(Vec));
    if(!g->alive||!g->adj){ graph_free(g); return NULL; }
    for(size_t i=0;i<n;i++){ g->alive[i]=true; vec_init(&g->adj[i]); }
    return g;
}
Graph* graph_from_edges(size_t n,const size_t* u,const size_t* v,size_t m){
    Graph* g=graph_create(n); if(!g) return NULL;
    for(size_t i=0;i<m;i++){ if(u[i]==v[i]) vec_push(&g->adj[u[i]], u[i]);
                             else { vec_push(&g->adj[u[i]], v[i]); vec_push(&g->adj[v[i]], u[i]); } }
    return g;
}
size_t graph_vertex_count(const Graph* g){ return g?g->n:0; }
bool   graph_alive(const Graph* g,size_t v){ return g&&v<g->n&&g->alive[v]; }

int graph_add_edge(Graph* g,size_t u,size_t v){
    if(!g||u>=g->n||v>=g->n) return -1;
    if(u==v) return vec_push(&g->adj[u], u);
    if(vec_push(&g->adj[u], v)<0) return -1;
    if(vec_push(&g->adj[v], u)<0) return -1;
    return 0;
}

static size_t degree_alive(const Graph* g,size_t v){
    size_t d=0; const Vec* a=&g->adj[v];
    for(size_t i=0;i<a->n;i++){ size_t u=a->a[i]; if(g->alive[u]) d++; }
    return d;
}
size_t graph_degree(const Graph* g,size_t v){
    if(!g||v>=g->n||!g->alive[v]) return (size_t)-1;
    return degree_alive(g,v);
}

bool graph_has_edge(const Graph* g, size_t u, size_t v) {
    if (!g || u >= g->n || v >= g->n) return false;
    if (!g->alive[u] || !g->alive[v]) return false;

    if (u == v) {
        const Vec* a = &g->adj[u];
        for (size_t i = 0; i < a->n; i++) {
            if (a->a[i] == u) return true;
        }
        return false;
    }

    return vec_contains_alive(&g->adj[u], g->alive, v) &&
           vec_contains_alive(&g->adj[v], g->alive, u);
}


static void remove_edge_once(Graph* g,size_t a,size_t b){ vec_remove_first(&g->adj[a], b); }

size_t graph_simplify(Graph* g) {
    if (!g) return 0;
    size_t suppressed = 0;
    bool progress = true;

    while (progress) {
        progress = false;
        for (size_t v = 0; v < g->n; v++) {
            if (!g->alive[v]) continue;
            if (degree_alive(g, v) != 2) continue;

        
            size_t u = (size_t)-1, w = (size_t)-1;
            for (size_t i = 0; i < g->adj[v].n; i++) {
                size_t x = g->adj[v].a[i];
                if (!g->alive[x]) continue;
                if (u == (size_t)-1) u = x;
                else { w = x; break; }
            }
            if (u == (size_t)-1 || w == (size_t)-1) continue;
            if (u == v || w == v) continue;  // у самой v петля - пропускаем
            // удалить старые рёбра
            remove_edge_once(g, v, u);
            remove_edge_once(g, u, v);
            remove_edge_once(g, v, w);
            remove_edge_once(g, w, v);
            g->alive[v] = false;

            // добавляем новое ребро
            if (u == w) {
                // удалить две копии ребра
                remove_edge_once(g, v, u);
                remove_edge_once(g, u, v);
                remove_edge_once(g, v, u);
                remove_edge_once(g, u, v);
                g->alive[v] = false;
                vec_push(&g->adj[u], u);
                vec_push(&g->adj[u], u);

                suppressed++;
                progress = true;
                continue;
            }
            else {
                vec_push(&g->adj[u], w);
                vec_push(&g->adj[w], u);
            }

            suppressed++;
            progress = true;
        }
    }
    return suppressed;
}


size_t graph_edge_count(const Graph* g){
    if(!g) return 0;
    size_t c =0;
    for(size_t v=0; v<g->n; v++){
        if(!g->alive[v]) continue;
        const Vec* a=&g->adj[v];
        for(size_t i=0;i<a->n;i++){
            size_t u=a->a[i]; if(!g->alive[u]) continue;
            if(u>=v) c++; // петля и каждое параллельное ребро учитываются
        }
    }
    return c;
}

void graph_free(Graph* g){
    if(!g) return;
    if(g->adj) for(size_t i=0;i<g->n;i++) vec_free(&g->adj[i]);
    free(g->adj); free(g->alive); free(g);
}
