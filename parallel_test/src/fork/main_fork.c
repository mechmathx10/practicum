#include "../datatypes.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DEFAULT_CHILDREN_COUNT 2
#define DEFAULT_MESSAGES_COUNT 10

/* ----------------------------------------------------------- */

struct fork_config
{
  int children_count;
  int messages_count;
};


static struct fork_config fconfig = {
  .children_count = DEFAULT_CHILDREN_COUNT,
  .messages_count = DEFAULT_MESSAGES_COUNT,
};

/* ----------------------------------------------------------- */

struct option options[] = {
  { .name = "children-count",  .val = 'c',  .has_arg = required_argument },
  { .name = "messages-count",  .val = 'm',  .has_arg = required_argument },
  { .name = "help",            .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *output_stream, char *program_name)
{
  fprintf(output_stream,
          "Usage: %s [OPTIONS]\n"
          "  --children-count, -p [value]      daemon children count\n"
          "  --messages-count, -m [value]      #messages for each child\n"
          "  --help,         -h                print this message.\n\n",
          program_name);
}


static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "c:m:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'c':
          fconfig.children_count = atoi(optarg);
          break;
        case 'm':
          fconfig.messages_count = atoi(optarg);
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }

  if (fconfig.children_count < 1)
    {
      fprintf(stderr, "Given children count is non-positive, so it has been "
                      "set to default: %d\n", DEFAULT_CHILDREN_COUNT);
      fconfig.children_count = DEFAULT_CHILDREN_COUNT;
    }

  if (fconfig.messages_count < 1)
    {
      fprintf(stderr, "Given messages count is non-positive, so it has been "
                      "set to default: %d\n", DEFAULT_MESSAGES_COUNT);
      fconfig.messages_count = DEFAULT_MESSAGES_COUNT;
    }

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

void
child_job(pid_t process_id, const int messages_count)
{
  srand(process_id);
  for (int i = 0; i < messages_count; ++i)
    {
      usleep(rand() % 100000);
      printf("Process %d: message %d\n", process_id, i);
    }
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  process_options(argc, argv);

  pid_t parent_id = getpid();
  pid_t process_id;
  const int children_count = fconfig.children_count;

  printf("Process %d initialized (parent)\n", parent_id);

  for (int i = 0; i < children_count; ++i)
    {
      process_id = fork();
      if (process_id == 0)
        {
          process_id = getpid();
          child_job(process_id, fconfig.messages_count);
          break;
        }
      else
        printf("Process %d initialized\n", process_id);
    }

  process_id = getpid();
  if (process_id == parent_id)
    {
      while (errno != ECHILD)
        waitpid(-1, NULL, 0);
      printf("Process %d terminated (parent)\n", parent_id);
    }
  else
    printf("Process %d terminated\n", process_id);

  return ET_CORRECT;
}
