#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include "abb.c"
int cmp (const char* clave1, const char* clave2){
    if (atoi(clave1) > atoi(clave2))
        return -1;
    if (atoi(clave1) < atoi(clave2))
        return 1;
    return 0;
}
void pruebas_abb_alumno(void){
    abb_t* arbol = abb_crear(cmp, free);
    char uno = '1';
    char dos = '2';
    char tres = '3';
    char cuatro = '4';
    /*arbol->raiz = nodo_abb_crear(&dos,NULL);
    arbol->raiz->izq = nodo_abb_crear(&uno,NULL);
    arbol->raiz->der = nodo_abb_crear(&tres,NULL);*/
    print_test("guardar dos", abb_guardar(arbol,&dos,NULL));
    print_test("guardar tres", abb_guardar(arbol,&tres,NULL));
    print_test("guardar uno", abb_guardar(arbol,&uno,NULL));
    print_test("guardar uno", abb_guardar(arbol,&cuatro,NULL));
    print_test("dos pertenece", abb_pertenece(arbol,&dos));
    print_test("tres pertenece", abb_pertenece(arbol,&tres));
    print_test("uno pertenece", abb_pertenece(arbol,&uno));
    print_test("4 pertenece", abb_pertenece(arbol,&cuatro));
    print_test("cant = 4", abb_cantidad(arbol)==4);
    abb_destruir(arbol);

}

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(int argc, char *argv[])
{
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    pruebas_abb_alumno();

    return failure_count() > 0;
}
