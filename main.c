#include "src/lrp.h"

int main() {
    double a, b, c, d;
    printf("Введите a, b, c, d: ");
    scanf("%lf %lf %lf %lf", &a, &b, &c, &d);
    calculate_lrp(a, b, c, d);
    return 0;
}