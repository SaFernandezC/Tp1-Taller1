#ifndef PROTOCOL_H
#define PROTOCOL_H

struct protocol_t {
    struct socket_t* socket;
    char* file_name;
};


int protocol_create(struct protocol_t* self, char* file);

int protocol_destroy(struct protocol_t* self);

int protocol_run(struct protocol_t* self, const char* host,
                  const char* service);

#endif // CLIENT_H
