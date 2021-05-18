#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common_socket.h"
#include "client_file_reader.h"

#define OK 0
#define ERROR -1

struct protocol_t {
    struct socket_t socket;
    struct file_reader_t f_reader;
};

/*
 * Todas la funciones devuelven 0 (OK) en caso de exito y
 * -1 (ERROR) ante fallas
 */

/*
 * Contructor
 */
int protocol_create(struct protocol_t* self, char* file_name);

/*
 * Destructor
 */
int protocol_destroy(struct protocol_t* self);

/*
 * Le indica al server que se conecte al host. Luego realiza
 * la tarea de leer linea -> enviar --> recibir --> descifrar --> print,
 * como lo indica el protocolo.
 */
int protocol_run(struct protocol_t* self, const char* host,
                  const char* service);

/*
* Envia (a traves del skt) la long de la linea y luego la misma.
*/
int protocol_send_line(struct socket_t* skt, char* line,
                      unsigned short len);

/*
* Recibe (a traves del skt) la long de la linea y luego la misma.
*/
int protocol_recv_line(struct socket_t* skt, char** line,
                      unsigned short* line_size);

/*
* Mapea el mensaje recibido segun el protocolo.
*/
void protocol_map_line(char* buffer, int buf_size);

/*
* Imprime tantos caracteres de buffer como le indique buf_size.
* Luego imprime un /n
*/
void protocol_print_line(char* buffer, int buf_size);

#endif // CLIENT_H
