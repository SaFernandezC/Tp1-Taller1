#include "cipher.h"

#include <stdlib.h>


int calcular_rango_matiz(int key_len){
  if(key_len == 16){
    return 4;
  } else if (key_len == 9) {
    return 3;
  } else {
    return 2;
  }
}


void cargar_matriz(int matriz[4][4], char* clave, int rango){

  int posicion = 0;

  for (size_t i = 0; i < rango; i++) {
    for (size_t j = 0; j < rango; j++) {
      matriz[i][j] = (int)clave[posicion] - 65;
      posicion++;
    }
  }
}

bool es_caracter_valido(char caracter){
  if (caracter < 'A' || caracter > 'Z'){
    return false;
  }
  return true;
}

int mapear_caracteres(char* string, int string_size, char* resultado){

  char letra_actual;
  int contador = 0;

  for (int i = 0; i < string_size; i++) {

    letra_actual = string[i];

    if(es_caracter_valido(letra_actual)){
      resultado[contador] = (int)letra_actual-65;
      contador++;
    }
  }
  return contador; //Retona la cantidad de caracteres_validos del string;
}

int calcular_tamanio(int rango_matriz, int caracteres_validos){
  if(caracteres_validos%rango_matriz == 0){
    return caracteres_validos;
  }
  return caracteres_validos + rango_matriz - (caracteres_validos%rango_matriz);
}

int ajustar_longitud(char** resultado, int rango_matriz, int caracteres_validos){
  int nueva_longitud = calcular_tamanio(rango_matriz, caracteres_validos);

  if(nueva_longitud == caracteres_validos){
    return nueva_longitud; //LA LONG ESTA BIEN
  }

  char* aux = realloc(*resultado, nueva_longitud*sizeof(char));
  if(!aux){
    return -1;
  }

  *resultado = aux;

  if(nueva_longitud > caracteres_validos){
    for (int i = caracteres_validos; i < nueva_longitud; i++) {
      aux[i] = 0;
    }
  }

  return nueva_longitud;
}

void calculos(int matriz[4][4], int rango_matriz, char* mensaje, int largo_mensaje){

  int vector_aux[rango_matriz];
  int posicion_vector = 0;
  int aux = 0;

  while (posicion_vector < largo_mensaje) {
    for (int i = 0; i < rango_matriz; i++) {
      aux = 0;
      for (int j = 0; j < rango_matriz; j++) {
        aux += matriz[i][j]*mensaje[posicion_vector+j];
      }
      vector_aux[i] = aux;
    }

    for (int i = 0; i < rango_matriz; i++) {
      mensaje[posicion_vector+i] = vector_aux[i]%26;
    }

    posicion_vector += rango_matriz;

  }

}
