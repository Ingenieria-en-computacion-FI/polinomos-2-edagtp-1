#ifndef __POLINOMIO_H__
#define __POLINOMIO_H__
 
typedef struct Termino {
    float coeficiente;
    int   exponente;
} Term;
 
typedef struct Nodo {
    Term         termino;
    struct Nodo* siguiente;
    struct Nodo* anterior;
} Nodo;
 
typedef struct {
    Nodo* cabeza;
    Nodo* cola;
} Polinomio;
 
/* ── Lista ─────────────────────────────────────────────────────────────── */
 
/**
 * @brief Crea un nuevo nodo para un término del polinomio.
 *
 * Reserva memoria dinámicamente para un nodo e inicializa
 * sus campos con el coeficiente y exponente proporcionados.
 * Los enlaces siguiente y anterior se inicializan en NULL.
 *
 * @param coef Coeficiente del término.
 * @param exp  Exponente del término.
 * @return Puntero al nodo creado.
 */
Nodo* crearNodo(float coef, int exp);
 
/**
 * @brief Inserta un nodo en el polinomio manteniendo
 *        el orden descendente por exponente.
 *
 * La función actualiza correctamente los enlaces
 * de la lista doblemente ligada.
 *
 * @param p     Polinomio destino.
 * @param nuevo Nodo a insertar.
 */
void insertarNodoOrdenado(Polinomio* p, Nodo* nuevo);
 
/**
 * @brief Elimina un nodo del polinomio según su exponente.
 *
 * La función libera la memoria asociada al nodo
 * eliminado y actualiza correctamente los enlaces.
 *
 * @param p   Polinomio del cual se eliminará el nodo.
 * @param exp Exponente del término a eliminar.
 */
void eliminarNodo(Polinomio* p, int exp);
 
/* ── Polinomio ──────────────────────────────────────────────────────────── */
 
/**
 * @brief Crea un polinomio vacío.
 *
 * Inicializa la estructura del polinomio con cabeza
 * y cola en NULL.
 *
 * @return Puntero al polinomio creado.
 */
Polinomio* crearPolinomio();
 
/**
 * @brief Inserta un término en el polinomio.
 *
 * La inserción mantiene:
 * - Orden descendente por exponente.
 * - Simplificación automática de términos semejantes.
 * - Eliminación de términos con coeficiente cero.
 *
 * @param p    Polinomio destino.
 * @param coef Coeficiente del término.
 * @param exp  Exponente del término.
 */
void insertarTermino(Polinomio* p, float coef, int exp);
 
/**
 * @brief Evalúa el polinomio para un valor dado de x.
 *
 * Calcula el valor numérico del polinomio sustituyendo
 * x en cada término.
 *
 * @param p Polinomio a evaluar.
 * @param x Valor de la variable independiente.
 * @return  Resultado de la evaluación.
 */
float evaluarPolinomio(Polinomio* p, float x);
 
/**
 * @brief Suma dos polinomios.
 *
 * El polinomio resultante se mantiene ordenado
 * y simplificado automáticamente.
 *
 * @param p1 Primer polinomio.
 * @param p2 Segundo polinomio.
 * @return   Nuevo polinomio resultado de la suma.
 */
Polinomio* sumarPolinomios(Polinomio* p1, Polinomio* p2);
 
/**
 * @brief Multiplica dos polinomios.
 *
 * El polinomio resultante se mantiene ordenado
 * y simplificado automáticamente.
 *
 * @param p1 Primer polinomio.
 * @param p2 Segundo polinomio.
 * @return   Nuevo polinomio resultado de la multiplicación.
 */
Polinomio* multiplicarPolinomios(Polinomio* p1, Polinomio* p2);
 
/**
 * @brief Convierte un polinomio a representación textual.
 *
 * El formato:
 * - Está ordenado de forma descendente.
 * - No contiene espacios.
 * - No muestra términos con coeficiente cero.
 *
 * Ejemplo: 3x^2+2x+1
 *
 * @param p Polinomio a convertir.
 * @return  Cadena dinámica con la representación del polinomio.
 */
char* polinomioToString(Polinomio* p);
 
/**
 * @brief Libera toda la memoria asociada a un polinomio.
 *
 * Libera:
 * - Todos los nodos de la lista.
 * - La estructura del polinomio.
 *
 * @param p Polinomio a destruir.
 */
void destruirPolinomio(Polinomio* p);
 
#endif /* __POLINOMIO_H__ */ 
