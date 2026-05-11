/*
 * Universidad Nacional Autónoma de México
 * Semestre 2026-2 — Estructuras de datos y algoritmos I
 * TAD Polinomio — Todo en uno
 * Ing. Adara Mercado Martínez
 *
 *  Alumnos:
 *    Ramirez Olvera Erick Yael
 *    Rosas Salazar Jose Eduardo
 *    Gonzalez Morales Daniel Alejandro
 *    Flores Alvarez Roberto Maximiliano
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "polinomio.h"

/* ═══════════════════════════════════════════════════════════════════════════
 *  LISTA — funciones internas
 * ═══════════════════════════════════════════════════════════════════════════ */

Nodo* crearNodo(float coef, int exp) {
    Nodo* n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) return NULL;
    n->termino.coeficiente = coef;
    n->termino.exponente   = exp;
    n->siguiente = NULL;
    n->anterior  = NULL;
    return n;
}

void insertarNodoOrdenado(Polinomio* p, Nodo* nuevo) {
    if (!p || !nuevo) return;

    /* Lista vacía */
    if (!p->cabeza) {
        p->cabeza = nuevo;
        p->cola   = nuevo;
        return;
    }

    /* Insertar al frente */
    if (nuevo->termino.exponente > p->cabeza->termino.exponente) {
        nuevo->siguiente    = p->cabeza;
        p->cabeza->anterior = nuevo;
        p->cabeza           = nuevo;
        return;
    }

    /* Buscar posición */
    Nodo* actual = p->cabeza;
    while (actual && actual->termino.exponente >= nuevo->termino.exponente) {
        actual = actual->siguiente;
    }

    /* Insertar al final */
    if (!actual) {
        nuevo->anterior    = p->cola;
        p->cola->siguiente = nuevo;
        p->cola            = nuevo;
        return;
    }

    /* Insertar en medio: antes de `actual` */
    nuevo->siguiente            = actual;
    nuevo->anterior             = actual->anterior;
    actual->anterior->siguiente = nuevo;
    actual->anterior            = nuevo;
}

void eliminarNodo(Polinomio* p, int exp) {
    if (!p || !p->cabeza) return;

    Nodo* actual = p->cabeza;
    while (actual) {
        if (actual->termino.exponente == exp) {
            if (actual->anterior)
                actual->anterior->siguiente = actual->siguiente;
            else
                p->cabeza = actual->siguiente;

            if (actual->siguiente)
                actual->siguiente->anterior = actual->anterior;
            else
                p->cola = actual->anterior;

            free(actual);
            return;
        }
        actual = actual->siguiente;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  POLINOMIO — operaciones
 * ═══════════════════════════════════════════════════════════════════════════ */

Polinomio* crearPolinomio() {
    Polinomio* p = (Polinomio*)malloc(sizeof(Polinomio));
    if (!p) return NULL;
    p->cabeza = NULL;
    p->cola   = NULL;
    return p;
}

void insertarTermino(Polinomio* p, float coef, int exp) {
    if (!p) return;

    /* Buscar término semejante */
    Nodo* actual = p->cabeza;
    while (actual) {
        if (actual->termino.exponente == exp) {
            actual->termino.coeficiente += coef;
            if (actual->termino.coeficiente == 0.0f)
                eliminarNodo(p, exp);
            return;
        }
        actual = actual->siguiente;
    }

    /* No existe: crear nodo nuevo */
    if (coef == 0.0f) return;
    Nodo* nuevo = crearNodo(coef, exp);
    if (!nuevo) return;
    insertarNodoOrdenado(p, nuevo);
}

float evaluarPolinomio(Polinomio* p, float x) {
    if (!p) return 0.0f;
    float resultado = 0.0f;
    Nodo* actual = p->cabeza;
    while (actual) {
        resultado += actual->termino.coeficiente *
                     (float)pow((double)x, actual->termino.exponente);
        actual = actual->siguiente;
    }
    return resultado;
}

Polinomio* sumarPolinomios(Polinomio* p1, Polinomio* p2) {
    Polinomio* resultado = crearPolinomio();
    if (!resultado) return NULL;

    Nodo* actual = p1->cabeza;
    while (actual) {
        insertarTermino(resultado,
                        actual->termino.coeficiente,
                        actual->termino.exponente);
        actual = actual->siguiente;
    }

    actual = p2->cabeza;
    while (actual) {
        insertarTermino(resultado,
                        actual->termino.coeficiente,
                        actual->termino.exponente);
        actual = actual->siguiente;
    }

    return resultado;
}

Polinomio* multiplicarPolinomios(Polinomio* p1, Polinomio* p2) {
    Polinomio* resultado = crearPolinomio();
    if (!resultado) return NULL;

    Nodo* a = p1->cabeza;
    while (a) {
        Nodo* b = p2->cabeza;
        while (b) {
            insertarTermino(resultado,
                            a->termino.coeficiente * b->termino.coeficiente,
                            a->termino.exponente   + b->termino.exponente);
            b = b->siguiente;
        }
        a = a->siguiente;
    }

    return resultado;
}

char* polinomioToString(Polinomio* p) {
    size_t capacidad = 256;
    char*  buf       = (char*)malloc(capacidad);
    if (!buf) return NULL;
    buf[0] = '\0';

    if (!p || !p->cabeza) {
        buf[0] = '0'; buf[1] = '\0';
        return buf;
    }

    int   primero = 1;
    Nodo* actual  = p->cabeza;

    while (actual) {
        float coef = actual->termino.coeficiente;
        int   exp  = actual->termino.exponente;
        char  tmp[64];

        if (exp == 0) {
            if (coef == (int)coef)
                snprintf(tmp, sizeof(tmp), "%d", (int)coef);
            else
                snprintf(tmp, sizeof(tmp), "%g", coef);

        } else if (exp == 1) {
            if      (coef ==  1.0f) snprintf(tmp, sizeof(tmp), "x");
            else if (coef == -1.0f) snprintf(tmp, sizeof(tmp), "-x");
            else if (coef == (int)coef)
                snprintf(tmp, sizeof(tmp), "%dx", (int)coef);
            else
                snprintf(tmp, sizeof(tmp), "%gx", coef);

        } else {
            if      (coef ==  1.0f) snprintf(tmp, sizeof(tmp), "x^%d", exp);
            else if (coef == -1.0f) snprintf(tmp, sizeof(tmp), "-x^%d", exp);
            else if (coef == (int)coef)
                snprintf(tmp, sizeof(tmp), "%dx^%d", (int)coef, exp);
            else
                snprintf(tmp, sizeof(tmp), "%gx^%d", coef, exp);
        }

        char signo[3] = "";
        if (!primero && coef > 0.0f) {
            signo[0] = '+'; signo[1] = '\0';
        }

        size_t necesario = strlen(buf) + strlen(signo) + strlen(tmp) + 1;
        if (necesario > capacidad) {
            capacidad = necesario * 2;
            char* nuevo = (char*)realloc(buf, capacidad);
            if (!nuevo) { free(buf); return NULL; }
            buf = nuevo;
        }

        strcat(buf, signo);
        strcat(buf, tmp);
        primero = 0;
        actual  = actual->siguiente;
    }

    return buf;
}

void destruirPolinomio(Polinomio* p) {
    if (!p) return;
    Nodo* actual = p->cabeza;
    while (actual) {
        Nodo* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    free(p);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  PRUEBAS
 * ═══════════════════════════════════════════════════════════════════════════ */

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
    /* Esperado: x^2+4 */

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
    printf("P1 antes:       %s\n", str); free(str);
    eliminarNodo(p1, 2);
    str = polinomioToString(p1);
    printf("P1 sin x^2:     %s\n", str); free(str);
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