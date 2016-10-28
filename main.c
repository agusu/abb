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
    abb_t* arbol = abb_crear(cmp, NULL);
    char uno = '1';
    char dos = '2';
    char tres = '3';
    char cuatro = '4';
    char cinco = '5';
    char seis = '6';
    print_test("guardar cuatro", abb_guardar(arbol,&cuatro,&cuatro));
    print_test("guardar dos", abb_guardar(arbol,&dos,&dos));
    print_test("guardar tres", abb_guardar(arbol,&tres,&tres));
    print_test("guardar uno", abb_guardar(arbol,&uno,&uno));
    print_test("guardar cinco", abb_guardar(arbol,&cinco,&cinco));
    print_test("guardar seis", abb_guardar(arbol,&seis,&seis));
    print_test("dos pertenece", abb_pertenece(arbol,&dos));
    print_test("tres pertenece", abb_pertenece(arbol,&tres));
    print_test("uno pertenece", abb_pertenece(arbol,&uno));
    print_test("cuatro pertenece", abb_pertenece(arbol,&cuatro));
    print_test("cinco pertenece", abb_pertenece(arbol,&cinco));
    print_test("seis pertenece", abb_pertenece(arbol,&seis));

    print_test("obtener hoja", (char*)abb_obtener(arbol,&seis) == &seis);
    abb_borrar(arbol,&seis);
    print_test("borrar hoja", !abb_pertenece(arbol,&seis));
    print_test("cant correcta", abb_cantidad(arbol)==5);

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
