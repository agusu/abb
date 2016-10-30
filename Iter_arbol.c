typedef struct abb_iter{
	const abb_t arbol;
	pila_t pila;
}abb_iter_t;

void abb_iterar_izq(pila_t *pila, abb_nodo_t *nodo_abb){
	if(!nodo_abb)
		return;
	pila = pila_apilar(pila, nodo_abb);
	abb_iterar_izq(pila, nodo_abb->izq);
}


abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t *iter = malloc(sizeof(abb_iter_t));
	iter->pila = pila_crear();
	if(!abb_iter_t || !iter->pila)
		return NULL;
	iter->arbol = arbol;
	abb_iterar_izq(iter->pila, iter->arbol->raiz);
	return iter;
}


bool abb_iter_in_avanzar(abb_iter_t *iter){
	if(abb_iter_in_al_final(iter))
		return false;
	abb_iterar_izq(iter->pila, pila_desapilar(iter->pila)->der);
	return true;
}


const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(abb_iter_in_al_final(iter))
		return NULL;
	return pila_ver_tope(pila)->clave;
}


bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(pila);
}


void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	pila_t pila = pila_crear();
	if(!pila)
		return;
	abb_iterar_izq(pila, arbol->raiz);
	nodo_abb *actual = pila_desapilar(pila);
	while(actual && visitar(actual->clave, actual->dato, extra)){
		abb_iterar_izq(pila, actual->der);
		actual = pila_desapilar(pila);
	}
}

}
