#include "config_parser.h"
#include "array.h"

#include <stdio.h>
#include <inttypes.h>

struct config_section
{
  size_t begin;
  size_t end;
  char *name;
};


struct config_value
{
  union
  {
    long _long;
    char *_string;
    bool _bool;
  } value;
};


struct config_node
{
  char *name;
  size_t name_len;
  struct config_node *parent;
  struct array *children;
  struct config_section *section;
};


static struct input_file config_file = {
  .ih = NULL,
  .len = 0,
  .fd = 0,
};
static struct config_section root_section;
static struct config_node *root_node;


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
  root_section.name = "ROOT";
  root_section.begin = 0;
  root_section.end = config_file.len;

  root_node = (struct config_node *) malloc(sizeof(struct config_node));
  root_node->name = "ROOT";
  root_node->name_len = 4;
  root_node->parent = NULL;
  root_node->children = (struct array *)malloc(sizeof(struct array));
  init_array(root_node->children, sizeof(struct config_node));
  root_node->section = &root_section;
}


static
void
destroy_config_sub_tree(struct config_node *node)
{
  for (size_t i = 0; i < node->children->count; ++i)
    destroy_config_sub_tree((struct config_node *) 
                            get_element(node->children, i));
  destroy_array(node->children);
  free(node->children);
  free(node);
}


static
void
destroy_config_tree(void)
{
  destroy_config_sub_tree(root_node);
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
  destroy_config_tree();
  close_file(config_file);
}
