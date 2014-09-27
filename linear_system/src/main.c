#include "input.h"
#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* lss - linear system solver */
struct lss_config
{
  char *input_filename;
  int has_input_file;
};

static struct lss_config lconfig = {
  .input_filename = (char *) NULL,
  .has_input_file = 0,
};

struct option options[] = {
  { .name = "input-file",    .val = 'i',  .has_arg = required_argument },
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
          lconfig.input_filename = optarg;
          lconfig.has_input_file = 0;
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }
}


int
main(int argc, char ** argv)
{
  process_options(argc, argv);

  struct vector v1, v2;
  struct simple_matrix m1, m2;
  
  FILE *vector_file = fopen("file_vector", "r");
  if (read_vector(vector_file, &v1) == ER_CORRECT)
    print_vector(stdout, &v1);
  fclose(vector_file);

  FILE *matrix_file = fopen("file_matrix", "r");
  if (read_simple_matrix(matrix_file, &m1) == ER_CORRECT)
    print_simple_matrix(stdout, &m1);
  fclose(matrix_file);

  FILE *extended_file = fopen("file_extended", "r");
  if (read_extended_matrix(extended_file, &m2, &v2) == ER_CORRECT)
    print_extended_matrix(stdout, &m2, &v2);
  fclose(extended_file);

  return 0;
}

