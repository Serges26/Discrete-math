#include "lrp.h"
void calculate_lrp(double a, double b, double c, double d) {
    double delta = a * a + 4 * b;
    printf("x1 = %.2f, x2 = %.2f\n", c, d);

    if (delta > 0) {
        double S = sqrt(delta);
        double r1 = (a + S) / 2.0;
        double r2 = (a - S) / 2.0;
        double A = (d - c * r2) / (r1 - r2);
        double B = (c * r1 - d) / (r1 - r2);
        printf("Δ = %.2f > 0\n", delta);
        printf("r1 = %.2f, r2 = %.2f\n", r1, r2);
        printf("A = %.2f, B = %.2f\n", A, B);
        printf("Формула: x_n = %.2f*(%.2f)^(n-1) + %.2f*(%.2f)^(n-1)\n",
               A, r1, B, r2);
    } 
    else if (delta == 0) {
       
        double r = a / 2.0;
        double A = c;
        double B = (r != 0) ? (d / r - c) : 0;

        printf("r = %.2f\n", r);
        printf("A = %.2f, B = %.2f\n", A, B);
        printf("Формула: x_n = (%.2f + %.2f*(n-1))*(%.2f)^(n-1)\n", A, B, r);
    } 
    else {
        // комплексные корни
        double S = sqrt(-delta);
        double realPart = a / 2.0;
        double imagPart = S / 2.0;
        printf("Дельта = %.2f < 0\n", delta);
        printf("r1, r2 = %.2f ± i*%.2f\n", realPart, imagPart);
        printf("Формула: x_n = A*(r1)^(n-1) + B*(r2)^(n-1)\n");
    }
}