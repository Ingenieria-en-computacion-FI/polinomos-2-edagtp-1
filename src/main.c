/*
 * Universidad Nacional Autónoma de México
 * Semestre 2026-2 — Estructuras de datos y algoritmos I
 * TAD Polinomio — Pruebas
 *
 *  Alumnos:
 *    Ramirez Olvera Erick Yael
 *    Rosas Salazar Jose Eduardo
 *    Gonzalez Morales Daniel Alejandro
 *    Flores Alvarez Roberto Maximiliano
 */

#include <stdio.h>
#include <stdlib.h>
#include "polinomio.h"

int main(void) {
    char *str;

    printf("╔══════════════════════════════╗\n");
    printf("║     TAD Polinomio — Pruebas  ║\n");
    printf("╚══════════════════════════════╝\n\n");

    /* ── Caso 1: Inserción y display ── */
    printf("--- Caso 1: Insercion y display ---\n");
    Polinomio *p1 = crearPolinomio();
    insertarTermino(p1, 3, 2);
    insertarTermino(p1, 2, 1);
    insertarTermino(p1, 1, 0);
    str = polinomioToString(p1);
    printf("P1 = %s\n", str); free(str);
    /* Esperado: 3x^2+2x+1 */

    /* ── Caso 2: Suma ── */
    printf("\n--- Caso 2: Suma ---\n");
    Polinomio *p2 = crearPolinomio();
    insertarTermino(p2, 1, 2);
    insertarTermino(p2, 4, 0);
    str = polinomioToString(p2);
    printf("P2 = %s\n", str); free(str);

    Polinomio *suma = sumarPolinomios(p1, p2);
    str = polinomioToString(suma);
    printf("P1 + P2 = %s\n", str); free(str);
    /* Esperado: 4x^2+2x+5 */

    /* ── Caso 3: Simplificación a cero ── */
    printf("\n--- Caso 3: Simplificacion ---\n");
    Polinomio *p3 = crearPolinomio();
    insertarTermino(p3,  3, 2);
    insertarTermino(p3, -3, 2);
    str = polinomioToString(p3);
    printf("3x^2 + (-3x^2) = %s\n", str); free(str);
    /* Esperado: 0 */

    /* ── Caso 4: Multiplicación ── */
    printf("\n--- Caso 4: Multiplicacion ---\n");
    Polinomio *p4 = crearPolinomio();
    insertarTermino(p4, 2, 1);
    insertarTermino(p4, 3, 0);

    Polinomio *p5 = crearPolinomio();
    insertarTermino(p5, 1, 2);
    insertarTermino(p5, 4, 0);

    str = polinomioToString(p4);
    printf("P4 = %s\n", str); free(str);
    str = polinomioToString(p5);
    printf("P5 = %s\n", str); free(str);

    Polinomio *prod = multiplicarPolinomios(p4, p5);
    str = polinomioToString(prod);
    printf("P4 * P5 = %s\n", str); free(str);
    /* Esperado: 2x^3+3x^2+8x+12 */

    /* ── Caso 5: Evaluación ── */
    printf("\n--- Caso 5: Evaluacion ---\n");
    printf("P1(2)  = %.2f\n", evaluarPolinomio(p1, 2.0f));   /* 17.00 */
    printf("P1(0)  = %.2f\n", evaluarPolinomio(p1, 0.0f));   /*  1.00 */
    printf("P1(-1) = %.2f\n", evaluarPolinomio(p1, -1.0f));  /*  2.00 */

    /* ── Caso 6: Eliminar término ── */
    printf("\n--- Caso 6: Eliminar termino ---\n");
    str = polinomioToString(p1);
    printf("P1 antes:   %s\n", str); free(str);
    eliminarNodo(p1, 2);
    str = polinomioToString(p1);
    printf("P1 sin x^2: %s\n", str); free(str);
    /* Esperado: 2x+1 */

    /* ── Caso 7: Simplificación parcial ── */
    printf("\n--- Caso 7: Simplificacion parcial ---\n");
    Polinomio *p6 = crearPolinomio();
    insertarTermino(p6,  5, 3);
    insertarTermino(p6, -2, 3);
    insertarTermino(p6,  4, 1);
    str = polinomioToString(p6);
    printf("5x^3 - 2x^3 + 4x = %s\n", str); free(str);
    /* Esperado: 3x^3+4x */

    /* ── Liberar memoria ── */
    destruirPolinomio(p1);
    destruirPolinomio(p2);
    destruirPolinomio(p3);
    destruirPolinomio(p4);
    destruirPolinomio(p5);
    destruirPolinomio(p6);
    destruirPolinomio(suma);
    destruirPolinomio(prod);

    return 0;
}