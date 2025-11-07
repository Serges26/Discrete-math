#ifndef TREE_H
#define TREE_H

#include <stddef.h>

// Структура дерева: вершины помечены 1..n, храним списки смежности
typedef struct {
    size_t n;        // количество вершин 
    size_t *deg;     // deg[i] — степень вершины i (1..n) 
    size_t **adj;    // adj[i][0..deg[i]-1] — соседи вершины i
} Tree;


//Восстановить дерево по коду Прюфера.
int tree_from_prufer(size_t n, const size_t *code, Tree *out);


//Сгенерировать случайное помеченное дерево порядка n
int tree_random(size_t n, Tree *out);

void tree_free(Tree *t);

#endif
