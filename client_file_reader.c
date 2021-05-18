#include "client_file_reader.h"

#define IGUAL 0
#define STDIN_CODE "-"

int file_reader_create(struct file_reader_t* self, char* file_name){
  if (!self){
    return ERROR;
  }
  self->file = NULL;
  self->file_name = file_name;
  return OK;
}

int file_reader_open_file(struct file_reader_t* self){
  if (!self || !self->file_name){
    return ERROR;
  }
  if (strcmp(self->file_name, STDIN_CODE) == IGUAL){
    self->file = stdin;
  } else{
    self->file = fopen(self->file_name, "r");
    if (!self->file){
      return ERROR;
    }
  }
  return OK;
}

int file_reader_close_file(struct file_reader_t* self){
  if (!self || !self->file){
    return ERROR;
  }

  if (self->file != stdin){
    fclose(self->file);
  }
  return OK;
}

int file_reader_read_line(struct file_reader_t* self, char** line, size_t* len){
  if (!self || !self->file){
    return ERROR;
  }
  return getline(line, len, self->file);
}

int file_reader_invalid_file(struct file_reader_t* self){
  return feof(self->file);
}

int file_reader_destroy(struct file_reader_t* self){
  if (!self || !self->file){
    return ERROR;
  }
  file_reader_close_file(self);
  self->file_name = 0;
  return OK;
}
