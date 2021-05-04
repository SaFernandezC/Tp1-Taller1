#ifndef CIPHER_H
#define CIPHER_H

#define MAX_RANGO_MAT 4
#define OK 1
#define ERROR -1

#include <stdbool.h>

/*
 * Todas la funciones int devuelven 0 (OK) en caso de exito y
 * -1 (ERROR) ante fallas
 */

/*
 * Dada la long de la clave retorna el
 * rango de la matriz a utilizar.
 */
int calcular_rango_matiz(int key_len);

/*
 * Carga la matriz recibida con los valores almacenados en clave.
 */
void cargar_matriz(int matriz[MAX_RANGO_MAT][MAX_RANGO_MAT], char* clave,
                  int rango);


/*
* Mapea los caracteres de str a numeros (segun el protocolo)
* y los almacena en resultado.
*/
int mapear_caracteres(char* str, int string_size, char* resultado);

/**
 * Calcula el tamanio que debe tener el vector de chars de
 * forma tal que sea multiplo del rango de la matriz.
 */
int calcular_tamanio(int rango_matriz, int caracteres_validos);

/**
 * De ser necesario, ajusta la longitud del vector result de forma tal
 * que su largo sea multiplo del rango matriz.
 */
int ajustar_longitud(char** result, int rango_matriz, int valid_caract);

/**
 * Realiza la multiplicacion de la matriz con cada subvector del mensaje.
 */
void calculos(int matriz[MAX_RANGO_MAT][MAX_RANGO_MAT], int rango_matriz,
              char* msg, int largo_msg);

/*
* Define si el caracter recibido es valido o no para el cifrado.
*/
bool es_caracter_valido(char caracter);


#endif // CIPHER_H
