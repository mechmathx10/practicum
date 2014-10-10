#include "input.h"
#include "output.h"
#include "block_utils.h"
#include "matrix_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define DEFAULT_BLOCK_SIZE 3

/* ----------------------------------------------------------- */

/* lss - linear system solver */
struct lss_config
{
  char *input_filename;
  int has_input_file;
  int block_size;
};

static struct lss_config lconfig = {
  .input_filename = (char *) NULL,
  .has_input_file = 0,
  .block_size = 0,
};

struct option options[] = {
  { .name = "input_file",    .val = 'i',  .has_arg = required_argument },
  { .name = "block_size",    .val = 'b',  .has_arg = required_argument },
  { .name = "help",          .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *out, char *program_name)
{
  fprintf(out, 
          "Usage: %s [OPTIONS]\n\n"
          "  --input_file, -c [filename]     deprecated.\n"
          "  --block_size, -b [size]         matrix block size.\n"
          "  --help, -h                      print this message\n",
          program_name);
}

/* ----------------------------------------------------------- */

static
void
process_options(int argc, char ** argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "c:b:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'c':
          lconfig.input_filename = optarg;
          lconfig.has_input_file = 0;
          break;
        case 'b':
          lconfig.block_size = atoi(optarg);
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }
  if (lconfig.block_size < 1)
    {
      fprintf(stderr, "Block size is not set or negative and been set to: %d\n",
                      DEFAULT_BLOCK_SIZE);
      lconfig.block_size = DEFAULT_BLOCK_SIZE;
    }
}

/* ----------------------------------------------------------- */

int
main(int argc, char ** argv)
{
  process_options(argc, argv);

#if 0
  struct vector v1, v2;
  struct simple_matrix m1, m2;

  FILE *vector_file = fopen("test_vector", "r");
  if (read_vector(vector_file, &v1) == ET_CORRECT)
    print_vector(stdout, &v1);
  fclose(vector_file);
  DELETE(v1);

  FILE *matrix_file = fopen("test_matrix", "r");
  if (read_simple_matrix(matrix_file, &m1) == ET_CORRECT)
    print_simple_matrix(stdout, &m1);
  fclose(matrix_file);
  DELETE(m1);

  FILE *extended_file = fopen("test_extended", "r");
  if (read_extended_matrix(extended_file, &m2, &v2) == ET_CORRECT)
    print_extended_matrix(stdout, &m2, &v2);
  fclose(extended_file);
  DELETE(m2);
  DELETE(v2);
#endif

#if 0
  struct block_matrix bm;
  FILE *block_file = fopen("test_block", "r");
  bm.block_size = lconfig.block_size;
  if (read_block_matrix(block_file, &bm) == ET_CORRECT)
      print_block_matrix(stdout, &bm);

  block b;
  b.values = malloc(SQUARE_DOUB(lconfig.block_size));
  get_block(&bm, &b, 0, 0);
  print_simple_matrix(stdout, &b);
  DELETE(b);

  fclose(block_file);
  DELETE(bm);

  block b2;
  make_unit_block(&b2, 10);
  print_simple_matrix(stdout, &b2);
  DELETE(b2);
#endif

  block b3, b4;
  make_unit_block(&b3, 2);
  make_unit_block(&b4, 2);
  print_simple_matrix(stdout, &b3);
  print_simple_matrix(stdout, &b4);
  b3.values[0] = 2;
  b3.values[1] = 1;
  inverse_block(&b3, &b4);
  print_simple_matrix(stdout, &b3);
  print_simple_matrix(stdout, &b4);

  block b5;
  make_zero_block(&b5, 2);
  b3.values[0] = 2;
  b3.values[1] = 1;
  multiply_blocks(&b3, &b4, &b5);
  print_simple_matrix(stdout, &b3);
  print_simple_matrix(stdout, &b4);
  print_simple_matrix(stdout, &b5);

  return 0;
}

/* ----------------------------------------------------------- */
