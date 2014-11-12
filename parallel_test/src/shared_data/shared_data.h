#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "../datatypes.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define BUF_LEN 256

/* ----------------------------------------------------------- */

typedef struct _shared_data_
{
  int semaphore_id;
  int done;
  int message_len;
  char message_buf[BUF_LEN];
} shared_data;


shared_data *
shared_data_open(void);


enum error_type
shared_data_close(shared_data *);


enum error_type
shared_data_lock(shared_data *);


enum error_type
shared_data_unlock(shared_data *);

/* ----------------------------------------------------------- */

#endif
