#include "config_parser.h"

#include <stdio.h>


static struct input_file config_file = {
  .ih = NULL,
  .len = 0,
  .fd = 0,
};

static
void
print_config_file(FILE *out)
{
  fprintf(out, "%.*s\n", (int) config_file.len, (char *) config_file.ih);
}

int
config_create(const char * config_filename)
{
  config_file = open_file(config_filename);
  print_config_file(stdout);
  return 1;
}



void
config_destroy(void)
{
  close_file(config_file);
}
