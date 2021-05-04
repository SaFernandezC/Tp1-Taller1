#include "common_socket.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> //Close la utiliza



int socket_set_fd(struct socket_t* self, int fd){
  self->sock_fd = fd;
  return OK;
}

int socket_connect(struct socket_t* self, const char* host,
                  const char* service){
  struct addrinfo hints, *server_info;

  memset(&hints, 0, sizeof(struct addrinfo));
  // printf("Conexion a %s - %s\n",host, service);

  int result = getaddrinfo(host, service, &hints, &server_info);
  if (result != OK){
    return ERROR;
  }

  self->sock_fd = socket(server_info->ai_family, server_info->ai_socktype,
                        server_info->ai_protocol);
  if (self->sock_fd == ERROR){
    freeaddrinfo(server_info);
    return ERROR;
  }

  int status = connect(self->sock_fd, server_info->ai_addr,
                      server_info->ai_addrlen);
  if (status == ERROR){
    freeaddrinfo(server_info);
    return ERROR;
  }

  freeaddrinfo(server_info);
  return OK;
}

int socket_close(struct socket_t* self){
  shutdown(self->sock_fd, SHUT_RDWR); //Cierro comunicacion (RD y WR)
  close(self->sock_fd); // Destruyo el socket
  return OK;
}

int socket_send_msg(struct socket_t* self, char* buf, int size){
  int sent = 0;
  bool valid_socket = true;

  while (sent < size && valid_socket) {
     int bytes = send(self->sock_fd, &buf[sent], size-sent, MSG_NOSIGNAL);

     if (bytes == 0) {
        valid_socket = false;
     } else if (bytes == -1) {
        valid_socket = false;
     } else {
        sent += bytes;
     }
  }

  if (valid_socket) {
     return sent;
  } else {
     return ERROR;
  }
}

int socket_recv_msg(struct socket_t* self, char* buf, int size){
  int received = 0;
  int bytes = 0;
  bool valid_socket = true;

  while (received < size && valid_socket) {
    bytes = recv(self->sock_fd, &buf[received], size-received, 0);

    if (bytes == 0) { // nos cerraron el socket :(
      valid_socket = false;
    } else if (bytes == -1) { // hubo un error >(
      valid_socket = false;
    } else {
      received += bytes;
    }
  }

  if (valid_socket) {
    return received;
  } else {
    return ERROR;
  }
}
