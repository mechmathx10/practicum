#include "config_parser.h"
#include "array.h"

#include <stdio.h>


struct config_section
{
  size_t begin;
  size_t end;
  char *name;
};


struct config_node
{
  char *name;
  size_t name_len;
  struct config_node *parent;
  struct array *children;
};

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


static
void
init_config_tree(void)
{

}


int
config_create(const char * config_filename)
{
  config_file = open_file(config_filename);
  print_config_file(stdout);
  init_config_tree();
  return 1;
}



void
config_destroy(void)
{
  close_file(config_file);
}
