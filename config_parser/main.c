#include "config_parser.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

struct test_config
{
  char *config_filename;
};

static struct test_config tconfig = {
  .config_filename = (char *) NULL,
};

struct option options[] = {
  { .name = "config-file",   .val = 'c',  .has_arg = required_argument },
  { .name = "help",          .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *out, char *program_name)
{
  fprintf(out, 
          "Usage: %s [OPTIONS]\n\n"
          "  --config-file, -c [filename]    config file.\n"
          "  --help, -h                      print this message\n",
          program_name);
}


static
void
process_options(int argc, char ** argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "c:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'c':
          tconfig.config_filename = optarg;
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(2);
        }
    }
  if (! tconfig.config_filename)
    {
      fprintf(stderr, "No config file specified!\n");
      exit(2); 
    }
}

int
main(int argc, char ** argv)
{
  process_options(argc, argv);
  config_create(tconfig.config_filename);
  config_destroy();
  return 0;
}

