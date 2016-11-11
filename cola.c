#include "cola.h"
#include <stdlib.h>


/*Defino la estructura nodo*/
typedef struct nodo nodo_t;
struct nodo{
  void* dato;
  nodo_t* proximo;
};

/*Defino la estructura cola*/
struct cola{
  nodo_t* primero;
  nodo_t* ultimo;
};

/*Creo un nodo nuevo, aun si valor es NULL*/
nodo_t* cola_nodo_crear(void* valor){
  nodo_t* nodo = malloc(sizeof(nodo_t));

  if (!nodo){
    return NULL;
  }
  nodo->dato = valor;
  nodo->proximo = NULL;
  return nodo;
}

/*Creo una nueva cola */
cola_t* cola_crear(void){
  cola_t* cola = malloc(sizeof(cola_t));

  if (!cola){
    return NULL;
  }
  cola->primero = NULL;
  cola->ultimo = NULL;
  return cola;
}

/*Destruir una cola existente*/
void cola_destruir(cola_t *cola, void destruir_dato(void*)){
  if(!cola){
    return;
  }
  void* dato;

  while(!cola_esta_vacia(cola)){
    dato = cola_desencolar(cola);
    if(destruir_dato){ //verifico que no me pasen NULL en lugar de una función
      destruir_dato(dato);
    }
  }
  free(cola);
}

bool cola_esta_vacia(const cola_t *cola){

  if((!cola) || (!cola->primero)){
    return true;
  }
  return false;
}

/*Encola un elemento en la cola*/
bool cola_encolar(cola_t *cola, void* valor){
  nodo_t* nodo_nuevo = cola_nodo_crear(valor);

  if(!cola || !nodo_nuevo){
    free(nodo_nuevo);
    return false;
  }
  if(cola_esta_vacia(cola)){
    cola->primero = nodo_nuevo;
    cola->ultimo = nodo_nuevo;
  }
  else{
    cola->ultimo->proximo = nodo_nuevo;
    cola->ultimo = nodo_nuevo;
  }
  return true;
}

/*Ver el primer elemento de la cola*/
void* cola_ver_primero(const cola_t *cola){
  /*en la primitiva cola_esta_vacia, ya verifico if(!cola)*/
  if(cola_esta_vacia(cola)){
    return NULL;
  }
  return cola->primero->dato;
}

/*Desencolo un elemento de la cola*/
void* cola_desencolar(cola_t *cola){
  /*en la primitiva cola_esta_vacia, ya verifico if(!cola)*/
  if(cola_esta_vacia(cola)){
    return NULL;
  }
  nodo_t* nodo_aux;
  void* dato;

  nodo_aux = cola->primero;
  dato = nodo_aux->dato; //almaceno el dato, para luego devolverlo
  cola->primero = nodo_aux->proximo;
  free(nodo_aux);
  if (cola_esta_vacia(cola)){
    cola->ultimo = NULL;
  }
  return dato;
}
