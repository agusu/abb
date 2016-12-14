#include "abb.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>

int cmp (const char* clave1, const char* clave2){
    if (atoi(clave1) < atoi(clave2))
        return -3;
    if (atoi(clave1) > atoi(clave2))
        return 2;
    return 0;
}

bool imprimir_n_primeros(const char *clave, void *dato, void *extra){
  int *pos = extra;
  if(*pos == 0) return false;
  (*pos)--;
  printf("La clave es: %s y el dato es: %s\n", clave, (char*)dato);
  return true;
}

bool calcular_promedio(const char *clave, void *dato, void *extra){
  int *promedio = extra;
  char *numero = dato;
  *promedio += atoi(numero);
  return true;
}

void pruebas_iter_post(void){
  abb_t* arbol = abb_crear(cmp, NULL);

  char *uno = "1";
  char *dos = "2";
  char *tres = "3";
  char *cuatro = "4";
  char *cinco = "5";
  char *nueve = "9";
  int promedio = 0, cant = 4;
  print_test("Guardo un elemento en el arbol (3)", abb_guardar(arbol, tres, tres));
  print_test("Guardo un elemento en el arbol (5)", abb_guardar(arbol, cinco, cinco));
  print_test("Guardo un elemento en el arbol (2)", abb_guardar(arbol, dos, dos));
  print_test("Guardo un elemento en el arbol (4)", abb_guardar(arbol, cuatro, cuatro));
  print_test("Guardo un elemento en el arbol (1)", abb_guardar(arbol, uno, uno));
  print_test("Guardo un elemento en el arbol (9)", abb_guardar(arbol, nueve, nueve));

  abb_post_order(arbol, imprimir_n_primeros, &cant);

  abb_post_order(arbol, calcular_promedio, &promedio);

  print_test("Calcular el promedio del arbol", promedio / 6 == 4 );

  abb_iter_post_t *iter = abb_iter_post_crear(arbol);
  print_test("El primer elemento es (1)", strcmp(abb_iter_post_ver_actual(iter), uno) == 0);
  abb_iter_post_avanzar(iter);
  print_test("El segundo elemento es (2)", strcmp(abb_iter_post_ver_actual(iter), dos) == 0);
  abb_iter_post_avanzar(iter);
  print_test("El tercer elemento es (4)", strcmp(abb_iter_post_ver_actual(iter), cuatro) == 0);
  abb_iter_post_avanzar(iter);
  print_test("El cuarto elemento es (9)", strcmp(abb_iter_post_ver_actual(iter), nueve) == 0);
  abb_iter_post_avanzar(iter);
  print_test("El quinto elemento es (5)", strcmp(abb_iter_post_ver_actual(iter), cinco) == 0);
  abb_iter_post_avanzar(iter);
  print_test("El sexto elemento es (3)", strcmp(abb_iter_post_ver_actual(iter), tres) == 0);
  abb_iter_post_avanzar(iter);
  print_test("No se puede avanzar más", !abb_iter_post_avanzar(iter));
  print_test("Esta al final el iterador", abb_iter_post_al_final(iter));
  abb_iter_post_destruir(iter);
  abb_destruir(arbol);

}
void pruebas_abb_alumno(void){
  pruebas_iter_post();
}
