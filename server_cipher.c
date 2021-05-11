#include "server_cipher.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLAVE_LARGA 16
#define CLAVE_MEDIA 9
#define CLAVE_CHICA 4
#define MID_RANGO_MAT 3
#define MIN_RANGO_MAT 2
#define MIN_CHAR 'A'
#define MAX_CHAR 'Z'
#define VALOR_MAX_CARAC 26

int cipher_create(struct cipher_t* self, char* key){
  self->key = key;
  return OK;
}

int cipher_calcular_rango_matiz(struct cipher_t* self){
  int key_len = strlen(self->key);
  if (key_len == CLAVE_LARGA){
    return MAX_RANGO_MAT;
  } else if (key_len == CLAVE_MEDIA) {
    return MID_RANGO_MAT;
  } else {
    return MIN_RANGO_MAT;
  }
}


void cipher_cargar_matriz(struct cipher_t* self,
                          int matriz[MAX_RANGO_MAT][MAX_RANGO_MAT], int rango){
  int posicion = 0;

  for (size_t i = 0; i < rango; i++) {
    for (size_t j = 0; j < rango; j++) {
      matriz[i][j] = (int)self->key[posicion] - 65;
      posicion++;
    }
  }
}

bool es_caracter_valido(char caracter){
  if (caracter < MIN_CHAR || caracter > MAX_CHAR){
    return false;
  }
  return true;
}

int mapear_caracteres(char* str, int string_size, char* result){
  int contador = 0;

  for (int i = 0; i < string_size; i++) {
    char letra_actual = str[i];

    if (es_caracter_valido(letra_actual)){
      result[contador] = (int)letra_actual-65;
      contador++;
    }
  }
  return contador; //Retona la cantidad de caracteres_validos del string;
}

int calcular_tamanio(int rango_matriz, int caracteres_validos){
  if (caracteres_validos%rango_matriz == 0){
    return caracteres_validos;
  }
  return caracteres_validos + rango_matriz - (caracteres_validos%rango_matriz);
}

int ajustar_longitud(char** result, int rango_matriz, int valid_caract){
  int nueva_longitud = calcular_tamanio(rango_matriz, valid_caract);

  if (nueva_longitud == valid_caract){
    return nueva_longitud;
  }

  char* aux = realloc(*result, nueva_longitud*sizeof(char));
  if (!aux){
    return ERROR;
  }

  *result = aux;

  if (nueva_longitud > valid_caract){
    for (int i = valid_caract; i < nueva_longitud; i++) {
      aux[i] = 0;
    }
  }

  return nueva_longitud;
}

void calculos(int matriz[4][4], int rango_matriz, char* msg, int largo_msg){
  int vector_aux[4]; //Maximo posible
  int posicion_vector = 0;

  while (posicion_vector < largo_msg) {
    for (int i = 0; i < rango_matriz; i++) {
      int aux = 0;
      for (int j = 0; j < rango_matriz; j++) {
        aux += matriz[i][j]*msg[posicion_vector+j];
      }
      vector_aux[i] = aux;
    }

    for (int i = 0; i < rango_matriz; i++) {
      msg[posicion_vector+i] = vector_aux[i]%VALOR_MAX_CARAC;
    }

    posicion_vector += rango_matriz;
  }
}
