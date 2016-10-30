#include "abb.h"
#include <stdio.h>


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

nodo_abb_t* nodo_abb_crear(const char* clave, void* dato){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
    if (!nodo) return NULL;
    nodo->dato = dato;
    nodo->clave = clave;//habría que hacer un strdup(copiar la clave)
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t cmp, const char* clave, nodo_abb_t** padre){
    if (!nodo) return NULL;
    int resultado = cmp(nodo->clave, clave);
    if (resultado == 0) return nodo;
    *padre = nodo;
    if (resultado > 0)
        return buscar_nodo(nodo->izq, cmp, clave, &nodo);
    return buscar_nodo(nodo->der, cmp, clave, &nodo);
}

/*
nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t cmp, const char* clave, nodo_abb_t** padre){
    if (!nodo) return NULL;
    int resultado = cmp(nodo->clave,clave);
	nodo_abb_t *nodo_devolver = nodo;
    switch(resultado){
		case -1: nodo_devolver = buscar_nodo(nodo->der,cmp,clave,&nodo);
				break;
		case 1:	nodo_devolver = buscar_nodo(nodo->izq, cmp, clave, &nodo);
				break;
	}
	return nodo_devolver;
}
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;
    abb->raiz = NULL;
    abb->destructor = destruir_dato;
    abb->cmp = cmp;
    abb->cantidad = 0;
    return abb;
}
// ---- Primitivas auxiliares a abb_guardar ---- //

bool _abb_guardar(nodo_abb_t* padre, const char* clave, void* dato, int lado){
    // Crea nodo y lo guarda.
    nodo_abb_t* hoja = nodo_abb_crear(clave,dato);
    if (!hoja) return false;
    if (lado == -1) padre->der = hoja;
    else padre->izq = hoja;
    return true;
}

bool _abb_reemplazar_nodo(abb_t* arbol, const char* clave, void* dato){
    // Caso clave preexistente
    //nodo_abb* raiz = arbol->raiz;
    nodo_abb_t* aux = buscar_nodo(arbol->raiz, arbol->cmp, clave, &(arbol->raiz));
    if(arbol->destructor)
		arbol->destructor(aux->dato);
    aux->dato = dato;
    return true;
}
bool _abb_guardar_rec(abb_t* arbol, nodo_abb_t* actual, const char* clave, void* dato){
    // Primitiva auxiliar recursiva
    int res = arbol->cmp(actual->clave, clave);
    if ((!actual->der && res == -1) || (!actual->izq && res == 1))
        return _abb_guardar(actual, clave, dato, res);
    if (res == -1) return _abb_guardar_rec(arbol,actual->der,clave,dato);
    return _abb_guardar_rec(arbol,actual->izq,clave,dato);

}
/*bool _abb_guardar_rec(abb_t*arbol, nodo_abb_t* actual, const char* clave, void* dato){
    // Primitiva auxiliar recursiva
	if(!actual)
		return false;
    int res = arbol->cmp(actual->clave, clave);
	switch (res){
        case -1: if (actual->der)
					_abb_guardar_rec(arbol, actual->der, clave, dato);
                break;
        case 1: if (actual->izq)
					_abb_guardar_rec(arbol, actual->izq, clave, dato);
                break;
        default: return _abb_reemplazar_nodo(arbol, clave, dato);
    }
	return _abb_guardar(actual, clave, dato, res);
}
*/

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!arbol) return false;
    nodo_abb_t* raiz = arbol->raiz;
    if (!raiz) { //arbol vacio
        raiz = nodo_abb_crear(clave,dato);
        if (!raiz) return false;
        arbol->raiz = raiz;
        arbol->cantidad++;
        return true;
    }

    if(!_abb_guardar_rec(arbol, raiz, clave, dato)) return false;
	/*if (!_abb_guardar(raiz, clave, dato, res)) return false;
    arbol->cantidad++;*/
    arbol->cantidad++;
    return true;
}

// --------- Primitivas auxiliares a abb_borrar ----------- //

int _cant_hijos(nodo_abb_t* nodo){
    if(nodo->der && nodo->izq)
		return 2;
	else{
		if(!nodo->der && !nodo->izq)
			return 0;
		else
			return 1;
	}
 }

void* _borrar_hoja(nodo_abb_t* nodo){
    void* dato = nodo->dato;
    free(nodo);
    return dato;
}

nodo_abb_t* _buscar_max_izq(nodo_abb_t* nodo){
	if(!nodo->der)
		return nodo;
	return _buscar_max_izq(nodo->der);
}

void *_abb_borrar(nodo_abb_t* nodo, nodo_abb_t* padre){
	int cant_hijos = _cant_hijos(nodo);
    void* dato = NULL;
    nodo_abb_t* nodo_aux;
    switch(cant_hijos){
		case 0:
                if (padre->der == nodo) padre->der = NULL;
                else padre->izq = NULL;
                dato = _borrar_hoja(nodo);
                //Borrar sin hijos.
				break;
		case 1: nodo_aux = nodo;//Borrar con 1 hijo.
				if (nodo->der) {
                    nodo = nodo->der;
                    padre->der = NULL;
                } else {
                    nodo = nodo->izq;
                    padre->izq = NULL;
                    }
				dato = _borrar_hoja(nodo_aux);
				break;
		default: //Borrar con 2 hijos.
				 nodo_aux = _buscar_max_izq(nodo);
				 const char* clave_aux = nodo_aux->clave;
				 void* dato_aux = _abb_borrar(nodo_aux, nodo);
                 dato = nodo->dato;
				 nodo->clave = clave_aux;
				 nodo->dato = dato_aux;}
	return dato;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    nodo_abb_t* raiz = arbol->raiz;
    nodo_abb_t* nodo = buscar_nodo(arbol->raiz, arbol->cmp, clave, &raiz);
    if(!nodo) return NULL;
    void *dato = _abb_borrar(nodo, raiz);
    arbol->cantidad --;
    return dato;

    /*bool der = false;
    if (cant_hijos == 0) {
        //tengo problemas con esto!!!! tengo q asignar a null??
        if (padre->der == nodo) der = true;
        dato = nodo->dato;
        free(nodo);
        arbol->cantidad--;
        if (der) padre->der = NULL;
        else padre->izq = NULL;
        return dato;
    }
    else if (cant_hijos == 2){
        // falta caso 2 hijos
        return NULL;
    }
    else {
        nodo_abb* aux;
        if (cant_hijos == -1) aux =     nodo->izq;
        else aux = nodo->der;
        if (padre->der == nodo) padre->der = aux;
        else if (padre->izq == nodo) padre->izq = aux;
        return _borrar_hoja(arbol,nodo);
    }
    return NULL;*/
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_abb_t* raiz = arbol->raiz;
    nodo_abb_t* nodo = buscar_nodo(arbol->raiz, arbol->cmp, clave, &raiz);
    if (!nodo) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_abb_t* raiz = arbol->raiz;
    if (!buscar_nodo(arbol->raiz, arbol->cmp, clave, &raiz))
        return false;
    return true;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(nodo_abb_t* nodo, abb_destruir_dato_t destructor){
    // Primitiva recursiva auxiliar
    if (!nodo) return;
    _abb_destruir(nodo->izq, destructor);
    _abb_destruir(nodo->der, destructor);
    if (destructor) destructor(nodo->dato);
    free(nodo);
}
void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->destructor);
    free(arbol);
}
