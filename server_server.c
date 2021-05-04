#include "server_server.h"
#include "server_cipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> //Close la utiliza
#include <arpa/inet.h> // inet_ntop -> Convierte addr de bin a text

#define SIZE_MSG_LEN 2
#define MAX_RANGO_MAT 4
#define VACIO 0
#define WAITING_CLIENTS_MAX 5


int server_create(struct server_t* self, char* key){
  self->key = key;
  self->fd = -1;
  return OK;
}

int server_destroy(struct server_t* self) {
    server_close(self);
    self->fd = -1;
    return OK;
}

int server_recive_line(struct socket_t* skt, char** msg, int* msg_size){
  unsigned short bytes_to_read = 0;
  int estado = socket_recv_msg(skt, (char*)&bytes_to_read, SIZE_MSG_LEN);
  bytes_to_read = ntohs(bytes_to_read);

  if (bytes_to_read > VACIO && estado != ERROR) {
    if (bytes_to_read > *msg_size) {
      char* aux = (char*)realloc(*msg, bytes_to_read*sizeof(char));
      if (!aux){
        return ERROR;
      }
      *msg = aux;
    }
    *msg_size = bytes_to_read;
    estado = socket_recv_msg(skt, *msg, *msg_size);
    return estado;
  }
  return ERROR;
}

int server_send_line(struct socket_t* skt, char** msg, int msg_size){
  unsigned short bytes_to_send = htons(msg_size);
  int estado = socket_send_msg(skt, (char*)&bytes_to_send, SIZE_MSG_LEN);

  if (estado != ERROR){
    estado = socket_send_msg(skt, *msg, msg_size);
  }

  return estado;
}

/*
 * Agranda/Achica la memoria de un bloque.
 */
int reservar_memoria(char** ptr, int size){
  char* aux = NULL;
  aux = realloc(*ptr ,size*sizeof(char));
  if (!aux){
    return ERROR;
  }
  *ptr = aux;
  return OK;
}

int server_run(struct server_t* self, char* service){
  if (server_bind_listen(self, service)) {
    return ERROR;
  }

  struct socket_t skt;
  if (server_accept(self, &skt) == ERROR){
    return ERROR;
  }


  int matriz[MAX_RANGO_MAT][MAX_RANGO_MAT];
  int rango_matriz = calcular_rango_matiz(strlen(self->key));
  cargar_matriz(matriz, self->key, rango_matriz);


  char* buffer = NULL;
  char* mensaje = NULL;
  int buffer_size = 0;
  int status = 0;

  while (status != ERROR){
    status = server_recive_line(&skt, &buffer, &buffer_size);

    if (status != ERROR){
      status = reservar_memoria(&mensaje, (int)buffer_size);
    }
    if (status != ERROR){
      int valid_caract = mapear_caracteres(buffer, buffer_size, mensaje);
      if (valid_caract == VACIO){
        status = server_send_line(&skt, &mensaje, VACIO);
      } else{
        int nueva_longitud = ajustar_longitud(&mensaje, rango_matriz, valid_caract);
        if (nueva_longitud != ERROR){
          calculos(matriz, rango_matriz, mensaje, nueva_longitud);
          status = server_send_line(&skt, &mensaje, nueva_longitud);
        }
      }
    }
  }

  free(mensaje);
  free(buffer);


  if (status == ERROR){
    return status;
  }
  return OK;
}

int server_accept(struct server_t* self, struct socket_t* skt){
  char addr_buf[INET_ADDRSTRLEN];
  struct sockaddr_in address;
  socklen_t addr_len = (socklen_t) sizeof(address);

  int fd = accept(self->fd, (struct sockaddr *)&address, &addr_len);
  if (fd == ERROR){
    return fd;
  }

  inet_ntop(AF_INET, &(address.sin_addr), addr_buf, INET_ADDRSTRLEN);

  socket_set_fd(skt, fd);
  return fd;
}

int server_bind_listen(struct server_t* self, char* service){
  int status;

  struct addrinfo hints, *server_info;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  status = getaddrinfo(NULL, service, &hints, &server_info);
  if (status != OK){
    return ERROR;
  }

  self->fd = socket(server_info->ai_family, server_info->ai_socktype,
                    server_info->ai_protocol);
  if (self->fd  == ERROR){
    return ERROR;
  }

  int val = 1;
  status = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
  if (status == ERROR){
    freeaddrinfo(server_info);
    return status;
  }

  status = bind(self->fd, server_info->ai_addr, server_info->ai_addrlen);
  if (status == ERROR){
    close(self->fd);
    freeaddrinfo(server_info);
    return status;
  }

  freeaddrinfo(server_info);

  status = listen(self->fd, WAITING_CLIENTS_MAX);
  if (status == ERROR){
    close(self->fd);
    return status;
  }

  return OK;
}

int server_close(struct server_t* self) {
    shutdown(self->fd, 2);
    close(self->fd);
    return OK;
}
