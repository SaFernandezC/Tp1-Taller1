#ifndef SOCKET_H
#define SOCKET_H

struct socket_t {
    int sock_fd;
};


int socket_create(struct socket_t* self);

int socket_destroy(struct socket_t* self);

int socket_connect(struct socket_t* self, const char* host, const char* service);

int socket_close(struct socket_t* self);

int socket_send_msg(struct socket_t* self, char* buf, int size);

int socket_recv_msg(struct socket_t* self, char* buf, int size);

int socket_set_fd(struct socket_t* self, int fd);


#endif // SOCKET_H
