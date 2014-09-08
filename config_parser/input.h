#ifndef INPUT_H
#define INPUT_H

#include <inttypes.h>
#include <stdlib.h>


typedef const uint8_t *input_handle;


struct input_file
{
  input_handle ih;
  size_t len;
  int fd;
};


struct input_file
open_file(const char *);


void
close_file(struct input_file);


#endif
