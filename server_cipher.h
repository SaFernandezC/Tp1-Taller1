#ifndef CIPHER_H
#define CIPHER_H

#include <stdbool.h>

int calcular_rango_matiz(int key_len);

void cargar_matriz(int matriz[4][4], char* clave, int rango);

int mapear_caracteres(char* str, int string_size, char* resultado);

int calcular_tamanio(int rango_matriz, int caracteres_validos);

int ajustar_longitud(char** result, int rango_matriz, int valid_caract);

void calculos(int matriz[4][4], int rango_matriz, char* msg, int largo_msg);

bool es_caracter_valido(char caracter);


#endif // CIPHER_H
