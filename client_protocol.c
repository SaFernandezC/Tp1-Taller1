#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "client_protocol.h"

#define SIZE_MSG_LEN 2
#define VACIO 0
#define IGUAL 0

int protocol_create(struct protocol_t* self, char* file_name){
  int status = socket_create(&self->socket);
  if (status == ERROR){
    return ERROR;
  }
  return file_reader_create(&self->f_reader, file_name);
}

int protocol_send_line(struct socket_t* skt, char* line,
                      unsigned short len){
  unsigned short len_big_endian = htons(len);
  int status = socket_send_msg(skt, (char*)&len_big_endian, SIZE_MSG_LEN);
  if (status != ERROR) {
    status = socket_send_msg(skt, line, len);
  }
  return status;
}

int protocol_recv_line(struct socket_t* skt, char** line,
                      unsigned short* line_size){
  unsigned short bytes_to_read;

  int estado = socket_recv_msg(skt, (char*)&bytes_to_read, SIZE_MSG_LEN);
  bytes_to_read = ntohs(bytes_to_read);

  if (bytes_to_read > VACIO && estado != ERROR){
    if (bytes_to_read > *line_size) {
      char* aux = realloc(*line, bytes_to_read*sizeof(char));
      if (!aux){
        return ERROR;
      }
      *line = aux;
    }
    estado = socket_recv_msg(skt, *line, bytes_to_read);
    *line_size = bytes_to_read;
    return estado;
  }

  if (bytes_to_read == VACIO){
    *line_size = VACIO;
    return OK;
  }

  return ERROR;
}

void protocol_map_line(char* buffer, int buf_size){
  if (buf_size == VACIO){
    return;
  }
  for (int i = 0; i < buf_size; i++) {
    buffer[i] = buffer[i] + 65;
  }
}

void protocol_print_line(char* buffer, int buf_size){
  for (int i = 0; i < buf_size && buf_size > 0; i++) {
    printf("%c", buffer[i]);
  }
  printf("\n");
  return;
}

int protocol_run(struct protocol_t* self, const char* host,
                  const char* service){
  if (socket_connect(&self->socket, host, service) == ERROR){
    return ERROR;
  }

  if (file_reader_open_file(&self->f_reader) == ERROR){
    socket_close(&self->socket);
    return ERROR;
  }

  char* line = NULL;
  size_t len = 0;
  unsigned short bytes_leidos;
  int status = 0;

  bytes_leidos = file_reader_read_line(&self->f_reader, &line, &len);

  while (!file_reader_invalid_file(&self->f_reader) && status != ERROR){
    status = protocol_send_line(&self->socket, line, bytes_leidos);
    if (status != ERROR){
      status = protocol_recv_line(&self->socket, &line, &bytes_leidos);
    }
    if (status != ERROR){
      protocol_map_line(line, bytes_leidos);
      protocol_print_line(line, bytes_leidos);
    }
    if (line){
      free(line);
      len = VACIO;
    }
    bytes_leidos = file_reader_read_line(&self->f_reader, &line, &len);
  }



  free(line);

  if (status == ERROR){
    return ERROR;
  }
  return OK;
}

int protocol_destroy(struct protocol_t* self){
  file_reader_destroy(&self->f_reader);
  socket_destroy(&self->socket);
  return OK;
}
