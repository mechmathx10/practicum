#include "../datatypes.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DEFAULT_LIFETIME "10"
#define DEFAULT_CYCLE_LENGTH 10

/* ----------------------------------------------------------- */

struct daemon_config
{
  char *lifetime;
};


struct daemon_config dconfig = {
  .lifetime = NULL,
};


struct daemon_globals
{
  pid_t pid;
};


struct daemon_globals dglobals = {
  .pid = -1,
};


/* ----------------------------------------------------------- */

struct option options[] = {
  { .name = "lifetime",        .val = 'l',  .has_arg = required_argument },
  { .name = "help",            .val = 'h',  .has_arg = no_argument },
  { .name = NULL },
};


static
void
print_usage(FILE *output_stream, char *program_name)
{
  fprintf(output_stream,
          "Usage: %s [OPTIONS]\n"
          "  --lifetime,     -l [value]        daemon lifetime\n"
          "  --help,         -h                print this message.\n\n",
          program_name);
}


static
enum error_type
process_options(int argc, char **argv)
{
  int opt;
  while ((opt = getopt_long(argc, argv, "l:h", options, NULL)) != -1)
    {
      switch (opt)
        {
        case 'l':
          dconfig.lifetime = optarg;
          break;
        case 'h':
          print_usage(stdout, argv[0]);
          exit(0);
        default:
          print_usage(stderr, argv[0]);
          exit(1);
        }
    }

  if ((! dconfig.lifetime) || atoi(dconfig.lifetime) < 1)
    {
      fprintf(stderr, "Given daemon lifetime is non-positive, so it has been "
                      "set to default: %d\n", atoi(DEFAULT_LIFETIME));
      dconfig.lifetime = DEFAULT_LIFETIME;
    }

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

static
enum error_type
start_daemon(void)
{
  const char  *daemon = "./daemon";
  char * const daemon_args[] = { "./daemon", dconfig.lifetime, NULL };
  pid_t result = 0;
  int status;

  printf("Daemon caller started\n");
  while (1)
    {
      dglobals.pid = fork();
      if (dglobals.pid == -1)
        {
          fprintf(stderr, "Cannot fork!\nError: %s\n", strerror(errno));
          return ET_FORK_ERROR;
        }
      else if (dglobals.pid == 0)
        {
          execv(daemon, daemon_args);
          fprintf(stderr, "Cannot exec!\nError: %s\n", strerror(errno));
          return ET_EXEC_ERROR;
        }

      while ((result = waitpid(dglobals.pid, &status, 0)) <= 0
             && errno == EINTR);

      if (result != dglobals.pid)
        {
          fprintf(stderr, "Cannot waitpid child!\nError: %s\n",
                          strerror(errno));
          return ET_ERROR;
        }

      if (WIFEXITED(status))
        printf("Child process %d exited with code %d\n",
               dglobals.pid, WEXITSTATUS(status));
      else if (WIFSIGNALED(status))
        printf("Child process %d killed by signal %d\n",
               dglobals.pid, WTERMSIG(status));
      else
        {
          fprintf(stderr, "Child process %d has been terminated SUDDENLY.\n",
                          dglobals.pid);
          return ET_ERROR;
        }
    }

  printf("Daemon caller finished\n");
  return ET_CORRECT;
}


void
signal_handler(int signum)
{
  int kill_result;
  switch (signum)
    {
    case SIGKILL:
    case SIGTERM:
      printf("Terminating process %d\n", dglobals.pid);
      kill_result = kill(dglobals.pid, SIGKILL);
      if (kill_result != 0)
        fprintf(stderr, "Error terminating process %d: %s\n",
                        dglobals.pid, strerror(errno));
      printf("Daemon finished\n");
      exit(ET_CORRECT);
    default:
      break;
    }
}

/* ----------------------------------------------------------- */

int
main(int argc, char **argv)
{
  process_options(argc, argv);

  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGHUP,  SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGTERM, signal_handler);

  pid_t pid;

  pid = fork();
  if (pid == -1)
    {
      fprintf(stderr, "Cannot fork!\nError: %s\n", strerror(errno));
      return ET_FORK_ERROR;
    }
  else if (pid != 0)
    return ET_CORRECT;

  pid = fork();
  if (pid == -1)
    {
      fprintf(stderr, "Cannot fork!\nError: %s\n", strerror(errno));
      return ET_FORK_ERROR;
    }
  else if (pid != 0)
    return ET_CORRECT;

  return start_daemon();
}

