#include <stdio.h>
#include <stdlib.h>
#include "src/clause.h"

int main(void) {
    int n, kolichestvo;
    printf("Введите n (количество переменных): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Некорректное значение n.\n");
        return 1;
    }

    printf("Введите количество троек: ");
    if (scanf("%d", &kolichestvo) != 1 || kolichestvo < 0) {
        printf("Некорректное количество троек.\n");
        return 1;
    }

    clause_t *klauzy = calloc((size_t)kolichestvo, sizeof(clause_t));
    if (klauzy == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }

    printf("Введите тройки (i j k), числа от -n до n (0 не допускается):\n");
    for (int i = 0; i < kolichestvo; i++) {
        if (scanf("%d %d %d", &klauzy[i].i, &klauzy[i].j, &klauzy[i].k) != 3) {
            printf("Ошибка ввода данных.\n");
            free(klauzy);
            return 1;
        }

        if (klauzy[i].i == 0 || klauzy[i].j == 0 || klauzy[i].k == 0 ||
            abs(klauzy[i].i) > n || abs(klauzy[i].j) > n || abs(klauzy[i].k) > n) {
            printf("Ошибка: значения должны быть в диапазоне [-n, n], без 0.\n");
            free(klauzy);
            return 1;
        }
    }

    int rezultat = is_sat(klauzy, (unsigned)kolichestvo);

    if (rezultat)
        printf("Существует набор (x1,...,xn), удовлетворяющий условию.\n");
    else
        printf("Такого набора не существует.\n");

    free(klauzy);
    return 0;
}
