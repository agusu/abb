#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <string.h>


typedef struct nodo_abb{
	const char* clave;
	void* dato;
	struct nodo_abb* izq;
	struct nodo_abb* der;
}nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	abb_destruir_dato_t destructor;
	size_t cantidad;
	abb_comparar_clave_t cmp;
};

typedef struct abb_iter{
	const abb_t* arbol;
	pila_t* pila;
}abb_iter_t;

typedef struct abb_iter_post{
	const abb_t* arbol;
	pila_t* pila;
}abb_iter_post_t;

typedef struct abb_item{
    const char* clave;
    void* valor;
} abb_item_t;


/******************************************************************
 *		                  FUNCIONES AUXILIARES                      *
 ******************************************************************/

char *strdup (const char *clave) {
	char *copia_clave = malloc (strlen (clave) + 1);
  if (copia_clave) strcpy (copia_clave,clave);
  return copia_clave;
}

nodo_abb_t* nodo_abb_crear(const char* clave, void* dato){
  nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
  if (!nodo) return NULL;
  nodo->dato = dato;
  nodo->clave = strdup(clave);
  if (!nodo->clave){
    free(nodo);
	return NULL;
  }
  nodo->der = NULL;
  nodo->izq = NULL;
  return nodo;
}

nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t cmp, const char* clave, nodo_abb_t** padre){
  if (!nodo) return NULL;
  int resultado = cmp(nodo->clave, clave);
	if (resultado == 0) return nodo;
	*padre = nodo;
	if (resultado > 0) return buscar_nodo(nodo->izq, cmp, clave, padre);
	else return buscar_nodo(nodo->der, cmp, clave, padre);
}

// ---- Primitivas auxiliares a abb_guardar ---- //

bool _abb_guardar(nodo_abb_t* padre, const char* clave, void* dato, int lado){
  // Crea nodo y lo guarda.
  nodo_abb_t* hoja = nodo_abb_crear(clave,dato);
  if (!hoja) return false;
  if (lado > 0) padre->izq = hoja;
  else padre->der = hoja;
  return true;
}

bool _abb_reemplazar_nodo(abb_t* arbol, nodo_abb_t* actual, void* dato){
  //Caso clave preexistente
	if(arbol->destructor) arbol->destructor(actual->dato);
	actual->dato = dato;
  arbol->cantidad--;
  return true;
}

bool _abb_guardar_rec(abb_t* arbol, nodo_abb_t* actual, const char* clave, void* dato){
	// Primitiva auxiliar recursiva
	int res = arbol->cmp(actual->clave, clave);
	if(res == 0) return _abb_reemplazar_nodo(arbol, actual, dato);
	if(res > 0){
		if (!actual->izq) return _abb_guardar(actual, clave, dato, res);
		else return _abb_guardar_rec(arbol, actual->izq, clave, dato);
	}
	else{
		if(!actual->der) return _abb_guardar(actual, clave, dato, res);
		else return _abb_guardar_rec(arbol, actual->der, clave, dato);
	}
}

// --------- Primitivas auxiliares a abb_borrar ----------- //

int _cant_hijos(nodo_abb_t* nodo){
	if(nodo->der && nodo->izq) return 2;
 	else{
 		if(!nodo->der && !nodo->izq) return 0;
 		else return 1;
 	}
}

void *_borrar_hoja(nodo_abb_t* nodo){
  void* dato = nodo->dato;
  free((void*)nodo->clave);
  free(nodo);
  return dato;
 }

nodo_abb_t *_buscar_max_izq(nodo_abb_t* nodo, nodo_abb_t** padre){
 	if(!nodo->der) return nodo;
 	*padre = nodo;
 	return _buscar_max_izq(nodo->der, padre);
}

void swap(nodo_abb_t* nodo, nodo_abb_t* nodo_aux){
	const char* clave_aux = nodo_aux->clave;
	void* dato_aux = nodo_aux->dato;
	nodo_aux->clave = nodo->clave;
	nodo_aux->dato = nodo->dato;
	nodo->clave = clave_aux;
	nodo->dato = dato_aux;
}

void cambiar_raiz(nodo_abb_t** raiz, int cant_hijos){
	switch(cant_hijos){
		case 0:
			*raiz = NULL;
			break;

		case 1:
			if((*raiz)->der) *raiz = (*raiz)->der;
			else *raiz = (*raiz)->izq;
	}
}

void *_abb_borrar(nodo_abb_t* nodo, nodo_abb_t* padre, nodo_abb_t** raiz){
	int cant_hijos = _cant_hijos(nodo);
	void* dato = NULL;
	nodo_abb_t* raiz_aux = *raiz;
	if(nodo == padre) cambiar_raiz(&raiz_aux, cant_hijos);

	nodo_abb_t* padre_aux;
	nodo_abb_t* nodo_aux;
	switch(cant_hijos){
		case 0://Borrar sin hijos.
			if (padre->der == nodo) padre->der = NULL;
			else padre->izq = NULL;
			dato = _borrar_hoja(nodo);
			break;

	  case 1://Borrar con 1 hijo.
			if (nodo->der){
				if(padre->izq == nodo) padre->izq = nodo->der;
				else padre->der = nodo->der;
			}
			else{
				if(padre->izq == nodo) padre->izq = nodo->izq;
				else padre->der = nodo->izq;
			}
			dato = _borrar_hoja(nodo);
			break;

		default: //Borrar con 2 hijos.
			padre_aux = nodo;
			nodo_aux = _buscar_max_izq(nodo->izq, &padre_aux);
			swap(nodo, nodo_aux);
			dato = _abb_borrar(nodo_aux, padre_aux, raiz);

	}
	*raiz = raiz_aux;
	return dato;
}

// --------- Primitivas auxiliares a abb_destruir ----------- //

void _abb_destruir(nodo_abb_t* nodo, abb_destruir_dato_t destructor){
	// Primitiva recursiva auxiliar
	if (!nodo) return;
	_abb_destruir(nodo->izq, destructor);
	_abb_destruir(nodo->der, destructor);
	void *dato = _borrar_hoja(nodo);
	if (destructor) destructor(dato);
 }

/******************************************************************
 *			                  PRIMITIVAS ABB                          *
 ******************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));
  if (!abb) return NULL;
  abb->raiz = NULL;
  abb->destructor = destruir_dato;
  abb->cmp = cmp;
  abb->cantidad = 0;
  return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if (!arbol || !clave) return false;
	nodo_abb_t* raiz = arbol->raiz;
	if (!raiz) { //arbol vacio
		raiz = nodo_abb_crear(clave, dato);
		if (!raiz) return false;
		arbol->raiz = raiz;
		arbol->cantidad++;
		return true;
	}

  if(!_abb_guardar_rec(arbol, raiz, clave, dato)) return false;
  arbol->cantidad++;
  return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if(!arbol->raiz || !clave) return NULL;
	nodo_abb_t *padre = arbol->raiz;
	nodo_abb_t* nodo = buscar_nodo(padre, arbol->cmp, clave, &padre);
	if(!nodo) return NULL;
	void *dato = _abb_borrar(nodo, padre, &(arbol->raiz));
	arbol->cantidad--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	if(!arbol->raiz || !clave) return NULL;
	nodo_abb_t* raiz = arbol->raiz;
	nodo_abb_t* nodo = buscar_nodo(arbol->raiz, arbol->cmp, clave, &raiz);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(!arbol->raiz) return NULL;
	nodo_abb_t* raiz = arbol->raiz;
	if (!buscar_nodo(arbol->raiz, arbol->cmp, clave, &raiz))
		return false;
	return true;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
	_abb_destruir(arbol->raiz, arbol->destructor);
	free(arbol);
}

/******************************************************************
 *			                ITERADOR EXTERNO ABB                      *
 ******************************************************************/

void abb_iterar_izq(pila_t *pila, nodo_abb_t *nodo_abb){
	if(!nodo_abb)
		return;
	pila_apilar(pila, nodo_abb);
	abb_iterar_izq(pila, nodo_abb->izq);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t *iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;
	iter->pila = pila_crear();
	if(!iter->pila){
		free(iter);
		return NULL;
	}
	iter->arbol = arbol;
	abb_iterar_izq(iter->pila, iter->arbol->raiz);
	return iter;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(abb_iter_in_al_final(iter))
		return NULL;
  nodo_abb_t* tope = pila_ver_tope(iter->pila);
	return tope->clave;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if(abb_iter_in_al_final(iter))
		return false;
  nodo_abb_t* tope = pila_desapilar(iter->pila);
	abb_iterar_izq(iter->pila, tope->der);
	return true;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}

/******************************************************************
 *			               ITERADOR INTERNO ABB                       *
 ******************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	pila_t* pila = pila_crear();
	if(!pila)
		return;
	abb_iterar_izq(pila, arbol->raiz);
	nodo_abb_t *actual = pila_desapilar(pila);
	while(actual && visitar(actual->clave, actual->dato, extra)){
		abb_iterar_izq(pila, actual->der);
		actual = pila_desapilar(pila);
	}
	pila_destruir(pila);
}


/******************************************************************
 *			            ITERADOR EXTERNO POST ORDEN ABB               *
 ******************************************************************/

bool es_hijo_izq(nodo_abb_t *desapilado, nodo_abb_t *tope_nuevo){
	if(!tope_nuevo || !tope_nuevo->izq) return false;
	return strcmp(desapilado->clave, tope_nuevo->izq->clave) == 0;
}

void abb_traza_izq(pila_t *pila, nodo_abb_t *nodo_abb){
	if(!nodo_abb) return;
	pila_apilar(pila, nodo_abb);
	if(nodo_abb->izq) abb_traza_izq(pila, nodo_abb->izq);
	else abb_traza_izq(pila, nodo_abb->der);
}

abb_iter_post_t *abb_iter_post_crear(const abb_t *arbol){
	abb_iter_post_t *iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;
	iter->pila = pila_crear();
	if(!iter->pila){
		free(iter);
		return NULL;
	}
	iter->arbol = arbol;
	abb_traza_izq(iter->pila, iter->arbol->raiz);
	return iter;
}

bool abb_iter_post_al_final(const abb_iter_post_t* iter){
	return pila_esta_vacia(iter->pila);
}

const char* abb_iter_post_ver_actual(const abb_iter_post_t* iter){
	if(abb_iter_post_al_final(iter)) return NULL;
	nodo_abb_t* tope = pila_ver_tope(iter->pila);
	return tope->clave;
}

bool abb_iter_post_avanzar(abb_iter_post_t* iter){
	if(abb_iter_post_al_final(iter)) return false;
	nodo_abb_t *desapilado = pila_desapilar(iter->pila);
	if(es_hijo_izq(desapilado, pila_ver_tope(iter->pila)))
		abb_traza_izq(iter->pila, ((nodo_abb_t *)pila_ver_tope(iter->pila))->der);
	return true;
}

void abb_iter_post_destruir(abb_iter_post_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}

/******************************************************************
 *			           ITERADOR INTERNO  POST ORDEN ABB               *
 ******************************************************************/

void abb_post_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	pila_t* pila = pila_crear();
	if(!pila) return;
	abb_traza_izq(pila, arbol->raiz);
	nodo_abb_t *actual = pila_desapilar(pila);
	while(actual && visitar(actual->clave, actual->dato, extra)){
		if(es_hijo_izq(actual, pila_ver_tope(pila)))
			abb_traza_izq(pila, ((nodo_abb_t *)pila_ver_tope(pila))->der);
		actual = pila_desapilar(pila);
	}
	pila_destruir(pila);
}

abb_item_t _abb_nodo_a_item(nodo_abb_t* nodo){
    abb_item_t item;
    item.clave = nodo->clave;
    item.valor = nodo->dato;
    return item;
}

abb_item_t* abb_obtener_items(abb_t* abb){
    abb_item_t* lista = malloc(sizeof(abb_item_t)*abb_cantidad(abb));
    pila_t* pila = pila_crear();
    if (!lista || !pila) return lista;
    abb_iterar_izq(pila,abb->raiz);
    size_t i = 0;
    nodo_abb_t* actual;
    while (!pila_esta_vacia(pila)){
        actual = pila_desapilar(pila);
        lista[i] = _abb_nodo_a_item(actual);
        abb_iterar_izq(pila,actual->der);
        i++;
    }
    pila_destruir(pila);
    return lista;
}
