#include "lista.h"
#include <stdbool.h>
#include <stdlib.h>


typedef struct nodo nodo_t;
struct nodo{
  void* dato;
  nodo_t* proximo;
};

struct lista{
  nodo_t* primero;
  nodo_t* ultimo;
  size_t largo;
};

struct lista_iter{
	nodo_t* actual;
  nodo_t* anterior;
	lista_t* lista;
};

nodo_t *nodo_crear(void* dato){
  nodo_t* nodo = malloc(sizeof(nodo_t));
  if(!nodo){
    return NULL;
  }
  nodo->dato = dato;
  nodo->proximo = NULL;
  return nodo;
}

/* ***************************************************************************
 *                          PRIMITIVAS DE LA LISTA
 * ***************************************************************************/

lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
  if(!lista) return NULL;
  lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	if(!lista || !lista->primero){
		return true;
	}
	return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
  if(!lista) return false;
  nodo_t* nodo_nuevo = nodo_crear(dato);
	if(!nodo_nuevo) return false;
  if(lista_esta_vacia(lista)){
    lista->ultimo = nodo_nuevo;
  }
	nodo_nuevo->proximo = lista->primero;
	lista->primero = nodo_nuevo;
	lista->largo ++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
  if(!lista) return false;
  nodo_t* nodo_nuevo = nodo_crear(dato);
	if(!nodo_nuevo) return false;
	if(lista_esta_vacia(lista)){
		lista->primero = nodo_nuevo;
	}
	else{
		lista->ultimo->proximo = nodo_nuevo;
	}
  lista->ultimo = nodo_nuevo;
	lista->largo ++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista)){//ya verifica (!lista) en lista_esta_vacia.
		return NULL;
	}
	nodo_t* nodo_aux = lista->primero;
	void* dato = lista->primero->dato;
  lista->primero = lista->primero->proximo;
	free(nodo_aux);
	if(lista_esta_vacia(lista)){
		lista->ultimo = NULL;
	}
	lista->largo --;
	return dato;
}

void *lista_ver_primero(const lista_t *lista){
	if(lista_esta_vacia(lista)){//ya verifica (!lista) en lista_esta_vacia.
		return NULL;
	}
	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista)){//ya verifica (!lista) en lista_esta_vacia.
		return NULL;
	}
	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	if(lista_esta_vacia(lista)){//ya verifica (!lista) en lista_esta_vacia.
		return 0;
	}
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	void* dato;
	while(!lista_esta_vacia(lista)){//ya verifica (!lista) en lista_esta_vacia.
		dato = lista_borrar_primero(lista);
		if(destruir_dato){
			destruir_dato(dato);
		}
	}
	free(lista);//si es NULL no hace nada.
}
/* ***************************************************************************
 *                          PRIMITIVAS DEL ITERADOR
 * ***************************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
  if(!lista) return NULL;
  lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter) return NULL;
	iter->lista = lista;
  iter->actual = lista->primero; //si esta vacia lista->primero = lista->ultimo = NULL.
  iter->anterior = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(lista_iter_al_final(iter)) return false;
  iter->anterior = iter->actual;
  iter->actual = iter->actual->proximo;
  return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
  if(lista_iter_al_final(iter)) return NULL;
  return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	if(!iter || !iter->actual) return true;
	return false;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void* dato){
  if(!iter) return false;
  nodo_t* nodo_nuevo = nodo_crear(dato);
  if(!nodo_nuevo) return false;
  if(iter->actual == iter->lista->primero)
    iter->lista->primero = nodo_nuevo;
  else{
    if(lista_iter_al_final(iter))
      iter->lista->ultimo = nodo_nuevo;
    iter->anterior->proximo = nodo_nuevo;
  }
  nodo_nuevo->proximo = iter->actual;
  iter->actual = nodo_nuevo;
  iter->lista->largo ++;
  return true;
}

void* lista_iter_borrar(lista_iter_t *iter){
  if(lista_iter_al_final(iter)) return NULL;
  void* dato= iter->actual->dato;
  nodo_t* nodo_aux = iter->actual;
  iter->actual = iter->actual->proximo;
  if(nodo_aux == iter->lista->primero){
    iter->lista->primero = iter->actual;
  }
  else{
    if(lista_iter_al_final(iter)){
      iter->lista->ultimo = iter->anterior;
    }
    iter->anterior->proximo = iter->actual;
  }
  free(nodo_aux);
  iter->lista->largo --;
  return dato;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
  if(!lista) return;
  nodo_t* nodo = lista->primero;
  while(nodo && visitar(nodo->dato, extra)){
    nodo = nodo->proximo;
  }
}
