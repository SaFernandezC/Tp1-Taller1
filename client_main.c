#include <stdio.h>
#include "client_protocol.h"

#define ARGS_HOST 1
#define ARGS_PORT 2
#define ARGS_FILE 3
#define ARGC_SIZE 4

int main(int argc, char** argv) {
  if (argc != ARGC_SIZE){
    printf("Uso: ./client <hostname> <servicename> <filename>");
    return -1;
  }

  struct protocol_t protocol;
  const char* host = argv[ARGS_HOST];
  const char* service = argv[ARGS_PORT];
  char* file = argv[ARGS_FILE];
  int status = 0;

  status = protocol_create(&protocol, file);
  if (status != 0){
    return status;
  }

  status = protocol_run(&protocol, host, service);
  if (status != 0){
    return status;
  }

  status = protocol_destroy(&protocol);
  if (status != 0){
    return status;
  }

  return 0;
}
