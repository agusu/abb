#include "pila.h"
#define TAM_INICIAL_PILA 5
#define TAM_REDIMENSIONAR 2
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t capacidad;
    size_t cantidad;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

bool pila_redimensionar(pila_t* pila, size_t capacidad_nueva){
  void **datos_nuevos;

  datos_nuevos = realloc(pila->datos, capacidad_nueva * sizeof(void*));
  if (!datos_nuevos){
    return false;
  }
  pila->datos = datos_nuevos;
  pila->capacidad = capacidad_nueva;
  return true;
}

pila_t* pila_crear(void){
  pila_t* pila = malloc(sizeof(pila_t));

  if (!pila){
    return NULL;
  }
  pila->datos = malloc(TAM_INICIAL_PILA * sizeof(void*));
  if (!pila->datos){
    free(pila);
    return NULL;
  }
  pila->cantidad = 0;
  pila->capacidad = TAM_INICIAL_PILA; //lo definí como 5
  return pila;
}

void pila_destruir(pila_t *pila){

    free(pila->datos);
    free(pila);
}

bool pila_apilar(pila_t *pila, void* valor){

  if (!pila){
    return false;
  }
  /*se fija si hay espacio suficiente para apilar otro elemento,
  en caso contrario, redimensiona la pila*/
  if (pila->capacidad == pila->cantidad){
    // TAM_REDIMENSIONAR lo defini como 2
    if (!pila_redimensionar(pila, pila->capacidad * TAM_REDIMENSIONAR)){
      return false;
    }
  }
  pila->cantidad += 1;
  pila->datos[pila->cantidad -1] = valor;
  return true;
}

bool pila_esta_vacia(const pila_t *pila){

  if (!pila){
    return true;
  }
  return pila->cantidad == 0;
}

void* pila_ver_tope(const pila_t *pila){

  if (pila_esta_vacia(pila)){
    return NULL;
  }
  return pila->datos[pila->cantidad-1];
}
/* Desapilar de la pila*/
void* pila_desapilar(pila_t *pila){
  void* dato;

  if (pila_esta_vacia(pila)){
    return NULL;
  }
  dato = pila->datos[pila->cantidad -1];
  /*se fija si hay que redimensionar la pila*/
  if ( pila->cantidad * 4 <= pila->capacidad && pila->capacidad == TAM_INICIAL_PILA){
    if ((pila->capacidad / TAM_REDIMENSIONAR) > TAM_INICIAL_PILA){
      pila_redimensionar(pila, pila->capacidad / TAM_REDIMENSIONAR);
    }
    else{
      pila_redimensionar(pila, TAM_INICIAL_PILA);
    }
  }
  pila->cantidad -= 1;
  return dato;
}
