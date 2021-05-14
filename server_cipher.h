#ifndef CIPHER_H
#define CIPHER_H

#define MAX_RANGO_MAT 4
#define OK 0
#define ERROR -1

struct cipher_t{
  char* key;
};

#include <stdbool.h>

/*
 * Todas la funciones int devuelven 0 (OK) en caso de exito y
 * -1 (ERROR) ante fallas
 */


 /*
  * Contructor
  */
int cipher_create(struct cipher_t* self, char* key);

/*
 * Destructor
 */
int cipher_destroy(struct cipher_t* self);

/*
 * Dada la long de la clave retorna el
 * rango de la matriz a utilizar.
 */
int cipher_calcular_rango_matiz(struct cipher_t* self);

/*
 * Carga la matriz recibida con los valores almacenados en clave.
 */
void cipher_cargar_matriz(struct cipher_t* self,
                          int matriz[MAX_RANGO_MAT][MAX_RANGO_MAT], int rango);


/*
* Mapea los caracteres de str a numeros (segun el protocolo)
* y los almacena en resultado.
*/
int cipher_map_characters(char* str, int string_size, char* resultado);

/**
 * Calcula el tamanio que debe tener el vector de chars de
 * forma tal que sea multiplo del rango de la matriz.
 */
int calcular_tamanio(int rango_matriz, int caracteres_validos);

/**
 * De ser necesario, ajusta la longitud del vector result de forma tal
 * que su largo sea multiplo del rango matriz y todos sus caracteres
 * sean validos.
 */
int cipher_adjust_len(char** result, int rango_matriz, int valid_caract);

/**
 * Realiza la multiplicacion de la matriz con cada subvector del mensaje.
 */
void cipher_cifrar_msg(int matriz[MAX_RANGO_MAT][MAX_RANGO_MAT],
                      int rango_matriz, char* msg, int largo_msg);

/*
* Define si el caracter recibido es valido o no para el cifrado.
*/
bool cipher_is_valid_char(char caracter);


#endif // CIPHER_H
