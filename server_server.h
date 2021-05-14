#ifndef SERVER_H
#define SERVER_H

#include "common_socket.h"
#include "server_cipher.h"

struct server_t {
    int fd;
    struct cipher_t cipher;
    struct socket_t socket;
};

/*
 * Todas la funciones devuelven 0 (OK) en caso de exito y
 * -1 (ERROR) ante fallas
 */

/*
 * Contructor
 */
int server_create(struct server_t* self, char* key);

/*
 * Destructor
 */
int server_destroy(struct server_t* self);

/*
 * Inicia el servidor, acepta el cliente y luego sigue
 * el protocolo del cifrado de escuchar/enviar
 * hasta que se cierre la conexion o surga un problema.
 */
int server_run(struct server_t* self, char* service);

/*
 * Abre un puerto e y luego escucha a los clientes.
 */
int server_bind_listen(struct server_t* self, char* service);

/*
 * Acepta cliente y asigna fd al socket de comunicacion.
 */
int server_accept(struct server_t* self, struct socket_t* socket);

/*
 * Recibe el largo del mensaje a recibir, si es
 * necesarioreserva memoria para el mismo y luego lo recibe.
 * Utiliza al socket skt para comunicarse.
 */
int server_recive_line(struct socket_t* skt, char** msg, int* msg_size);

/*
 * Envia el largo del mensaje, y luego envia el mismo.
 * Utiliza al socket skt para comunicarse.
 */
int server_send_line(struct socket_t* skt, char** msg, int msg_size);

/*
 * Cierra el server.
 */
int server_close(struct server_t* self);

#endif // SERVER_H
