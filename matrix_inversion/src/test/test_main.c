#include "test_solver.h"
#include "../datatypes.h"
#include "../input.h"
#include "../output.h"
#include "../matrix_utils.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>

/* ----------------------------------------------------------- */

struct test_config
{
  char *input_filename;
  enum input_type input_stream_type;
  char *output_filename;
  enum output_type output_stream_type;
};

static struct test_config tconfig = {
  .input_filename = (char *) NULL,
  .input_stream_type = IT_CONSOLE,
  .output_filename = (char *) NULL,
  .output_stream_type = OT_CONSOLE,
};

/* ----------------------------------------------------------- */

struct option options[] = {
  { .name = "input_file",    .val = 'i',  .has_arg = required_argument },
  { .name = "output_file",   .val = 'o',  .has_arg = required_argument },
  { .name = "help",          .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *output_stream, char *program_name)
{
  fprintf(output_stream,
          "Usage: %s [OPTIONS]\n\n"
          "  --input_file,  -i [filename]     input file.\n"
          "  --output-file, -o [filename]     output file.\n"
          "  --help,        -h                print this message\n",
          program_name);
}


static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'i':
          tconfig.input_filename = optarg;
          tconfig.input_stream_type = IT_FILE;
          break;
        case 'o':
          tconfig.output_filename = optarg;
          tconfig.output_stream_type = OT_FILE;
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  int current_state = ET_CORRECT;
  printf("This is a test main function\n");
  process_options(argc, argv);

  FILE *input_stream = NULL;
  FILE *output_stream = NULL;
  if (tconfig.input_stream_type == IT_FILE)
    {
      input_stream = fopen(tconfig.input_filename, "r");
      if (! input_stream)
        {
          fprintf(stderr, "Cannot open file %s for input\n",
                  tconfig.input_filename);
          return ET_FILE_ERROR;
        }
    }
  else // tconfig.input_stream_type == IT_CONSOLE
    input_stream = stdin;
  if (tconfig.output_stream_type == OT_FILE)
    {
      output_stream = fopen(tconfig.output_filename, "w");
      if (! output_stream)
        {
          fprintf(stderr, "Cannot open file %s for output\n",
                  tconfig.output_filename);
          if (tconfig.input_stream_type == IT_FILE)
            fclose(input_stream);
          return ET_FILE_ERROR;
        }
    }
  else // tconfig.output_stream_type == OT_CONSOLE
    output_stream = stdout;

  struct simple_matrix matrix;
  struct simple_matrix result;

  enum input_type in_type = tconfig.input_stream_type;
  current_state = read_square_matrix(input_stream, &matrix, in_type);
  if (tconfig.input_stream_type == IT_FILE)
    fclose(input_stream);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Error reading matrix");
      if (tconfig.output_stream_type == OT_FILE)
        fclose(output_stream);
      return ET_INPUT_ERROR;
    }
  make_unit_block(&result, matrix.height);

  time_t t = clock();
  inverse_matrix(&matrix, &result);
  t = clock() - t;
  printf("Inversion time: %.3fs\n", (double)t / CLOCKS_PER_SEC);
  print_simple_matrix(output_stream, &result);

  DELETE(matrix);
  DELETE(result);
  if (tconfig.output_stream_type == OT_FILE)
    fclose(output_stream);
  return ET_CORRECT;
}
