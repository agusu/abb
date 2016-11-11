#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pila.h"
#include "abb.h"
#include "lista.h"
#include "cola.h"
#define CANT_VECTOR_AUX 5
#define CANT_CLAVES 5
#define CANT_CLAVES_MUCHAS 10000

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

int cmp (const char* clave1, const char* clave2){
    if (atoi(clave1) < atoi(clave2))
        return -1;
    if (atoi(clave1) > atoi(clave2))
        return 1;
    return 0;
}

/*Recibe un arbol, un vector de claves y 3 posiciones.
 *Guarda las claves en el arbol de forma que quede moderadamente balanceado.*/
void cargar_datos_rec(abb_t *arbol, char *claves[], size_t actual, size_t mayor, size_t menor){
  abb_guardar(arbol, claves[actual], claves[actual]);
  if(mayor - actual > 1)
    cargar_datos_rec(arbol, claves, actual + (mayor-actual)/2, mayor , actual);
  if(actual - menor > 1)
    cargar_datos_rec(arbol, claves, actual - (actual-menor)/2, actual, menor);
  return;
}

void cargar_datos(abb_t *arbol, char *claves[], size_t actual, size_t mayor, size_t menor){
  cargar_datos_rec(arbol, claves, actual, mayor, menor);
  abb_guardar(arbol, claves[mayor], claves[mayor]);
  abb_guardar(arbol, claves[menor], claves[menor]);
}

/*Calcula la n potencia de un numero entero*/
int pwd(int base, size_t n){
  int inicio = base;
  for(size_t i = 1; i < n; i++){
    base *= inicio;
  }
  return base;
}

/*Recibe un vector de claves y una cantidad.
 *Crea tantas claves como indique la cantidad pasada por parametro*/
void crear_claves(char **claves, size_t cantidad){
  for(int i = 0; i < cantidad; i++){
    claves[i] = malloc(10);
    sprintf(claves[i], "%d", i);
  }
}
/*Recibe un vector de claves y una cantidad.
 *Libera tantas claves como indique la cantidad pasada por parametro*/
void liberar_claves(char **claves, size_t cantidad){
  for(int i = 0; i < cantidad; i++){
    free(claves[i]);
  }
}

void llenar_pila(pila_t* pila, int* vector){
  for(int i = 0; i < CANT_VECTOR_AUX; i++){
    vector[i] = i + 1;
    pila_apilar(pila, &vector[i]);
  }
}

void llenar_cola(cola_t* cola, int* vector){
  for(int i = 0; i < CANT_VECTOR_AUX; i++){
    vector[i] = i + 1;
    cola_encolar(cola, &vector[i]);
  }
}

void llenar_lista(lista_t* lista, int* vector){
  for(int i = 0; i < CANT_VECTOR_AUX; i++){
    vector[i] = i + 1;
    lista_insertar_primero(lista, &vector[i]);
  }
}
/********************************Destructores********************************/

void wrapper_pila_destruir(void* pila){
  pila_destruir((pila_t*)pila);
}

void wrapper_cola_destruir(void* cola){
  cola_destruir((cola_t*)cola, NULL);
}

void wrapper_lista_destruir(void* lista){
  lista_destruir((lista_t*)lista, NULL);
}

/***********************Funciones Iterador Interno***************************/

bool calcular_promedio(const char *clave, void *dato, void *extra){
  int *promedio = extra;
  char *numero = dato;
  *promedio += atoi(numero);
  return true;
}

bool imprimir_n_primeros(const char *clave, void *dato, void *extra){
  int *pos = extra;
  if(*pos == 0) return false;
  (*pos)--;
  printf("La clave es: %s y el dato es: %s\n", clave, (char*)dato);
  return true;
}

/* ******************************************************************
 *                         PRUEBAS DEL ABB
 * *****************************************************************/

void pruebas_abb_vacio(void){
  abb_t* arbol = abb_crear(cmp, NULL);
  char *clave = "Prueba";
  int valor = 10;

  printf("\n~~~~~ PRUEBAS ABB VACIO ~~~~~\n");
  print_test("Verifico si la cantidad es 0", abb_cantidad(arbol) == 0);
  print_test("Verifico si clave pertenece al arbol vacio", !abb_pertenece(arbol, clave));
  print_test("Verifico si puedo obtener de un arbol vacio", !abb_obtener(arbol, clave));
  print_test("Verifico si puedo borrar en un arbol vacio", !abb_borrar(arbol, clave));
  print_test("Agrego un elemento al arbol vacio", abb_guardar(arbol, clave, &valor));
  print_test("Verifico si la cantidad es 1", abb_cantidad(arbol) == 1);
  print_test("Veo si la clave ingresada anteriormente pertenece al arbol", abb_pertenece(arbol, clave));
  print_test("Obtengo la clave ingresada anteriormente", abb_obtener(arbol, clave));
  print_test("Borro la clave ingresada anteriormente", abb_borrar(arbol, clave));
  print_test("Verifico si la cantidad es 0", abb_cantidad(arbol) == 0);
  print_test("Verifico si puedo borrar una clave borrada", !abb_borrar(arbol, clave));
  abb_destruir(arbol);
  print_test("Destruyo el arbol vacio", true);

}

void pruebas_iterador_abb_vacio(){
	abb_t *abb = abb_crear(cmp, NULL);
	abb_iter_t *iterador = abb_iter_in_crear(abb);

  printf("\n~~~~~ PRUEBAS ITERADOR ABB VACIO ~~~~~\n");
  print_test("Iterador de arbol vacio al final", abb_iter_in_al_final(iterador));
	print_test("Avanzar iterador estando al final", !abb_iter_in_avanzar(iterador));
	print_test("Actual es NULL", !abb_iter_in_ver_actual(iterador));
	abb_destruir(abb);
	abb_iter_in_destruir(iterador);
}

void pruebas_casos_borrar(void){
    abb_t* arbol = abb_crear(cmp, NULL);
    char *uno = "1";
    char *dos = "2";
    char *tres = "3";
    char *cuatro = "4";
    char *cinco = "5";
    char *seis = "6";

    printf("\n~~~~~ PRUEBAS ABB CASOS BORRAR~~~~~\n");
    print_test("Guardo un elemento en el arbol (4)", abb_guardar(arbol, cuatro, cuatro));
    print_test("Guardo un elemento en el arbol (2)", abb_guardar(arbol, dos, dos));
    print_test("Guardo un elemento en el arbol (3)", abb_guardar(arbol, tres, tres));
    print_test("Guardo un elemento en el arbol (1)", abb_guardar(arbol, uno, uno));
    print_test("Guardo un elemento en el arbol (5)", abb_guardar(arbol, cinco, cinco));
    print_test("Guardo un elemento en el arbol (6)", abb_guardar(arbol, seis, seis));


    print_test("Verifico que se hayan guardado 6 elementos", abb_cantidad(arbol) == 6);
    bool ok = abb_pertenece(arbol, uno) && abb_pertenece(arbol, cinco) &&
              abb_pertenece(arbol, cuatro) && abb_pertenece(arbol, dos) &&
              abb_pertenece(arbol, tres);
    print_test("Verifico que los elementos guardados pertenezcan al arbol", ok);
    //borro un nodo con un solo hijo.
    print_test("Borrar un nodo que tiene un hijo", strcmp((char*)abb_borrar(arbol, cinco), cinco) == 0);
    print_test("Veo si padre con un hijo pertenece al arbol",!abb_pertenece(arbol, cinco));
    print_test("Hijo del nodo borrado pertenece", abb_pertenece(arbol, seis));
    print_test("Cantidad luego de borrar un nodo (5)", abb_cantidad(arbol) == 5);
    //borro un nodo con dos hijos.
    print_test("Borrar un nodo que tiene dos hijos", strcmp((char*)abb_borrar(arbol, dos), dos) == 0);
    print_test("Veo si el padre con dos hijos esta en el arbol", !abb_pertenece(arbol, dos));
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
    //borrar raiz con un solo hijo.
    print_test("Borrar la raiz que tiene un hijo ", strcmp((char*)abb_borrar(arbol, tres), tres) == 0);
    print_test("Veo si la raiz con un hijo esta en el arbol",!abb_pertenece(arbol, tres));
    print_test("Hijo del nodo borrado pertenece", abb_pertenece(arbol, seis));
    print_test("Cantidad luego de borrar un nodo (1)", abb_cantidad(arbol) == 1);
    //borrar raiz sin hijos.
    print_test("Borrar la raiz que no tiene hijos", strcmp((char*)abb_borrar(arbol, seis), seis) == 0);
    print_test("Veo si la raiz esta en el arbol",!abb_pertenece(arbol, seis));
    print_test("Cantidad luego de borrar la hoja (0)", abb_cantidad(arbol) == 0);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, seis));

    abb_destruir(arbol);

}

void pruebas_abb_destructor(){
  lista_t *listas[CANT_CLAVES];
  pila_t *pilas[CANT_CLAVES];
  cola_t *colas[CANT_CLAVES];
  char *claves[CANT_CLAVES];
  int vector[CANT_VECTOR_AUX];
  crear_claves(claves, CANT_CLAVES);

  printf("\n~~~~~ PRUEBAS ABB DESTRUCTOR ~~~~~\n");
  abb_t *arbol = abb_crear(strcmp, wrapper_pila_destruir);
  int i = CANT_CLAVES/2;
  for (int j = 0; j < CANT_CLAVES; j++){
    i += j * pwd(-1, j);
    pilas[j] = pila_crear();
    llenar_pila(pilas[j], vector);
    print_test("Insertar al arbol una pila de números enteros",
                                            abb_guardar(arbol, claves[i], pilas[j]));
  }
  abb_destruir(arbol);
  print_test("Arbol con pilas adentro destruido", true);
  i = CANT_CLAVES/2;
  arbol = abb_crear(strcmp, wrapper_cola_destruir);
  for (int j = 0; j < CANT_CLAVES; j++){
    i += j * pwd(-1, j);
    colas[j] = cola_crear();
    llenar_cola(colas[j], vector);
    print_test("Insertar al arbol una cola de números enteros",
                                            abb_guardar(arbol, claves[i], colas[j]));
  }
  abb_destruir(arbol);
  print_test("Arbol con colas adentro destruido", true);

  arbol = abb_crear(strcmp, wrapper_lista_destruir);
  i = CANT_CLAVES/2;
  for (int j = 0; j < CANT_CLAVES; j++){
    i += j * pwd(-1, j);
    listas[i] = lista_crear();
    llenar_lista(listas[i], vector);
    print_test("Insertar al arbol una lista de números enteros",
                                            abb_guardar(arbol, claves[i], listas[i]));
  }
  abb_destruir(arbol);
  print_test("Arbol con listas adentro destruido", true);
  liberar_claves(claves, CANT_CLAVES);
}

void pruebas_abb_reemplazar(){
  char *claves[] = {"perro", "gato", "vaca"};
  char *valores[] = {"muuu", "guau", "miau"};
  abb_t *arbol = abb_crear(strcmp, NULL);

  printf("\n~~~~~ PRUEBAS ABB REEMPLAZAR ~~~~~\n");
  //agrego elementos al arbol.
  print_test("Agrego un elemento al arbol",abb_guardar(arbol, claves[0], valores[0]));
  print_test("Agrego un elemento al arbol", abb_guardar(arbol, claves[1], valores[1]));
  print_test("Agrego un elemento al arbol", abb_guardar(arbol, claves[2], valores[2]));
  //verifico que los datos esten bien cargados.
  print_test("Verifico que se haya ingresado bien el dato",
                                  (char*)abb_obtener(arbol, claves[0]) == valores[0]);
  print_test("Verifico que se haya ingresado bien el dato",
                                  (char*)abb_obtener(arbol, claves[1]) == valores[1]);
  print_test("Verifico que se haya ingresado bien el dato",
                                  (char*)abb_obtener(arbol, claves[2]) == valores[2]);

  //cambio los valores de las claves del arbol.
  print_test("Cambio un elemento del arbol",abb_guardar(arbol, claves[0], valores[1]));
  print_test("Cambio un elemento del arbol", abb_guardar(arbol, claves[1], valores[2]));
  print_test("Cambio un elemento del arbol", abb_guardar(arbol, claves[2], valores[0]));
  //verifico que se hayan cambiado los valores.
  print_test("Verifico que se haya cambiado el dato",
                                  (char*)abb_obtener(arbol, claves[0]) == valores[1]);
  print_test("Verifico que se haya cambiado el dato",
                                  (char*)abb_obtener(arbol, claves[1]) == valores[2]);
  print_test("Verifico que se haya cambiado el dato",
                                  (char*)abb_obtener(arbol, claves[2]) == valores[0]);

  abb_destruir(arbol);
  print_test("Arbol destruido correctamente", true);
}

void pruebas_abb_muchos_elementos(void){
  abb_t *arbol = abb_crear(cmp, NULL);
  char *claves[CANT_CLAVES_MUCHAS];
	bool ok = true;
  crear_claves(claves, CANT_CLAVES_MUCHAS);

  printf("\n~~~~~ PRUEBAS ABB MUCHOS ELEMENTOS ~~~~~\n");
  //Agrego muchos elementos al arbol.
  cargar_datos(arbol, claves, CANT_CLAVES_MUCHAS/2, CANT_CLAVES_MUCHAS - 1, 0);
  //me fijo si pertenencen tanto las claves como los datos.
  for (size_t j = 0; j < CANT_CLAVES_MUCHAS; j++){
    if (!abb_pertenece(arbol, claves[j]) || (char*)abb_obtener(arbol, claves[j]) != claves[j])
      ok = false;
  }
  print_test("Guardar muchos elementos en el arbol", ok);
  print_test("Verifico que todos los elementos pertenezcan", ok);
  print_test("Verifico que la cantidad sea la correcta", abb_cantidad(arbol) == CANT_CLAVES_MUCHAS);
  //Borro todas las claves en el arbol.
  for (size_t j = 0; j <  CANT_CLAVES_MUCHAS; j++){
    if (strcmp((char*)abb_borrar(arbol, claves[j]), claves[j]) != 0) ok = false;
  }
  print_test("Borrar todos los elementos del arbol", ok);
  print_test("Verifico que la cantidad sea la correcta ", abb_cantidad(arbol) == 0);

  abb_destruir(arbol);//destruyo el arbol
  print_test("Destruyo el arbol exitosamente", true);


  arbol = abb_crear(cmp, NULL);//creo otro arbol.
  printf("\n~~~~~ PRUEBAS ITERADOR ABB MUCHOS ELEMENTOS ~~~~~\n");
  //Agrego muchos elementos al arbol.
  cargar_datos(arbol, claves, CANT_CLAVES_MUCHAS/2, CANT_CLAVES_MUCHAS - 1, 0);
  abb_iter_t *iterador = abb_iter_in_crear(arbol);//creo el iterador
  ok = true;
  //Controlo que el iterador, recorra todos los elementos.
  for (int j = 0; j < CANT_CLAVES_MUCHAS; j++){
    if(strcmp(abb_iter_in_ver_actual(iterador), claves[j]) != 0) ok = false;
    abb_iter_in_avanzar(iterador);
  }

  print_test("Avanzar y ver actual iterador para todos las claves", ok);
	print_test("Iterador al final", abb_iter_in_al_final(iterador));
	print_test("Iterador no avanza mas", !abb_iter_in_avanzar(iterador));
	print_test("Actual es NULL", !abb_iter_in_ver_actual(iterador));

  abb_iter_in_destruir(iterador);

  abb_destruir(arbol);

  liberar_claves(claves, CANT_CLAVES_MUCHAS);
}

void pruebas_iterador_interno(void){
  abb_t *arbol = abb_crear(cmp, NULL);
  char *claves[CANT_CLAVES];
	bool ok = true;
  crear_claves(claves, CANT_CLAVES);
  int promedio = 0, cant = 3;

  printf("\n~~~~~ PRUEBAS ITERADOR INTERNO ~~~~~\n");
  cargar_datos(arbol, claves, CANT_CLAVES/2, CANT_CLAVES - 1, 0);
  //prueba que recorre todo el arbol.
  abb_in_order(arbol, calcular_promedio, &promedio);
  print_test("Calcular el promedio del arbol", promedio / CANT_CLAVES == 2 );
  //prueba en la que el iterador sale por false.
  abb_in_order(arbol, imprimir_n_primeros, &cant);
  print_test("Imprimir los primeros n elementos del arbol", ok);

  abb_destruir(arbol);
  liberar_claves(claves, CANT_CLAVES);
}

void pruebas_abb_alumno(void){
  pruebas_abb_vacio();
  pruebas_iterador_abb_vacio();
  pruebas_casos_borrar();
  pruebas_abb_destructor();
  pruebas_abb_reemplazar();
  pruebas_abb_muchos_elementos();
  pruebas_iterador_interno();
}
