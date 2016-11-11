#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/
// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un nuevo elemento en la primera posición de la lista.
// Devuelve falso en caso de error. Funciona en tiempo constante.
// Pre: la lista fue creada.
// Post: se insertó un nuevo elemento al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un nuevo elemento al final de la lista. Devuelve falso en
// caso de error. Funciona en tiempo constante.
// Pre: la lista fue creada.
// Post: se insertó un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista. Si está vacía devuelve NULL, caso
// contrario devuelve el valor del elemento eliminado.
// Pre: la lista fue creada.
// Post: se eliminó el primer elemento de la lista.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el primer elemento de la lista.
// Pre: la lista fue creada.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el ultimo elemento de la lista.
// Pre: la lista fue creada.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad de elementos de la lista. Funciona en tiempo constante.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista y la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/
//Recibe una lista como parametro.
//Pre: la lista fue creada.
//Post: devuelve un iterador que apunta al primer elemento.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza una posicion en la lista
// Pre: el iterador fue creado.
// Post: se avanzó una posición en el iterador. Devuelve false si esta al final.
bool lista_iter_avanzar(lista_iter_t *iter);

//Pre: el iterador fue creado.
//Post: devuelve un puntero  al valor de la posicion actual del iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Pre: el iterador fue creado.
//Post: devuelve true si esta al final de la lista.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
//Pre: el iterador fue creado.
//Post: se destruyo el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un nuevo elemento a la lista en la posición donde se encuentre
// el iterador. Devuelve true si pudo realizarlo, false en caso contrario.
// Pre: la lista y el iterador fueron creados.
// Post: se insertó un nuevo elemento a la lista.
bool lista_iter_insertar(lista_iter_t *iter, void* dato);

// Elimina el elemento de la lista al que apunte el iterador.
// Pre: la lista y el iterador fueron creados.
// Post: se eliminó el elemento de la lista.
void* lista_iter_borrar(lista_iter_t *iter);

/* ******************************************************************
 *                 PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/

 // La función de callback "visitar" recibe el dato y un puntero extra,
 // se la aplica a cada dato de la lista hasta que se llegue al final o "visitar"
 // devuelva false.
 // Pre: la lista fue creada.
 void lista_iterar(lista_t *lista, bool visitar(void*, void*), void *extra);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // LISTA_H
