#include "../datatypes.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#define DEFAULT_THREAD_COUNT 2

/* ----------------------------------------------------------- */

struct thread_config
{
  int thread_count;
};


static struct thread_config tconfig = {
  .thread_count = DEFAULT_THREAD_COUNT,
};

/* ----------------------------------------------------------- */

struct option options[] = {
  { .name = "thread-count",  .val = 't',  .has_arg = required_argument },
  { .name = "help",          .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *output_stream, char *program_name)
{
  fprintf(output_stream,
          "Usage: %s [OPTIONS]\n"
          "  --thread-count, -p [value]        execution thread count\n"
          "  --help,         -h                print this message.\n\n",
          program_name);
}


static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "t:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 't':
          tconfig.thread_count = atoi(optarg);
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }

  if (tconfig.thread_count < 1)
    {
      fprintf(stderr, "Given thread count is non-positive, so it has been set "
                      "to default: %d\n", DEFAULT_THREAD_COUNT);
      tconfig.thread_count = DEFAULT_THREAD_COUNT;
    }

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

void *
thread_job(void *arg)
{
  UNUSED(arg);
  printf("Hello! I am just another thread.\n");
  return (void *) NULL;
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  process_options(argc, argv);

  const int thread_count = tconfig.thread_count;
  pthread_t *thread_ids = (pthread_t *) malloc(thread_count
                                               * sizeof(pthread_t));
  for (int i = 0; i < thread_count; ++i)
    pthread_create(&thread_ids[i], NULL, thread_job, NULL);

  for (int i = 0; i < thread_count; ++i)
    pthread_join(thread_ids[i], NULL);

  return ET_CORRECT;
}
