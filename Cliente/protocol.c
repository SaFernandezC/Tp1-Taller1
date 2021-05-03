#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "protocol.h"
#include "socket.h"





int protocol_create(struct protocol_t* self, char* file){
  self->file_name = file;
  return 0;
}

int protocol_send_line(struct socket_t* skt, char* line, unsigned short len){
  unsigned short len_big_endian = htons(len);
  int status = socket_send_msg(skt, (char*)&len_big_endian, 2);
  if (status != -1) {
    status = socket_send_msg(skt, line, len);
  }
  return status;
}

int protocol_recv_line(struct socket_t* skt, char** line, unsigned short* line_size){

  int estado = socket_recv_msg(skt, (char*)line_size, 2);
  *line_size = ntohs(*line_size) + 1; //SUMO 1 por el \0 que debo agregar luego

  if(*line_size > 0 && estado != -1){
    if (*line_size > strlen(*line)) { //SUMO 1 POR EL /0
      char* aux = realloc(*line, *line_size*sizeof(char));
      if(!aux){
        return -1;
      }
      *line = aux;
    }
    estado = socket_recv_msg(skt, *line, *line_size-1);
    return estado;
  }
  return -1;
}

void protocol_map_line(char* buffer, int buf_size){

  for (int i = 0; i < buf_size - 1; i++) {
    buffer[i] = buffer[i] + 65;
  }
  buffer[buf_size - 1] = '\0';
}

int protocol_open_file(FILE** file, char* file_name){
  if(strcmp(file_name, "-") == 0){
    *file = stdin;
  } else{
    *file = fopen(file_name, "r");
    if(!*file){
      return -1;
    }
  }
}

int protocol_run(struct protocol_t* self, const char* host, const char* service){

  struct socket_t skt;

  if(socket_connect(&skt, host, service) == -1){
    // printf("ERROR\n");
    return -1;
  }

  FILE* file;
  if(protocol_open_file(&file,self->file_name) == -1){
    socket_close(&skt);
    return -1;
  }

  char* line = NULL;
  size_t len = 0;
  unsigned short bytes_leidos;
  int status = 0;

  bytes_leidos = getline(&line, &len, file);

  while(!feof(file) && status != -1){

    status = protocol_send_line(&skt, line, bytes_leidos);
    if(status != -1){
      status = protocol_recv_line(&skt, &line, &bytes_leidos);
    }
    if(status != -1){
      protocol_map_line(line, bytes_leidos);
      printf("%s\n", line);
    }
    bytes_leidos = getline(&line, &len, file);
  }

  free(line);
  fclose(file);
  socket_close(&skt);

  if(status == -1){
    return -1;
  }
  return 0;
}

int protocol_destroy(struct protocol_t* self){
  return 0;
}
