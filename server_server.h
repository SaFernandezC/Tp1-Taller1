#ifndef SERVER_H
#define SERVER_H

#include "socket.h"

struct server_t {
    int fd;
    char* key;
};

int server_create(struct server_t* self, char* key);

int server_destroy(struct server_t* self);

int server_run(struct server_t* self, char* service);

int server_bind_listen(struct server_t* self, char* service);

int server_accept(struct server_t* self, struct socket_t* socket);

int server_close(struct server_t* self);

#endif // SERVER_H
