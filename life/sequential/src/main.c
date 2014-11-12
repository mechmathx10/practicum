#include "../../headers/datatypes.h"
#include "life_sequential.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_SLEEP_INTERVAL 1000000

/* ----------------------------------------------------------- */

struct life_config
{
  char *input_filename;
  enum input_type input_stream_type;
  char *output_filename;
  enum output_type output_stream_type;
  int sleep_interval; // usec
  int max_turns;
};


static struct life_config lconfig = {
  .input_filename = (char *) NULL,
  .input_stream_type = IT_CONSOLE,
  .output_filename = (char *) NULL,
  .output_stream_type = OT_CONSOLE,
  .sleep_interval = DEFAULT_SLEEP_INTERVAL,
  .max_turns = -1,
};

/* ----------------------------------------------------------- */

struct option options[] = {
  { .name = "input_file",     .val = 'i',  .has_arg = required_argument },
  { .name = "output_file",    .val = 'o',  .has_arg = required_argument },
  { .name = "sleep-interval", .val = 's',  .has_arg = required_argument },
  { .name = "max-turns",      .val = 'm',  .has_arg = required_argument },
  { .name = "help",           .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *output_stream, char *program_name)
{
  fprintf(output_stream,
          "Usage: %s [OPTIONS]\n"
          "  --input_file,    -i [filename]     Input file.\n"
          "  --output-file,   -o [filename]     Output file.\n"
          "  --sleep-interval,-s [value]        Field update interval in micro"
          "seconds. Default is %d.\n"
          "  --help,          -h                Print this message.\n\n",
          program_name, DEFAULT_SLEEP_INTERVAL);
}

/* ----------------------------------------------------------- */

static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:s:m:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'i':
          lconfig.input_filename = optarg;
          lconfig.input_stream_type = IT_FILE;
          break;
        case 'o':
          lconfig.output_filename = optarg;
          lconfig.output_stream_type = OT_FILE;
          break;
        case 's':
          lconfig.sleep_interval = atoi(optarg);
          break;
        case 'm':
          lconfig.max_turns = atoi(optarg);
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(ET_CORRECT);
        default:
          print_usage(stderr, argv[0]);
          exit(ET_ARG_ERROR);
        }
    }

  if (lconfig.sleep_interval < 1)
    {
      fprintf(stderr, "Given interval is non-positive, set to default: %d\n",
                      DEFAULT_SLEEP_INTERVAL);
      lconfig.sleep_interval = DEFAULT_SLEEP_INTERVAL;
    }
  if (lconfig.max_turns < 0)
    fprintf(stderr, "Negative max_turns parameter has no effect\n");
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
open_streams(FILE **input_stream, FILE **output_stream)
{
  if (lconfig.input_stream_type == IT_FILE)
    {
      *input_stream = fopen(lconfig.input_filename, "r");
      if (! *input_stream)
        {
          fprintf(stderr, "Cannot open file %s for input.",
                          lconfig.input_filename);

          return ET_FILE_ERROR;
        }
    }
  else // lconfig.input_stream_type == IT_CONSOLE
    *input_stream = stdin;

  if (lconfig.output_stream_type == OT_FILE)
    {
      *output_stream = fopen(lconfig.output_filename, "w");
      if (! *output_stream)
        {
          fprintf(stderr, "Cannot open file %s for output.",
                          lconfig.output_filename);
          if (lconfig.input_stream_type == IT_FILE)
            fclose(*input_stream);
          return ET_FILE_ERROR;
        }
    }
  else // lconfig.output_stream_type == OT_CONSOLE
    *output_stream = stdout;

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  process_options(argc, argv);

  FILE *input_stream = NULL;
  FILE *output_stream = NULL;
  enum error_type current_state = open_streams(&input_stream, &output_stream);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Error opening streams\n");
      return ET_FILE_ERROR;
    }

  struct field life_field;
  current_state = init_field(input_stream, &life_field,
                             lconfig.input_stream_type);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Cannot initialize field!\n");
      if (lconfig.input_stream_type == IT_FILE)
        fclose(input_stream);
      if (lconfig.output_stream_type == OT_FILE)
        fclose(output_stream);
      return ET_INPUT_ERROR;
    }
  if (lconfig.input_stream_type == IT_FILE)
    fclose(input_stream);

  int turn_counter = 0;
  print_field(output_stream, &life_field);
  while (life_field.alive_count > 0)
    {
      if (lconfig.max_turns > 0)
        if (turn_counter >= lconfig.max_turns)
          break;
      update_field(&life_field);
      if (lconfig.output_stream_type == OT_CONSOLE)
        system("clear");
      fprintf(output_stream, "Turn: %d\nCells alive:%d\n\n",
                              turn_counter, life_field.alive_count);
      print_field(output_stream, &life_field);
      usleep(lconfig.sleep_interval);
      ++turn_counter;
    }
  fprintf(output_stream, "Finished.\nTurns alive: %d\nCells left: %d\n",
                          turn_counter, life_field.alive_count);

  delete_field(&life_field);
  if (lconfig.output_stream_type == OT_FILE)
    fclose(output_stream);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
