#include "solver.h"
#include "datatypes.h"
#include "input.h"
#include "output.h"
#include "matrix_utils.h"

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#define EPS 1e-5

/* ----------------------------------------------------------- */

struct mrm_config
{
  char *input_filename;
  enum input_type input_stream_type;
  char *output_filename;
  enum output_type output_stream_type;
  double precision;
};


static struct mrm_config mrm_config = {
  .input_filename = (char *) NULL,
  .input_stream_type = IT_CONSOLE,
  .output_filename = (char *) NULL,
  .output_stream_type = OT_CONSOLE,
  .precision = EPS,
};

/* ----------------------------------------------------------- */

struct option options[] = {
  { .name = "input_file",    .val = 'i',  .has_arg = required_argument },
  { .name = "output_file",   .val = 'o',  .has_arg = required_argument },
  { .name = "precision",     .val = 'p',  .has_arg = required_argument },
  { .name = "help",          .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *output_stream, char *program_name)
{
  fprintf(output_stream,
          "Usage: %s [OPTIONS]\n"
          "  --input_file,  -i [filename]     input file.\n"
          "  --output-file, -o [filename]     output file.\n"
          "  --precision,   -p [value]        precision. (default is %f)\n"
          "  --help,        -h                print this message.\n\n",
          program_name, EPS);
}


static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:p:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'i':
          mrm_config.input_filename = optarg;
          mrm_config.input_stream_type = IT_FILE;
          break;
        case 'o':
          mrm_config.output_filename = optarg;
          mrm_config.output_stream_type = OT_FILE;
          break;
        case 'p':
          mrm_config.precision = atof(optarg);
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }
  if (mrm_config.input_stream_type == IT_CONSOLE &&
      mrm_config.output_stream_type == OT_CONSOLE)
    {
      printf("Neither input nor output file specified, might be an error.\n");
      print_usage(stdout, argv[0]);
    }
  if (mrm_config.precision < 0)
    {
      fprintf(stderr, "Given precision is negative, so precision has been set "
                      "to default: %f\n", EPS);
      mrm_config.precision = EPS;
    }

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  int current_state = ET_CORRECT;
  process_options(argc, argv);

  FILE *input_stream = NULL;
  FILE *output_stream = NULL;
  if (mrm_config.input_stream_type == IT_FILE)
    {
      input_stream = fopen(mrm_config.input_filename, "r");
      if (! input_stream)
        {
          fprintf(stderr, "Cannot open file %s for input\n",
                  mrm_config.input_filename);
          return ET_FILE_ERROR;
        }
    }
  else // mrm_config.input_stream_type == IT_CONSOLE
    input_stream = stdin;
  if (mrm_config.output_stream_type == OT_FILE)
    {
      output_stream = fopen(mrm_config.output_filename, "w");
      if (! output_stream)
        {
          fprintf(stderr, "Cannot open file %s for output\n",
                  mrm_config.output_filename);
          if (mrm_config.input_stream_type == IT_FILE)
            fclose(input_stream);
          return ET_FILE_ERROR;
        }
    }
  else // mrm_config.output_stream_type == OT_CONSOLE
    output_stream = stdout;

  struct simple_matrix coefficients;
  struct vector free_terms;
  struct vector result;

  enum input_type in_type = mrm_config.input_stream_type;
  current_state = read_extended_matrix(input_stream, &coefficients,
                                       &free_terms, in_type);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Error reading matrix\n");
      if (mrm_config.output_stream_type == OT_FILE)
        fclose(output_stream);
      if (mrm_config.input_stream_type == IT_FILE)
        fclose(input_stream);
      return ET_INPUT_ERROR;
    }
  if (mrm_config.input_stream_type == IT_FILE)
    fclose(input_stream);

  const int size = coefficients.height;
  init_vector(&result, size);

  // TODO : solve system
  print_extended_matrix_m(stdout, &coefficients, &free_terms,
                          "This is our extended matrix");
  print_vector(stdout, &free_terms);


  DELETE(coefficients);
  DELETE(free_terms);
  DELETE(result);
  if (mrm_config.output_stream_type == OT_FILE)
    fclose(output_stream);
  return ET_CORRECT;
}
