#include <stdio.h>


#include "server_server.h"

#define ARGS_PORT 1
#define ARGS_KEY 2
#define ARGC_SIZE 3

int main(int argc, char** argv) {
  if (argc != ARGC_SIZE){
    printf("Uso: ./server <servicename> <key>");
    return -1;
  }

  struct server_t server;
  char* service = argv[ARGS_PORT];
  char* key = argv[ARGS_KEY];

  server_create(&server, key);

  server_run(&server, service);

  server_destroy(&server);

  return 0;
}
