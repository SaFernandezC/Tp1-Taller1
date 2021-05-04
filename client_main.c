#include <stdio.h>
#include "protocol.h"

#define ARGS_HOST 1
#define ARGS_PORT 2
#define ARGS_FILE 3

int main(int argc, char** argv) {
  if(argc < 4){
    printf("Uso: ./client <hostname> <servicename> <filename>");
    return -1;
  }

  struct protocol_t protocol;
  const char* host = argv[ARGS_HOST];
  const char* service = argv[ARGS_PORT];
  char* file = argv[ARGS_FILE];

  protocol_create(&protocol, file);

  protocol_run(&protocol, host, service);

  protocol_destroy(&protocol);

  return 0;
}
