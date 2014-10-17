#include "input.h"
#include "output.h"
#include "block_utils.h"
#include "matrix_utils.h"
#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define DEFAULT_BLOCK_SIZE 3

/* ----------------------------------------------------------- */

/* mi - matrix inverter */
struct mi_config
{
  char *input_filename;
  enum stream_type input_stream_type;
  char *output_filename;
  enum stream_type output_stream_type;
  int block_size;
};

static struct mi_config mconfig = {
  .input_filename = (char *) NULL,
  .input_stream_type = ST_CONSOLE,
  .output_filename = (char *) NULL,
  .output_stream_type = ST_CONSOLE,
  .block_size = 0,
};

struct option options[] = {
  { .name = "input_file",    .val = 'i',  .has_arg = required_argument },
  { .name = "output_file",   .val = 'o',  .has_arg = required_argument },
  { .name = "block_size",    .val = 'b',  .has_arg = required_argument },
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
          "  --block_size,  -b [size]         matrix block size.\n"
          "  --help,        -h                print this message\n",
          program_name);
}

/* ----------------------------------------------------------- */

static
void
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:b:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'i':
          mconfig.input_filename = optarg;
          mconfig.input_stream_type = ST_FILE;
          break;
        case 'o':
          mconfig.output_filename = optarg;
          mconfig.output_stream_type = ST_FILE;
          break;
        case 'b':
          mconfig.block_size = atoi(optarg);
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
      fprintf(stderr, "Block size is not set or negative and been set to: %d\n",
                      DEFAULT_BLOCK_SIZE);
      mconfig.block_size = DEFAULT_BLOCK_SIZE;
    }
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  process_options(argc, argv);

  FILE *input_stream = NULL;
  FILE *output_stream = NULL;
  if (mconfig.input_stream_type == ST_FILE)
    {
      input_stream = fopen(mconfig.input_filename, "r");
      if (! input_stream)
        {
          fprintf(stderr, "Cannot open file %s for input\n",
                  mconfig.input_filename);
          return ET_FILE_ERROR;
        }
    }
  else // mconfig.input_stream_type == ST_CONSOLE
    input_stream = stdin;

  if (mconfig.output_stream_type == ST_FILE)
    {
      output_stream = fopen(mconfig.output_filename, "w");
      if (! output_stream)
        {
          fprintf(stderr, "Cannot open file %s for output\n",
                  mconfig.output_filename);
          if (mconfig.input_stream_type == ST_FILE)
            fclose(input_stream);
        }
    }
  else // mconfig.output_stream_type == ST_CONSOLE
    output_stream = stdout;



  if (mconfig.input_stream_type == ST_FILE)
    fclose(input_stream);
  if (mconfig.output_stream_type == ST_FILE)
    fclose(output_stream);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
