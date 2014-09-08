#include "input.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

struct input_file
open_file(const char * filename)
{
  FILE *in_f = fopen(filename, "r");
  if (! in_f)
    {
      fprintf(stderr, "Cannot open file %s\n", filename);
      exit(2);
    }
  size_t len;
  fseek(in_f, 0, SEEK_END);
  len = ftell(in_f);
  fseek(in_f, 0, SEEK_SET);
  fclose(in_f);

  int fd = open(filename, O_RDONLY);
  void *data = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED)
    {
      fprintf(stderr, "Cannot mmap file %s\n", filename);
      exit(2);
    }
  input_handle ih = (input_handle) data;

  struct input_file result = {
    .ih = ih,
    .len = len,
    .fd = fd,
  };
  return result;
}

void
close_file(struct input_file in_f)
{
  munmap((void *) in_f.ih, in_f.len);
  close(in_f.fd);
}
