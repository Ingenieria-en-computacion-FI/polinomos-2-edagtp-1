/*
=========================================================
PRUEBAS PARA TAD POLINOMIO
=========================================================

Objetivos de las pruebas:
- Verificar estructura de lista doblemente ligada
- Verificar inserción ordenada
- Verificar simplificación de términos semejantes
- Verificar eliminación de términos con coeficiente cero
- Verificar suma de polinomios
- Verificar multiplicación
- Verificar evaluación numérica
- Verificar integridad de cabeza y cola
- Verificar recorridos anterior/siguiente
=========================================================
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../include/polinomio.h"

#define EPSILON 0.0001

int total = 0;
int passed = 0;

/* ===================================================== */
/* ASSERTS */
/* ===================================================== */

void assertTrue(int condition, const char* test) {
    total++;

    if (condition) {
        printf("[OK] %s\n", test);
        passed++;
    } else {
        printf("[ERROR] %s\n", test);
    }
}

void assertString(
    const char* obtained,
    const char* expected,
    const char* test
) {
    total++;

    if (strcmp(obtained, expected) == 0) {
        printf("[OK] %s\n", test);
        passed++;
    } else {
        printf("[ERROR] %s\n", test);
        printf("Esperado: %s\n", expected);
        printf("Obtenido: %s\n", obtained);
    }
}

void assertFloat(
    float obtained,
    float expected,
    const char* test
) {
    total++;

    if (fabs(obtained - expected) < EPSILON) {
        printf("[OK] %s\n", test);
        passed++;
    } else {
        printf("[ERROR] %s\n", test);
        printf("Esperado: %.2f\n", expected);
        printf("Obtenido: %.2f\n", obtained);
    }
}

/* ===================================================== */
/* PRUEBAS DE CREACION */
/* ===================================================== */

void testCrearPolinomio() {

    Polinomio* p = crearPolinomio();

    assertTrue(
        p != NULL,
        "Crear polinomio"
    );

    assertTrue(
        p->cabeza == NULL,
        "Cabeza inicial NULL"
    );

    assertTrue(
        p->cola == NULL,
        "Cola inicial NULL"
    );

    destruirPolinomio(p);
}

/* ===================================================== */
/* PRUEBAS DE INSERCION */
/* ===================================================== */

void testInsercionSimple() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 3, 2);

    assertString(
        polinomioToString(p),
        "3x^2",
        "Insercion simple"
    );

    destruirPolinomio(p);
}

void testOrdenDescendente() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 1, 1);
    insertarTermino(p, 5, 5);
    insertarTermino(p, 3, 3);

    assertString(
        polinomioToString(p),
        "5x^5+3x^3+x",
        "Orden descendente"
    );

    destruirPolinomio(p);
}

void testSimplificacionBasica() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 3, 2);
    insertarTermino(p, 5, 2);

    assertString(
        polinomioToString(p),
        "8x^2",
        "Simplificacion de terminos"
    );

    destruirPolinomio(p);
}

void testEliminacionPorCero() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 3, 2);
    insertarTermino(p, -3, 2);

    assertString(
        polinomioToString(p),
        "0",
        "Eliminacion por coeficiente cero"
    );

    destruirPolinomio(p);
}

void testInsercionCompleja() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 2, 1);
    insertarTermino(p, 4, 5);
    insertarTermino(p, -1, 1);
    insertarTermino(p, 7, 3);

    assertString(
        polinomioToString(p),
        "4x^5+7x^3+x",
        "Insercion compleja"
    );

    destruirPolinomio(p);
}

/* ===================================================== */
/* PRUEBAS DE LISTA DOBLE */
/* ===================================================== */

void testEnlacesDobles() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 5, 5);
    insertarTermino(p, 3, 3);
    insertarTermino(p, 1, 1);

    int correct =
        p->cabeza->siguiente->anterior == p->cabeza
        &&
        p->cola->anterior->siguiente == p->cola;

    assertTrue(
        correct,
        "Enlaces dobles correctos"
    );

    destruirPolinomio(p);
}

void testCabezaYCola() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 7, 7);
    insertarTermino(p, 1, 1);

    int correct =
        p->cabeza->termino.exponente == 7
        &&
        p->cola->termino.exponente == 1;

    assertTrue(
        correct,
        "Cabeza y cola correctas"
    );

    destruirPolinomio(p);
}

/* ===================================================== */
/* PRUEBAS DE ELIMINACION */
/* ===================================================== */

void testEliminarNodo() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 5, 5);
    insertarTermino(p, 3, 3);
    insertarTermino(p, 1, 1);

    eliminarNodo(p, 3);

    assertString(
        polinomioToString(p),
        "5x^5+x",
        "Eliminar nodo intermedio"
    );

    destruirPolinomio(p);
}

/* ===================================================== */
/* PRUEBAS DE SUMA */
/* ===================================================== */

void testSumaBasica() {

    Polinomio* p1 = crearPolinomio();
    Polinomio* p2 = crearPolinomio();

    insertarTermino(p1, 3, 2);
    insertarTermino(p1, 2, 1);

    insertarTermino(p2, 1, 2);
    insertarTermino(p2, 4, 0);

    Polinomio* res = sumarPolinomios(p1, p2);

    assertString(
        polinomioToString(res),
        "4x^2+2x+4",
        "Suma basica"
    );

    destruirPolinomio(p1);
    destruirPolinomio(p2);
    destruirPolinomio(res);
}

void testSumaConCancelacion() {

    Polinomio* p1 = crearPolinomio();
    Polinomio* p2 = crearPolinomio();

    insertarTermino(p1, 5, 2);
    insertarTermino(p2, -5, 2);

    Polinomio* res = sumarPolinomios(p1, p2);

    assertString(
        polinomioToString(res),
        "0",
        "Suma con cancelacion"
    );

    destruirPolinomio(p1);
    destruirPolinomio(p2);
    destruirPolinomio(res);
}

/* ===================================================== */
/* PRUEBAS DE MULTIPLICACION */
/* ===================================================== */

void testMultiplicacionBasica() {

    Polinomio* p1 = crearPolinomio();
    Polinomio* p2 = crearPolinomio();

    insertarTermino(p1, 1, 1);
    insertarTermino(p1, 1, 0);

    insertarTermino(p2, 1, 1);
    insertarTermino(p2, 1, 0);

    Polinomio* res = multiplicarPolinomios(p1, p2);

    assertString(
        polinomioToString(res),
        "x^2+2x+1",
        "Multiplicacion basica"
    );

    destruirPolinomio(p1);
    destruirPolinomio(p2);
    destruirPolinomio(res);
}

void testMultiplicacionConSimplificacion() {

    Polinomio* p1 = crearPolinomio();
    Polinomio* p2 = crearPolinomio();

    insertarTermino(p1, 2, 1);
    insertarTermino(p1, 1, 0);

    insertarTermino(p2, 3, 1);
    insertarTermino(p2, -1, 0);

    Polinomio* res = multiplicarPolinomios(p1, p2);

    assertString(
        polinomioToString(res),
        "6x^2+x-1",
        "Multiplicacion con simplificacion"
    );

    destruirPolinomio(p1);
    destruirPolinomio(p2);
    destruirPolinomio(res);
}

/* ===================================================== */
/* PRUEBAS DE EVALUACION */
/* ===================================================== */

void testEvaluacionBasica() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 3, 2);
    insertarTermino(p, 2, 1);
    insertarTermino(p, 1, 0);

    float val = evaluarPolinomio(p, 2);

    assertFloat(
        val,
        17,
        "Evaluacion basica"
    );

    destruirPolinomio(p);
}

void testEvaluacionCero() {

    Polinomio* p = crearPolinomio();

    insertarTermino(p, 5, 3);

    float val = evaluarPolinomio(p, 0);

    assertFloat(
        val,
        0,
        "Evaluacion en cero"
    );

    destruirPolinomio(p);
}

/* ===================================================== */
/* MAIN */
/* ===================================================== */

int main() {

    printf("====================================\n");
    printf("PRUEBAS TAD POLINOMIO\n");
    printf("====================================\n\n");

    testCrearPolinomio();

    testInsercionSimple();
    testOrdenDescendente();
    testSimplificacionBasica();
    testEliminacionPorCero();
    testInsercionCompleja();

    testEnlacesDobles();
    testCabezaYCola();

    testEliminarNodo();

    testSumaBasica();
    testSumaConCancelacion();

    testMultiplicacionBasica();
    testMultiplicacionConSimplificacion();

    testEvaluacionBasica();
    testEvaluacionCero();

    printf("\n====================================\n");
    printf("RESULTADO FINAL\n");
    printf("====================================\n");

    printf(
        "Pruebas superadas: %d/%d\n",
        passed,
        total
    );

    return 0;
}
