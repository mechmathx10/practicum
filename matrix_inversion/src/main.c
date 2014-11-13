#include "io/input.h"
#include "io/output.h"
#include "utils/block_utils.h"
#include "utils/matrix_utils.h"
#include "solver/solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

#define _DEBUG_ 0

#define DEFAULT_BLOCK_SIZE  3
#define DEFAULT_MATRIX_SIZE 10

#define CLOSE_FILES 1


/* ----------------------------------------------------------- */

/* mi - matrix inverter */
struct mi_config
{
  char *input_filename;
  enum input_type input_stream_type;
  char *output_filename;
  enum output_type output_stream_type;
  int block_size;
  int generated;
  int matrix_size;
  int matrix_size_given;
};


static struct mi_config mconfig = {
  .input_filename = (char *) NULL,
  .input_stream_type = IT_CONSOLE,
  .output_filename = (char *) NULL,
  .output_stream_type = OT_CONSOLE,
  .block_size = 0,
  .generated = 0,
  .matrix_size = DEFAULT_MATRIX_SIZE,
  .matrix_size_given = 0,
};


struct option options[] = {
  { .name = "input_file",    .val = 'i',  .has_arg = required_argument },
  { .name = "output_file",   .val = 'o',  .has_arg = required_argument },
  { .name = "block_size",    .val = 'b',  .has_arg = required_argument },
  { .name = "generate",      .val = 'g',  .has_arg = no_argument },
  { .name = "size",          .val = 's',  .has_arg = required_argument },
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
          "  --block_size,  -b [size]         matrix block size.\n"
          "  --generate     -g                generate matrix, not input\n"
          "                                   (only size must be given)\n"
          "  --size         -s [size]         matrix size, works with -g.\n"
          "  --help,        -h                print this message\n\n",
          program_name);
}

/* ----------------------------------------------------------- */

static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:b:gs:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'i':
          mconfig.input_filename = optarg;
          mconfig.input_stream_type = IT_FILE;
          break;
        case 'o':
          mconfig.output_filename = optarg;
          mconfig.output_stream_type = OT_FILE;
          break;
        case 'b':
          mconfig.block_size = atoi(optarg);
          break;
        case 'g':
          mconfig.generated = 1;
          break;
        case 's':
          mconfig.matrix_size = atoi(optarg);
          mconfig.matrix_size_given = 1;
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }
  if (mconfig.block_size < 1)
    {
      fprintf(stderr, "Block size is not given or has been given negative "
                      "and been set to default: %d\n",
                      DEFAULT_BLOCK_SIZE);
      mconfig.block_size = DEFAULT_BLOCK_SIZE;
    }
  if (mconfig.generated)
    {
      if (mconfig.matrix_size_given && mconfig.matrix_size < 1)
        {
          fprintf(stderr, "Non-positive matrix size!\n");
          return ET_ARG_ERROR;
        }
      if (mconfig.input_stream_type == IT_FILE)
        {
          fprintf(stderr, "-g option is active, so no input file is needed, "
                          "-i has no effect\n");
        }
      mconfig.input_stream_type = IT_GENERATE;
    }
  else
    {
      if (mconfig.matrix_size_given)
        fprintf(stderr, "Matrix size option has no effect without -g option\n");
    }
  if (mconfig.input_stream_type == IT_CONSOLE &&
      mconfig.output_stream_type == OT_CONSOLE)
    {
      printf("Neither input nor output file specified, might be an error.\n");
      print_usage(stdout, argv[0]);
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

double
func(int i, int j)
{
  return 1.0 + (double) (i > j ? i : j);
}

/* ----------------------------------------------------------- */

static
enum error_type
init_streams(FILE **input_stream, FILE **output_stream)
{
  if (mconfig.input_stream_type == IT_FILE)
    {
      *input_stream = fopen(mconfig.input_filename, "r");
      if (! *input_stream)
        {
          fprintf(stderr, "Cannot open file %s for input\n",
                          mconfig.input_filename);
          return ET_FILE_ERROR;
        }
    }
  else // mconfig.input_stream_type == IT_CONSOLE || IT_GENERATE
    *input_stream = stdin;
  if (mconfig.output_stream_type == OT_FILE)
    {
      *output_stream = fopen(mconfig.output_filename, "w");
      if (! *output_stream)
        {
          fprintf(stderr, "Cannot open file %s for output\n",
                          mconfig.output_filename);
          if (mconfig.input_stream_type == IT_FILE)
            fclose(*input_stream);
          return ET_FILE_ERROR;
        }
    }
  else // mconfig.output_stream_type == OT_CONSOLE
    *output_stream = stdout;
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  int current_state = ET_CORRECT;

  current_state = process_options(argc, argv);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Options processing failed.\n");
      return current_state;
    }

#define OPEN_FILE_SECTION
  FILE *input_stream = NULL;
  FILE *output_stream = NULL;
  current_state = init_streams(&input_stream, &output_stream);
  if (current_state != ET_CORRECT)
    return current_state;
#undef OPEN_FILE_SECTION

#define INPUT_SECTION
  struct block_matrix matrix;
  struct block_matrix result;
  struct block_matrix saved_matrix;
  matrix.block_size = mconfig.block_size;
  result.block_size = mconfig.block_size;
  enum input_type in_type = mconfig.input_stream_type;

  if (mconfig.input_stream_type == IT_GENERATE)
    {
      matrix.size = mconfig.matrix_size_given ? mconfig.matrix_size : -1;
      current_state = generate_block_matrix(&matrix, func);
      if (current_state != ET_CORRECT)
        {
          fprintf(stderr, "Error generating matrix.\n");
          if (mconfig.output_stream_type == OT_FILE)
            fclose(output_stream);
          return ET_ERROR;
        }
    }
  else
    {
      current_state = read_block_matrix(input_stream, &matrix, in_type);
      if (current_state != ET_CORRECT)
        {
          fprintf(stderr, "Error reading matrix.\n");
          if (mconfig.input_stream_type == IT_FILE)
            fclose(input_stream);
          if (mconfig.output_stream_type == OT_FILE)
            fclose(output_stream);
          return ET_INPUT_ERROR;
        }
    }
  if (mconfig.input_stream_type == IT_FILE)
    fclose(input_stream);

  make_unit_block_matrix(&result, matrix.size);
  make_block_matrix_copy(&matrix, &saved_matrix);
  print_block_matrix_full_m(output_stream, &saved_matrix, "Saved matrix");
#undef INPUT_SECTION

#define INVERSE_SECTION
  time_t t = clock();
  current_state = inverse_block_matrix(&matrix, &result);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Error inversing matrix.\n");
      DELETE(matrix);
      DELETE(result);
      DELETE(saved_matrix);
      if (mconfig.output_stream_type == OT_FILE)
        fclose(output_stream);
      return current_state;
    }
  t = clock() - t;
  printf("Inversion time: %.3fs\n", (double)t / CLOCKS_PER_SEC);
#undef INVERSE_SECTION

#define OUTPUT_SECTION

#undef OUTPUT_SECTION

#define RESOURSE_FREE_SECTION
  DELETE(matrix);
  DELETE(result);
  DELETE(saved_matrix);
  if (mconfig.output_stream_type == OT_FILE)
    fclose(output_stream);
#undef RESOURSE_FREE_SECTION
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
