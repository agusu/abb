#include "abb.h"
#include <stdio.h>

struct nodo_abb{
	const char* clave;
	void* dato;
	struct nodo_abb* izq;
	struct nodo_abb* der;
};

struct abb{
	struct nodo_abb* raiz;
	abb_destruir_dato_t destructor;
	size_t cantidad;
	abb_comparar_clave_t cmp;
};

nodo_abb* nodo_abb_crear(const char* clave, void* dato){
    nodo_abb* nodo = malloc(sizeof(nodo_abb));
    if (!nodo) return NULL;
    nodo->dato = dato;
    nodo->clave = clave;
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
}

nodo_abb* buscar_nodo(nodo_abb* nodo, abb_comparar_clave_t cmp, const char* clave, nodo_abb** padre){
    if (!nodo) return NULL;
    int resultado = cmp(nodo->clave,clave);
    if (resultado == 0) return nodo;
    *padre = nodo;
    if (resultado > 0)
        return buscar_nodo(nodo->izq, cmp, clave, &nodo);
    return buscar_nodo(nodo->der,cmp,clave,&nodo);
}

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
bool _abb_guardar(nodo_abb* padre,const char* clave, void* dato, int lado){
    // Crea nodo y lo guarda.
    nodo_abb* hoja = nodo_abb_crear(clave,dato);
    if (!hoja) return false;
    if (lado == -1) padre->der = hoja;
    else padre->izq = hoja;
    return true;
}

bool _abb_guardar_aux(nodo_abb* actual, const char* clave, void* dato, abb_comparar_clave_t cmp){
    // Primitiva auxiliar recursiva
    int res = cmp(actual->clave, clave);
    if ((!actual->der && res == -1) || (!actual->izq && res == 1))
        return _abb_guardar(actual, clave, dato, res);
    return _abb_guardar_aux(actual,clave,dato,cmp);
}

bool _abb_reemplazar_nodo(abb_t* arbol, const char* clave, void* dato){
    // Caso clave preexistente
    nodo_abb* raiz = arbol->raiz;
    nodo_abb* aux = buscar_nodo(arbol->raiz,arbol->cmp,clave, &raiz);
    arbol->destructor(aux->dato);
    aux->dato = dato;
    return true;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!arbol) return false;
    nodo_abb* raiz = arbol->raiz;
    if (abb_pertenece(arbol,clave)){
        return _abb_reemplazar_nodo(arbol,clave,dato);
    }
    if (!raiz) { //arbol vacio
        raiz = nodo_abb_crear(clave,dato);
        if (!raiz) return false;
        arbol->raiz = raiz;
        arbol->cantidad++;
        return true;
    }
    abb_comparar_clave_t cmp = arbol->cmp;
    int res = cmp(raiz->clave,clave);
    bool ok = 0;
    switch (res){
        case -1: if (raiz->der) ok = _abb_guardar_aux(raiz->der, clave, dato, cmp);
                break;
        case 1: if (raiz->izq) ok = _abb_guardar_aux(raiz->izq, clave, dato, cmp);
                break;
    }
    if ((res == -1 || res == 1) && ok) {arbol->cantidad++; return true;}
    if (!_abb_guardar(raiz, clave, dato, res)) return false;
    arbol->cantidad++;
    return true;
}
// --------- Primitivas auxiliares a abb_borrar ----------- //
int _cant_hijos(nodo_abb* nodo){
    int hijos = 0;
    bool der, izq;
    if (nodo->der) {hijos++; der = true;}
    if (nodo->izq) {hijos++; izq = true;}
    if (hijos == 1){
        if (der) return 1;
        if (izq) return -1;
    }
    return hijos;
}

void* _borrar_hoja(abb_t* arbol, nodo_abb* nodo){
    void* dato = nodo->dato;
    free(nodo);
    arbol->cantidad--;
    return dato;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!abb_pertenece(arbol,clave)) return NULL;
    nodo_abb* padre = arbol->raiz;
    nodo_abb* nodo = buscar_nodo(arbol->raiz, arbol->cmp, clave, &padre);
    int cant_hijos = _cant_hijos(nodo);
    void* dato;
    bool der = false;
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
    return NULL;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    nodo_abb* raiz = arbol->raiz;
    if (!raiz) return NULL;
    nodo_abb* nodo = buscar_nodo(arbol->raiz,arbol->cmp,clave,&raiz);
    if (!nodo) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_abb* padre = arbol->raiz;
    if (!buscar_nodo(arbol->raiz, arbol->cmp, clave, &padre))
        return false;
    return true;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(nodo_abb* nodo, abb_destruir_dato_t destructor){
    // Primitiva recursiva auxiliar
    if (!nodo) return;
    _abb_destruir(nodo->der,destructor);
    nodo->der = NULL;
    _abb_destruir(nodo->izq,destructor);
    nodo->izq = NULL;
    if (destructor){destructor(nodo->dato);}
    free(nodo);
}
void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->destructor);
    free(arbol);
}
