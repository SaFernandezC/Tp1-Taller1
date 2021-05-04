#ifndef SOCKET_H
#define SOCKET_H

#define OK 1
#define ERROR -1

struct socket_t {
    int sock_fd;
};

/*
 * Todas la funciones devuelven 0 (OK) en caso de exito y
 * -1 (ERROR) ante fallas
 */

/*
 * Contructor
 */
int socket_create(struct socket_t* self);

/*
 * Destructor
 */
int socket_destroy(struct socket_t* self);

/*
 * Dado un puerto y un servicio, crea el socket y establece la
 * conexion con el servidor
 */
int socket_connect(struct socket_t* self, const char* host,
                  const char* service);

/*
* Cierra la comunicacion y lo destruye.
*/
int socket_close(struct socket_t* self);

/*
 * Envia tantos bytes como se le indique y los almacena en buf.
 *
 */
int socket_send_msg(struct socket_t* self, char* buf, int size);

/*
 * Recibe tantos bytes como se le indique y los almacena en buf
 */
int socket_recv_msg(struct socket_t* self, char* buf, int size);

/*
 * Dado un file descriptor, se le asigna al socket.
 */
int socket_set_fd(struct socket_t* self, int fd);


#endif // SOCKET_H
