#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <string.h>

#define OK 0
#define ERROR -1


struct file_reader_t {
  FILE* file;
  char* file_name;
};

/*
 * Todas la funciones devuelven 0 (OK) en caso de exito y
 * -1 (ERROR) ante fallas
 */

/*
 * Contructor
 */
int file_reader_create(struct file_reader_t* self, char* file_name);

/*
 * Lee una linea del arcihvo almacenandola en el char* line
 * retorna la cantidad de bytes leidos
 */
int file_reader_read_line(struct file_reader_t* self, char** line, size_t* len);

/*
 * Abre el archivo
 */
int file_reader_open_file(struct file_reader_t* self);

/*
 * Cierra el archivo
 */
int file_reader_close_file(struct file_reader_t* self);

/*
 * Retorna -1 si se llego al final del archivo
 */
int file_reader_invalid_file(struct file_reader_t* self);

/*
 * Destructor
 */
int file_reader_destroy(struct file_reader_t* self);


#endif // FILE_READER_H
