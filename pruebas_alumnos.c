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
#define CANT_CLAVES_MUCHAS 5000

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

int cmp (const char* clave1, const char* clave2){
    if (atoi(clave1) < atoi(clave2))
        return -3;
    if (atoi(clave1) > atoi(clave2))
        return 2;
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
    char *siete = "7";
    char *ocho = "8";
    char *nueve = "9";
    char *diez = "10";
    char *once = "11";


    printf("\n~~~~~ PRUEBAS ABB CASOS BORRAR~~~~~\n");
    print_test("Guardo un elemento en el arbol (5)", abb_guardar(arbol, cinco, cinco));
    print_test("Guardo un elemento en el arbol (2)", abb_guardar(arbol, dos, dos));
    print_test("Guardo un elemento en el arbol (4)", abb_guardar(arbol, cuatro, cuatro));
    print_test("Guardo un elemento en el arbol (3)", abb_guardar(arbol, tres, tres));
    print_test("Guardo un elemento en el arbol (1)", abb_guardar(arbol, uno, uno));
    print_test("Guardo un elemento en el arbol (9)", abb_guardar(arbol, nueve, nueve));
    print_test("Guardo un elemento en el arbol (7)", abb_guardar(arbol, siete, siete));
    print_test("Guardo un elemento en el arbol (8)", abb_guardar(arbol, ocho, ocho));
    print_test("Guardo un elemento en el arbol (6)", abb_guardar(arbol, seis, seis));
    print_test("Guardo un elemento en el arbol (11)", abb_guardar(arbol, once, once));
    print_test("Guardo un elemento en el arbol (10)", abb_guardar(arbol, diez, diez));

    print_test("Verifico que se hayan guardado 11 elementos", abb_cantidad(arbol) == 11);
    bool ok = abb_pertenece(arbol, uno) && abb_pertenece(arbol, dos) &&
              abb_pertenece(arbol, tres) && abb_pertenece(arbol, cuatro) &&
              abb_pertenece(arbol, cinco) && abb_pertenece(arbol, seis) &&
              abb_pertenece(arbol, siete) && abb_pertenece(arbol, ocho) &&
              abb_pertenece(arbol, nueve) && abb_pertenece(arbol, diez) &&
              abb_pertenece(arbol, once);
    print_test("Verifico que los elementos guardados pertenezcan al arbol", ok);

    //borro nodo con 2 hijos. Uno de los hijos tiene un hijo.
    print_test("Borrar nodo con 2 hijos (dos)",
                                strcmp((char*)abb_borrar(arbol, dos), dos) == 0);
    print_test("Veo si el nodo no esta en el arbol", !abb_pertenece(arbol, dos));
    print_test("Cantidad luego de borrar el nodo (9)", abb_cantidad(arbol) == 10);
    print_test("Veo que no se haya modificado el valor de (tres)",
                                strcmp((char*)abb_obtener(arbol, tres), tres) == 0);
    print_test("Veo que no se haya modificado el valor de (uno)",
                                strcmp((char*)abb_obtener(arbol, uno), uno) == 0);
    print_test("Veo que no se haya modificado el valor de (cuatro)",
                                strcmp((char*)abb_obtener(arbol, cuatro), cuatro) == 0);

    print_test("Borrar elemento previamente borrado (falso)", !abb_borrar(arbol, dos));

    //borro raiz con 2 hijos.
    print_test("Borrar la raiz con 2 hijos (cinco)",
                                strcmp((char*)abb_borrar(arbol, cinco), cinco) == 0);
    print_test("Veo si la raiz no esta en el arbol", !abb_pertenece(arbol, cinco));
    print_test("Cantidad luego de borrar la raiz (9)", abb_cantidad(arbol) == 9);
    print_test("Veo que no se haya modificado el valor de (tres)",
                                strcmp((char*)abb_obtener(arbol, tres), tres) == 0);
    print_test("Borrar elemento previamente borrado (falso)", !abb_borrar(arbol, cinco));

    //borro un nodo con dos hijos. Cuyos hijos son hojas.
    print_test("Borrar un nodo que tiene dos hijos (siete)",
                                 strcmp((char*)abb_borrar(arbol, siete), siete) == 0);
    print_test("Veo si el nodo con dos hijos no esta en el arbol", !abb_pertenece(arbol, siete));
    print_test("Cantidad luego de borrar otro nodo (8)", abb_cantidad(arbol) == 8);
    print_test("Hijo derecho (ocho) pertenece", abb_pertenece(arbol, ocho));
    print_test("Hijo izquierdo (seis) pertenece", abb_pertenece(arbol, seis));
    print_test("Veo que no se haya modificado el valor de (ocho)",
                                strcmp((char*)abb_obtener(arbol, ocho), ocho) == 0);
    print_test("Veo que no se haya modificado el valor de (seis)",
                                strcmp((char*)abb_obtener(arbol, seis), seis) == 0);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, siete));
    print_test("Guardo un elemento en el arbol (siete)", abb_guardar(arbol, siete, siete));
    print_test("Borrar un nodo que tiene dos hijos (siete)",
                                 strcmp((char*)abb_borrar(arbol, siete), siete) == 0);

    //borrar una hoja.
    print_test("Borrar una hoja (diez)", strcmp((char*)abb_borrar(arbol, diez), diez) == 0);
    print_test("Veo si la hoja no esta en el arbol",!abb_pertenece(arbol, diez));
    print_test("Cantidad luego de borrar la hoja (7)", abb_cantidad(arbol) == 7);
    print_test("Borrar elemento previamente borrado (falso)", !abb_borrar(arbol, diez));

    //borrar una hoja.
    print_test("Borrar una hoja (ocho)",
                                strcmp((char*)abb_borrar(arbol, ocho), ocho) == 0);
    print_test("Veo si la hoja no esta en el arbol", !abb_pertenece(arbol, ocho));
    print_test("Cantidad luego de borrar un nodo (6)", abb_cantidad(arbol) == 6);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, ocho));
    print_test("Verifico si (seis) mantiene su valor",
                               strcmp((char*)abb_obtener(arbol, seis), seis) == 0);


    //Borro nodo con un hijo derecho.
    print_test("Borrar nodo con un hijo derecho (uno)",
                                strcmp((char*)abb_borrar(arbol, uno), uno) == 0);
    print_test("Veo si el nodo no esta en el arbol",!abb_pertenece(arbol, uno));
    print_test("Cantidad luego de borrar la hoja (5)", abb_cantidad(arbol) == 5);
    print_test("Borrar elemento previamente borrado (falso)", !abb_borrar(arbol, uno));
    print_test("Verifico si (tres) mantiene su valor",
                               strcmp((char*)abb_obtener(arbol, tres), tres) == 0);


    //Borro hoja.
    print_test("Borrar una hoja (once)",
                                strcmp((char*)abb_borrar(arbol, once), once) == 0);
    print_test("Veo si la hoja no esta en el arbol",!abb_pertenece(arbol, once));
    print_test("Cantidad luego de borrar un nodo (4)", abb_cantidad(arbol) == 4);
    print_test("Borrar elemento previamente borrado (falso)", !abb_borrar(arbol, once));

    //borro un nodo con un hijo izq.
    print_test("Borrar un nodo que tiene un hijo izq (nueve)",
                                 strcmp((char*)abb_borrar(arbol, nueve), nueve) == 0);
    print_test("Veo si el nodo no esta en el arbol", !abb_pertenece(arbol, nueve));
    print_test("Cantidad luego de borrar otro nodo (3)", abb_cantidad(arbol) == 3);
    print_test("Hijo izquierdo (seis) pertenece", abb_pertenece(arbol, seis));
    print_test("Veo que no se haya modificado el valor de (seis)",
                                strcmp((char*)abb_obtener(arbol, seis), seis) == 0);

    //Borro una hoja.
    print_test("Borrar una hoja (seis)",
                              strcmp((char*)abb_borrar(arbol, seis), seis) == 0);
    print_test("Veo si la hoja esta en el arbol",!abb_pertenece(arbol, seis));
    print_test("Cantidad luego de borrar la hoja (2)", abb_cantidad(arbol) == 2);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, seis));


    //borro raiz con un hijo.
    print_test("Guardo un elemento en el arbol (4)", abb_guardar(arbol, cuatro, NULL));
    print_test("Borrar un nodo que tiene un hijo (cuatro)",
                                (char*)abb_borrar(arbol, cuatro) == NULL );
    print_test("Veo si el nodo con un hijo no pertenece al arbol",!abb_pertenece(arbol, cuatro));
    print_test("Hijo del nodo borrado pertenece", abb_pertenece(arbol, tres));
    print_test("Cantidad luego de borrar un nodo (1)", abb_cantidad(arbol) == 1);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, seis));

    //borrar raiz sin hijos.
    print_test("Borrar la raiz sin hijos (tres)",
                                strcmp((char*)abb_borrar(arbol, tres), tres) == 0);
    print_test("Veo si la raiz no esta en el arbol",!abb_pertenece(arbol, tres));
    print_test("Cantidad luego de borrar un nodo (0)", abb_cantidad(arbol) == 0);
    print_test("Borrar elemento previamente borrado", !abb_borrar(arbol, tres));

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

  print_test("Veo que la cantidad sea la correcta(3)", abb_cantidad(arbol) == 3);
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

  print_test("Veo que la cantidad sea la correcta(3)", abb_cantidad(arbol) == 3);
  abb_destruir(arbol);
  print_test("Arbol destruido correctamente", true);

  printf("\n~~~~~ PRUEBAS ABB REEMPLAZAR DESTRUIR ~~~~~\n");

  int *valor1 = malloc(sizeof(int)), *valor2 = malloc(sizeof(int));
  int *valor3 = malloc(sizeof(int)), *valor4 = malloc(sizeof(int));
  int *valor5 = malloc(sizeof(int)), *valor6 = malloc(sizeof(int));
  //coloco los valores.
  *valor1 = 1;
  *valor2 = 2;
  *valor3 = 3;
  *valor4 = 4;
  *valor5 = 5;
  *valor6 = 6;

  arbol = abb_crear(strcmp, free);
  //agrego elementos al arbol.
  print_test("Agrego un elemento al arbol", abb_guardar(arbol, claves[0], valor1));
  print_test("Agrego un elemento al arbol", abb_guardar(arbol, claves[1], valor2));
  print_test("Agrego un elemento al arbol", abb_guardar(arbol, claves[2], valor3));
  print_test("Veo que la cantidad sea la correcta(3)", abb_cantidad(arbol) == 3);
  //verifico que los datos esten bien cargados.
  print_test("Verifico que se haya ingresado bien el dato",
                                  *(int*)abb_obtener(arbol, claves[0]) == *valor1);
  print_test("Verifico que se haya ingresado bien el dato",
                                  *(int*)abb_obtener(arbol, claves[1]) == *valor2);
  print_test("Verifico que se haya ingresado bien el dato",
                                  *(int*)abb_obtener(arbol, claves[2]) == *valor3);

  print_test("Veo que la cantidad sea la correcta(3)", abb_cantidad(arbol) == 3);
  //cambio los valores de las claves del arbol.
  print_test("Cambio un elemento del arbol",abb_guardar(arbol, claves[0], valor4));
  print_test("Cambio un elemento del arbol", abb_guardar(arbol, claves[1], valor5));
  print_test("Cambio un elemento del arbol", abb_guardar(arbol, claves[2], valor6));
  //verifico que se hayan cambiado los valores.
  print_test("Verifico que se haya ingresado bien el dato",
                                  *(int*)abb_obtener(arbol, claves[0]) == *valor4);
  print_test("Verifico que se haya ingresado bien el dato",
                                  *(int*)abb_obtener(arbol, claves[1]) == *valor5);
  print_test("Verifico que se haya ingresado bien el dato",
                                  *(int*)abb_obtener(arbol, claves[2]) == *valor6);

  print_test("Veo que la cantidad sea la correcta(3)", abb_cantidad(arbol) == 3);

  abb_destruir(arbol);
  print_test("Arbol destruido correctamente", true);

}

void pruebas_abb_muchos_elementos(size_t cantidad){
  abb_t *arbol = abb_crear(cmp, NULL);
  char *claves[cantidad];
	bool ok = true;
  crear_claves(claves, cantidad);

  printf("\n~~~~~ PRUEBAS ABB MUCHOS ELEMENTOS ~~~~~\n");
  //Agrego muchos elementos al arbol.
  cargar_datos(arbol, claves, cantidad/2, cantidad - 1, 0);
  //me fijo si pertenencen tanto las claves como los datos.
  for (size_t j = 0; j < cantidad; j++){
    if (!abb_pertenece(arbol, claves[j]) || (char*)abb_obtener(arbol, claves[j]) != claves[j])
      ok = false;
  }
  print_test("Guardar muchos elementos en el arbol", ok);
  print_test("Verifico que todos los elementos pertenezcan", ok);
  print_test("Verifico que la cantidad sea la correcta", abb_cantidad(arbol) == cantidad);
  //Borro todas las claves en el arbol.
  for (size_t j = 0; j <  cantidad; j++){
    if (strcmp((char*)abb_borrar(arbol, claves[j]), claves[j]) != 0) ok = false;
  }
  print_test("Borrar todos los elementos del arbol", ok);
  print_test("Verifico que la cantidad sea la correcta ", abb_cantidad(arbol) == 0);

  abb_destruir(arbol);//destruyo el arbol
  print_test("Destruyo el arbol exitosamente", true);


  arbol = abb_crear(cmp, NULL);//creo otro arbol.
  printf("\n~~~~~ PRUEBAS ITERADOR ABB MUCHOS ELEMENTOS ~~~~~\n");
  //Agrego muchos elementos al arbol.
  cargar_datos(arbol, claves, cantidad/2, cantidad - 1, 0);
  abb_iter_t *iterador = abb_iter_in_crear(arbol);//creo el iterador
  ok = true;
  //Controlo que el iterador, recorra todos los elementos.
  for (int j = 0; j < cantidad; j++){
    if(strcmp(abb_iter_in_ver_actual(iterador), claves[j]) != 0) ok = false;
    abb_iter_in_avanzar(iterador);
  }

  print_test("Avanzar y ver actual iterador para todos las claves", ok);
	print_test("Iterador al final", abb_iter_in_al_final(iterador));
	print_test("Iterador no avanza mas", !abb_iter_in_avanzar(iterador));
	print_test("Actual es NULL", !abb_iter_in_ver_actual(iterador));

  abb_iter_in_destruir(iterador);
  abb_destruir(arbol);
  liberar_claves(claves, cantidad);
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
  pruebas_abb_muchos_elementos(CANT_CLAVES_MUCHAS);
  pruebas_iterador_interno();
}
