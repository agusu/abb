CC = gcc
C_FLAGS = -g -O2 -std=c99 -Wall -Wconversion -Wno-sign-conversion -Wbad-function-cast -Wshadow -Wpointer-arith -Wunreachable-code -Wformat=2 -Werror
DEFAULT = TPAbb
OBJ_ABB = abb
OBJ_PILA = pila
OBJ_COLA = cola
OBJ_LISTA = lista
OBJ_TESTING  = testing
OBJS =  $(OBJ_ABB).o $(OBJ_PILA).o $(OBJ_TESTING).o $(OBJ_LISTA).o $(OBJ_COLA).o
VALGRIND = valgrind
V_FLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

all: $(DEFAULT)

$(DEFAULT): clean $(OBJS)
		$(CC) $(C_FLAGS) main.c $(OBJS) pruebas_alumnos.c -o $(DEFAULT)

%.o : %.c %.c
		$(CC) $(CFLAGS) $< -c

run: $(DEFAULT)
		$(VALGRIND) $(V_FLAGS) ./$(DEFAULT)

clean_obj:
		rm -f *.o

clean: clean_obj
		rm -f $(DEFAULT)
