#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pila.h"
#include "abb.h"


int cmp (const char* clave1, const char* clave2){
    if (atoi(clave1) < atoi(clave2))
        return -1;
    if (atoi(clave1) > atoi(clave2))
        return 1;
    return 0;
}

void pruebas_casos_borrar(void){
    abb_t* arbol = abb_crear(cmp, NULL);
    char *uno = "1";
    char *dos = "2";
    char *tres = "3";
    char *cuatro = "4";
    char *cinco = "5";
    char *seis = "6";

    abb_guardar(arbol, cuatro, cuatro);
    abb_guardar(arbol, dos, dos);
    abb_guardar(arbol, tres, tres);
    abb_guardar(arbol, uno, uno);
    abb_guardar(arbol, cinco, cinco);
    abb_guardar(arbol, seis, seis);

    print_test("Verifico que se hayan guardado 6 elementos", abb_cantidad(arbol) == 6);
    bool ok = abb_pertenece(arbol, uno) && abb_pertenece(arbol, cinco) &&
              abb_pertenece(arbol, cuatro) && abb_pertenece(arbol, dos) &&
              abb_pertenece(arbol, tres);
    print_test("Verifico que los elementos guardados pertenezcan al arbol", ok);
    //borro un nodo con un solo hijo.
    print_test("Borrar un nodo que tiene un hijo", strcmp((char*)abb_borrar(arbol, cinco), cinco) == 0);
    print_test("Borrar padre con un hijo",!abb_pertenece(arbol, cinco));
    print_test("Hijo del nodo borrado pertenece", abb_pertenece(arbol, seis));
    print_test("Cantidad luego de borrar un nodo (5)", abb_cantidad(arbol) == 5);
    //borro un nodo con dos hijos.
    print_test("Borrar un nodo que tiene dos hijos", strcmp((char*)abb_borrar(arbol, dos), dos) == 0);
    print_test("Borrar padre con dos hijos", !abb_pertenece(arbol, dos));
    print_test("Cantidad luego de borrar otro nodo (4)", abb_cantidad(arbol) == 4);
    print_test("Hijo derecho(3) pertenece", abb_pertenece(arbol, tres));
    print_test("Hijo izquierdo(1) pertenece", abb_pertenece(arbol, uno));
    //borro raiz con 2 hijos.
    print_test("Borrar la raiz con 2 hijos", strcmp((char*)abb_borrar(arbol, cuatro), cuatro) == 0);
    print_test("Veo si la raiz esta en el arbol", !abb_pertenece(arbol, cuatro));
    print_test("Cantidad luego de borrar la raiz (3)", abb_cantidad(arbol) == 3);
    print_test("Elementos restantes pertenecen", abb_pertenece(arbol, seis) &&
                              abb_pertenece(arbol, uno) && abb_pertenece(arbol, tres));
    //borrar una hoja.
    print_test("Borrar una hoja", strcmp((char*)abb_borrar(arbol, uno), uno) == 0);
    print_test("Veo si la hoja esta en el arbol",!abb_pertenece(arbol, uno));
    print_test("Cantidad luego de borrar la hoja (2)", abb_cantidad(arbol) == 2);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, uno));
    abb_destruir(arbol);

}


void pruebas_iterador_vacio(){
	abb_t *abb = abb_crear(cmp, NULL);
	abb_iter_t *iterador = abb_iter_in_crear(abb);

  print_test("Iterador de arbol vacio al final",abb_iter_in_al_final(iterador)==true);
	print_test("Avanzar iterador estando al final", abb_iter_in_avanzar(iterador) == false);
	print_test("Actual es NULL",abb_iter_in_ver_actual(iterador)==NULL);
	abb_destruir(abb);
	abb_iter_in_destruir(iterador);
}


void pruebas_iterador(){
  char *claves[] = {"perro", "gato", "vaca"};
  char *valores[] = {"guau", "miau", "mu"};
  abb_t *arbol = abb_crear(strcmp, NULL);
  abb_guardar(arbol,claves[0],valores[0]);
  abb_guardar(arbol,claves[1],valores[1]);
  abb_guardar(arbol,claves[2],valores[2]);

  char *claves_en_orden[] = {"gato", "perro", "vaca"};
	bool ok = true;
	abb_iter_t *iterador = abb_iter_in_crear(arbol);

  for (int i = 0; i < 3; i++){
        ok &= (abb_iter_in_ver_actual(iterador) == claves_en_orden[i]);
        abb_iter_in_avanzar(iterador);
	}
	print_test("Avanzar y ver actual iterador", ok);
	print_test("Iterador al final", abb_iter_in_al_final(iterador));
	print_test("Iterador no avanza mas", !abb_iter_in_avanzar(iterador));
	print_test("Actual es NULL",!abb_iter_in_ver_actual(iterador));
	abb_destruir(arbol);
	abb_iter_in_destruir(iterador);
}


void pruebas_abb_alumno(void){
    pruebas_casos_borrar();
    pruebas_iterador_vacio();
    pruebas_iterador();
}
