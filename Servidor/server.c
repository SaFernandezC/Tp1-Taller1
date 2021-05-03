#include "server.h"
#include "cipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> //Close la utiliza
#include <arpa/inet.h> // inet_ntop //VVER QUE ES


int server_create(struct server_t* self, char* key){
  self->key = key;
  self->fd = -1;
  return 0;
}

int server_destroy(struct server_t* self) {
    server_close(self);
    self->fd = -1;
    return 0;
}

/*-----------------------------------------------------------*/



int server_recive_line(struct socket_t* skt, char** msg, int* msg_size){
  unsigned short bytes_to_read = 0;
  int estado = socket_recv_msg(skt, (char*)&bytes_to_read, 2);
  bytes_to_read = ntohs(bytes_to_read);

  if (bytes_to_read > 0 && estado != -1 ) {
    if (bytes_to_read > *msg_size) {
      char* aux = (char*)realloc(*msg, bytes_to_read*sizeof(char));
      if(!aux){
        return -1; //PONER CTE
      }
      *msg = aux;
    }
    *msg_size = bytes_to_read;
    estado = socket_recv_msg(skt, *msg, *msg_size);
    return estado;
  }
  return -1;
}

int server_send_line(struct socket_t* skt, char** msg, int msg_size){
  unsigned short bytes_to_send = htons(msg_size);
  int estado = socket_send_msg(skt, (char*)&bytes_to_send, 2);

  if(estado != -1){
    estado = socket_send_msg(skt, *msg, msg_size);
  }

  return estado;
}

int reservar_memoria(char** ptr, int size){
  char* aux = NULL;
  aux = realloc(*ptr ,size*sizeof(char));
  if(!aux){
    return -1;
  }
  *ptr = aux;
  return 0;
}

/*-----------------------------------------------------------*/

int server_run(struct server_t* self, char* service){

  if (server_bind_listen(self, service)) {
    // printf("Error al tomar puerto");
    return -1;
  }

  struct socket_t skt;
  if(server_accept(self, &skt) == -1){
    // printf("Error al acceder al cliente\n");
    return -1;
  }


  int matriz[4][4];
  int rango_matriz = calcular_rango_matiz(strlen(self->key));
  cargar_matriz(matriz, self->key, rango_matriz);


  /*--------------------WHILE--------------------*/
  char* buffer = NULL;
  char* mensaje = NULL;
  int buffer_size = 0;

  int caracteres_validos = 0;
  int nueva_longitud = 0;
  int status = 0;

  while(status != -1){
    status = server_recive_line(&skt, &buffer, &buffer_size);

    if(status != -1){
      status = reservar_memoria(&mensaje, (int)buffer_size-1);
    }
    if(status != -1){
      caracteres_validos = mapear_caracteres(buffer, buffer_size, mensaje);
      nueva_longitud = ajustar_longitud(&mensaje, rango_matriz, caracteres_validos);
      if(nueva_longitud != -1){
        calculos(matriz, rango_matriz, mensaje, nueva_longitud);
        status = server_send_line(&skt, &mensaje, nueva_longitud);
      }
    }
  }

  free(buffer);
  free(mensaje);

  if(status == -1){
    return status;
  }
  return 0;
}

int server_accept(struct server_t* self, struct socket_t* skt){//SACAR EL PARAMETRO 2
  char addr_buf[INET_ADDRSTRLEN];
  struct sockaddr_in address;
  socklen_t addr_len = (socklen_t) sizeof(address);

  int fd = accept(self->fd, (struct sockaddr *)&address, &addr_len);
  if(fd == -1){
    return fd;
  }

  inet_ntop(AF_INET, &(address.sin_addr), addr_buf, INET_ADDRSTRLEN);
  // printf("Se conectó un usuario: %s, con el fd %i\n", addr_buf, fd);

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

  status = getaddrinfo(NULL, service, &hints, &server_info); //CHEQUEAR ERRORES QUE PUEDE DEVOLVER ESTO
  if(status != 0){
    return -1;
  }

  self->fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
  if(self->fd  == -1){
    return -1;
  }

  int val = 1;
  status = setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
  if(status == -1){
    freeaddrinfo(server_info);
    return status;
  }

  status = bind(self->fd, server_info->ai_addr, server_info->ai_addrlen);
  if(status == -1){
    close(self->fd);
    freeaddrinfo(server_info);
    return status;
  }

  freeaddrinfo(server_info);

  status = listen(self->fd, 5); // VER ESE 5
  if(status == -1){
    close(self->fd);
    return status;
  }

  return 0;

}

int server_close(struct server_t* self) {
    shutdown(self->fd, 2);
    close(self->fd);
    return 0;
}
