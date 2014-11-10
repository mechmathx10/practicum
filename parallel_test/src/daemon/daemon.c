#include "../datatypes.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_LIFETIME 10

#define LOUD 1

/* ----------------------------------------------------------- */

static
void
daemon_job(pid_t pid)
{
#if LOUD
  printf("Daemon #%d: MUAHAHAHAHA!!!\n", pid);
#else
  UNUSED(pid);
#endif
  sleep(1);
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  int lifetime = -1;
  if (argc != 2)
    {
      fprintf(stderr, "Cannot start daemon, wrong arguments. Correct:\n"
                      "%s [lifetime]\n", argv[0]);
      return ET_ARG_ERROR;
    }
  lifetime = atoi(argv[1]);
  if (lifetime < 1)
    {
      printf("Given lifetime is non-positive. Lifetime is set to default: %d\n",
             DEFAULT_LIFETIME);
      lifetime= DEFAULT_LIFETIME;
    }

  int pid = getpid();
  printf("Daemon process started. Pid: %d\n", pid);

  for (int i = 0; i < lifetime; ++i)
    daemon_job(pid);

  printf("Daemon process finished. Pid: %d\n", pid);
  return ET_CORRECT;
}
