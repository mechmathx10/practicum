#include "shared_data.h"

#include <errno.h>

#define SHARED_MEMORY_KEY 1221
#define SEMAPHORE_KEY     1222

/* ----------------------------------------------------------- */

static int shared_memory_id;
static int semaphore_id;

/* ----------------------------------------------------------- */

static
void *
shared_memory_open(const int key, const int size, int * const created)
{
  void *result;
  *created = 0;

  shared_memory_id = shmget(key, size, 0);
  if (shared_memory_id == -1)
    {
      if (errno == ENOENT)
        {
          shared_memory_id = shmget(key, size, IPC_CREAT | 0664);
          if (shared_memory_id == -1)
            {
              fprintf(stderr, "Cannot create shared memory!\n");
              return NULL;
            }
          *created = 1;
        }
      else
        {
          fprintf(stderr, "Cannot get shared memory!\n");
          return NULL;
        }
    }

  result = (void *) shmat(shared_memory_id, 0, 0);
  if (result == (void *) -1)
    {
      fprintf(stderr, "Cannot attach shared memory!\n");
      return NULL;
    }

  return result;
}

/* ----------------------------------------------------------- */

static
enum error_type
shared_memory_close(void *memory)
{
  int detach_result = shmdt(memory);
  if (detach_result == -1)
    {
      fprintf(stderr, "Cannot detach shared memory!\n");
      return ET_SHM_ERROR;
    }
  shmctl(shared_memory_id, IPC_RMID, 0);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

static
int
semaphore_open(const int key, int * const created)
{
  int result;
  *created = 0;

  result = semget(key, 1, 0);
  if (result == -1)
    {
      if (errno == ENOENT)
        {
          result = semget(key, 1, IPC_CREAT | 664);
          if (result == -1)
            {
              fprintf(stderr, "Cannot create semaphore!\n");
              return -1;
            }
          *created = 1;
        }
      else
        {
          fprintf(stderr, "Cannot get semaphore!\n");
          return -1;
        }
    }

  if (*created)
    {
#ifdef _SEM_SEMUN_UNDEFINED
     union semun
     {
       int val;
       struct semid_ds *buf;
       unsigned short int *array;
       struct seminfo *__buf;
     };
#endif
    union semun semaphore_union;
    semaphore_union.val = 1;
    int semctl_result = semctl(result, 0, SETVAL, semaphore_union);
    if (semctl_result == -1)
      {
        fprintf(stderr, "Cannot initialize semaphore!\n");
        return -1;
      }
    }
  return result;
}

/* ----------------------------------------------------------- */

static
enum error_type
semaphore_close(const int id)
{
  semctl(id, 0, IPC_RMID, 0);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

static
enum error_type
semaphore_lock(const int id)
{
  struct sembuf semaphore_buf = {
    .sem_num = 0,
    .sem_op = -1,
    .sem_flg = 0,
  };

  int semop_result = semop(id, &semaphore_buf, 1);
  if (semop_result == -1)
    {
      if (errno == EIDRM)
        return ET_SEM_ERROR;

      fprintf(stderr, "Cannot lock semaphore!\n");
      return ET_SEM_ERROR;
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

static
enum error_type
semaphore_unlock(const int id)
{
  struct sembuf semaphore_buf = {
    .sem_num = 0,
    .sem_op =  1,
    .sem_flg = 0,
  };

  int semop_result = semop(id, &semaphore_buf, 1);
  if (semop_result == -1)
    {
      fprintf(stderr, "Cannot unlock semaphore!\n");
      return ET_SEM_ERROR;
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

shared_data *
shared_data_open(void)
{
  shared_data *result;
  int shared_memory_created;
  int semaphore_created;

  result = (shared_data *) shared_memory_open(SHARED_MEMORY_KEY,
                                              sizeof(shared_data),
                                              &shared_memory_created);
  if (! result)
    return NULL;

  semaphore_id = semaphore_open(SEMAPHORE_KEY, &semaphore_created);
  if (semaphore_id == -1)
    {
      shared_memory_close(result);
      return NULL;
    }

  enum error_type current_state = semaphore_lock(semaphore_id);
  if (current_state != ET_CORRECT)
    {
      shared_data_close(result);
      return NULL;
    }

  if (shared_memory_created)
    {
      memset((char *) result, 0, sizeof(shared_data));
      result->semaphore_id = semaphore_id;
    }

  current_state = semaphore_unlock(semaphore_id);
  if (current_state != ET_CORRECT)
    {
      shared_data_close(result);
      return NULL;
    }
  return result;
}

/* ----------------------------------------------------------- */

enum error_type
shared_data_close(shared_data *data)
{
  enum error_type current_state = semaphore_close(semaphore_id);
  if (current_state != ET_CORRECT)
    return ET_SHM_ERROR;
  current_state = shared_memory_close(data);
  if (current_state != ET_CORRECT)
    return ET_SHM_ERROR;
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
shared_data_lock(shared_data *data)
{
  int id = data->semaphore_id;
  return semaphore_lock(id);
}

/* ----------------------------------------------------------- */

enum error_type
shared_data_unlock(shared_data *data)
{
  int id = data->semaphore_id;
  return semaphore_unlock(id);
}

/* ----------------------------------------------------------- */
