#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "polinomio.h"
 
/*  Alumnos:
    Ramirez Olvera Erick Yael
    Rosas Salazar Jose Eduardo
    Gonzales Morales Daniel Alejandro
    Flores Alvarez Roberto Maximiliano
*/

/**
 * Crea un nodo nuevo con el coeficiente y exponente indicados.
 * Devuelve NULL si malloc falla.
 */
Nodo* crearNodo(float coef, int exp) {
    Nodo* n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) return NULL;
    n->termino.coeficiente = coef;
    n->termino.exponente   = exp;
    n->siguiente = NULL;
    n->anterior  = NULL;
    return n;
}
 
/**
 * Inserta `nuevo` en la lista doblemente ligada de `p` manteniendo
 * orden DESCENDENTE por exponente.
 *
 * Casos:
 *   1. Lista vacía  → nuevo es cabeza y cola.
 *   2. Mayor que la cabeza → se inserta al frente.
 *   3. Menor que la cola  → se inserta al final.
 *   4. En medio → se busca la posición correcta.
 */
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
        nuevo->anterior  = p->cola;
        p->cola->siguiente = nuevo;
        p->cola          = nuevo;
        return;
    }
 
    /* Insertar en medio: antes de `actual` */
    nuevo->siguiente          = actual;
    nuevo->anterior           = actual->anterior;
    actual->anterior->siguiente = nuevo;
    actual->anterior          = nuevo;
}
 
/**
 * Busca el nodo cuyo exponente coincide con `exp` y lo elimina
 * liberando su memoria. Si no existe, no hace nada.
 */
void eliminarNodo(Polinomio* p, int exp) {
    if (!p || !p->cabeza) return;
 
    Nodo* actual = p->cabeza;
    while (actual) {
        if (actual->termino.exponente == exp) {
            /* Actualizar enlace del anterior */
            if (actual->anterior)
                actual->anterior->siguiente = actual->siguiente;
            else
                p->cabeza = actual->siguiente;   /* era la cabeza */
 
            /* Actualizar enlace del siguiente */
            if (actual->siguiente)
                actual->siguiente->anterior = actual->anterior;
            else
                p->cola = actual->anterior;      /* era la cola */
 
            free(actual);
            return;
        }
        actual = actual->siguiente;
    }
}
 
/* ═══════════════════════════════════════════════════════════════════════════
 *  POLINOMIO – operaciones 
 * ═══════════════════════════════════════════════════════════════════════════ */
 
/**
 * Crea y devuelve un polinomio vacío (cabeza = cola = NULL).
 */
Polinomio* crearPolinomio() {
    Polinomio* p = (Polinomio*)malloc(sizeof(Polinomio));
    if (!p) return NULL;
    p->cabeza = NULL;
    p->cola   = NULL;
    return p;
}
 
/**
 * Inserta el término (coef, exp) en `p` manteniendo:
 *   - Orden descendente.
 *   - Simplificación: si ya existe un nodo con ese exponente,
 *     suma los coeficientes.
 *   - Limpieza: si el coeficiente resultante es 0, elimina el nodo.
 */
void insertarTermino(Polinomio* p, float coef, int exp) {
    if (!p) return;
 
    /* Buscar término semejante */
    Nodo* actual = p->cabeza;
    while (actual) {
        if (actual->termino.exponente == exp) {
            actual->termino.coeficiente += coef;
            /* Eliminar si coeficiente resulta cero */
            if (actual->termino.coeficiente == 0.0f)
                eliminarNodo(p, exp);
            return;
        }
        actual = actual->siguiente;
    }
 
    /* No existe: crear nodo nuevo (ignorar si coef == 0) */
    if (coef == 0.0f) return;
    Nodo* nuevo = crearNodo(coef, exp);
    if (!nuevo) return;
    insertarNodoOrdenado(p, nuevo);
}
 
/**
 * Evalúa el polinomio en x usando el esquema de Horner generalizado
 * (o simplemente iterando cada término con pow).
 */
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
 
/**
 * Suma p1 y p2 y devuelve un nuevo polinomio resultado.
 * El resultado queda ordenado y simplificado gracias a insertarTermino.
 */
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
 
/**
 * Multiplica p1 × p2 y devuelve un nuevo polinomio resultado.
 * Producto término a término; insertarTermino se encarga de
 * acumular términos semejantes y mantener el orden.
 */
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
 
/**
 * Convierte el polinomio a una cadena dinámica sin espacios,
 * ordenada de forma descendente, sin términos de coeficiente cero.
 *
 * Formato:
 *   - Término general : [coef]x^[exp]
 *   - Grado 1         : [coef]x
 *   - Grado 0         : [coef]
 *   - Coef == 1       : x^[exp]  (omite el 1 delante)
 *   - Coef == -1      : -x^[exp]
 *   - Signo + entre términos positivos no se imprime al inicio.
 *
 * El llamador es responsable de liberar la cadena con free().
 */
char* polinomioToString(Polinomio* p) {
    /* Buffer inicial; se realloca si hace falta */
    size_t capacidad = 256;
    char*  buf       = (char*)malloc(capacidad);
    if (!buf) return NULL;
    buf[0] = '\0';
 
    if (!p || !p->cabeza) {
        /* Polinomio vacío → "0" */
        buf[0] = '0'; buf[1] = '\0';
        return buf;
    }
 
    int   primero = 1;   /* para omitir '+' delante del primer término */
    Nodo* actual  = p->cabeza;
 
    while (actual) {
        float coef = actual->termino.coeficiente;
        int   exp  = actual->termino.exponente;
        char  tmp[64];
 
        /* Construir el fragmento del término en tmp */
        if (exp == 0) {
            /* Término independiente */
            if (coef == (int)coef)
                snprintf(tmp, sizeof(tmp), "%d", (int)coef);
            else
                snprintf(tmp, sizeof(tmp), "%g", coef);
 
        } else if (exp == 1) {
            /* Término lineal */
            if      (coef ==  1.0f) snprintf(tmp, sizeof(tmp), "x");
            else if (coef == -1.0f) snprintf(tmp, sizeof(tmp), "-x");
            else if (coef == (int)coef)
                snprintf(tmp, sizeof(tmp), "%dx", (int)coef);
            else
                snprintf(tmp, sizeof(tmp), "%gx", coef);
 
        } else {
            /* Término general */
            if      (coef ==  1.0f) snprintf(tmp, sizeof(tmp), "x^%d", exp);
            else if (coef == -1.0f) snprintf(tmp, sizeof(tmp), "-x^%d", exp);
            else if (coef == (int)coef)
                snprintf(tmp, sizeof(tmp), "%dx^%d", (int)coef, exp);
            else
                snprintf(tmp, sizeof(tmp), "%gx^%d", coef, exp);
        }
 
        /* Añadir signo '+' si no es el primero y el término es positivo */
        char signo[3] = "";
        if (!primero && coef > 0.0f) {
            signo[0] = '+'; signo[1] = '\0';
        }
 
        /* Redimensionar buffer si hace falta */
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
 
/**
 * Libera todos los nodos de la lista y luego la estructura Polinomio.
 */
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
