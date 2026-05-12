#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/polinomio.h"

#define MAX_LINE 256
#define EPSILON 0.0001

int total = 0;
int passed = 0;

/* ===================================================== */
/* ASSERTS */
/* ===================================================== */

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
/* UTILIDADES */
/* ===================================================== */

void insertTerm(
    Polinomio* p,
    float coef,
    int exp
) {
    insertarTermino(p, coef, exp);
}

Polinomio* buildPolynomialFromSection(
    FILE* file
) {

    Polinomio* p = crearPolinomio();

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file)) {

        if (
            strstr(line, "expected")
            || strstr(line, "p2:")
            || strstr(line, "x:")
            || strstr(line, "type:")
            || strstr(line, "name:")
        ) {
            fseek(file, -strlen(line), SEEK_CUR);
            break;
        }

        float coef;
        int exp;

        if (
            sscanf(
                line,
                " - {coef: %f, exp: %d}",
                &coef,
                &exp
            ) == 2
        ) {

            insertTerm(p, coef, exp);
        }
    }

    return p;
}

/* ===================================================== */
/* PARSER Y EJECUCION */
/* ===================================================== */

void runTests(const char* filename) {

    FILE* file = fopen(filename, "r");

    if (!file) {

        printf("No se pudo abrir %s\n", filename);
        return;
    }

    char line[MAX_LINE];

    char name[100];
    char type[50];
    char expected[200];

    float expectedValue = 0;
    float x = 0;

    while (fgets(line, sizeof(line), file)) {

        if (strstr(line, "- name:")) {

            sscanf(line, " - name: %s", name);

            fgets(line, sizeof(line), file);
            sscanf(line, " type: %s", type);

            /* ========================================= */
            /* INSERT */
            /* ========================================= */

            if (strcmp(type, "insert") == 0) {

                Polinomio* p = crearPolinomio();

                while (fgets(line, sizeof(line), file)) {

                    if (strstr(line, "expected:")) {

                        sscanf(
                            line,
                            " expected: \"%[^\"]\"",
                            expected
                        );

                        break;
                    }

                    float coef;
                    int exp;

                    if (
                        sscanf(
                            line,
                            " - {coef: %f, exp: %d}",
                            &coef,
                            &exp
                        ) == 2
                    ) {

                        insertarTermino(
                            p,
                            coef,
                            exp
                        );
                    }
                }

                assertString(
                    polinomioToString(p),
                    expected,
                    name
                );

                destruirPolinomio(p);
            }

            /* ========================================= */
            /* SUMA */
            /* ========================================= */

            else if (strcmp(type, "sum") == 0) {

                Polinomio* p1;
                Polinomio* p2;

                while (fgets(line, sizeof(line), file)) {

                    if (strstr(line, "p1:")) {
                        p1 = buildPolynomialFromSection(file);
                    }

                    if (strstr(line, "p2:")) {
                        p2 = buildPolynomialFromSection(file);
                    }

                    if (strstr(line, "expected:")) {

                        sscanf(
                            line,
                            " expected: \"%[^\"]\"",
                            expected
                        );

                        break;
                    }
                }

                Polinomio* res =
                    sumarPolinomios(p1, p2);

                assertString(
                    polinomioToString(res),
                    expected,
                    name
                );

                destruirPolinomio(p1);
                destruirPolinomio(p2);
                destruirPolinomio(res);
            }

            /* ========================================= */
            /* MULTIPLICACION */
            /* ========================================= */

            else if (
                strcmp(type, "multiply") == 0
            ) {

                Polinomio* p1;
                Polinomio* p2;

                while (fgets(line, sizeof(line), file)) {

                    if (strstr(line, "p1:")) {
                        p1 = buildPolynomialFromSection(file);
                    }

                    if (strstr(line, "p2:")) {
                        p2 = buildPolynomialFromSection(file);
                    }

                    if (strstr(line, "expected:")) {

                        sscanf(
                            line,
                            " expected: \"%[^\"]\"",
                            expected
                        );

                        break;
                    }
                }

                Polinomio* res =
                    multiplicarPolinomios(p1, p2);

                assertString(
                    polinomioToString(res),
                    expected,
                    name
                );

                destruirPolinomio(p1);
                destruirPolinomio(p2);
                destruirPolinomio(res);
            }

            /* ========================================= */
            /* EVALUACION */
            /* ========================================= */

            else if (
                strcmp(type, "evaluate") == 0
            ) {

                Polinomio* p1;

                while (fgets(line, sizeof(line), file)) {

                    if (strstr(line, "p1:")) {
                        p1 = buildPolynomialFromSection(file);
                    }

                    if (strstr(line, "x:")) {
                        sscanf(line, " x: %f", &x);
                    }

                    if (strstr(line, "expected_value:")) {

                        sscanf(
                            line,
                            " expected_value: %f",
                            &expectedValue
                        );

                        break;
                    }
                }

                float result =
                    evaluarPolinomio(p1, x);

                assertFloat(
                    result,
                    expectedValue,
                    name
                );

                destruirPolinomio(p1);
            }
        }
    }

    fclose(file);
}

/* ===================================================== */
/* MAIN */
/* ===================================================== */

int main() {

    printf("====================================\n");
    printf("AUTOGRADING TAD POLINOMIO\n");
    printf("====================================\n\n");

    runTests("tests/cases.yaml");

    printf("\n====================================\n");
    printf("RESULTADO FINAL\n");
    printf("====================================\n");

    printf(
        "Pruebas superadas: %d/%d\n",
        passed,
        total
    );

    if (passed != total) {
        return 1;
    }

    return 0;
}
