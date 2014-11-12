#include "shared_data.h"

/* ----------------------------------------------------------- */

void
writer(shared_data *data)
{
  enum error_type current_state;
  while (1)
    {
      current_state = shared_data_lock(data);
      if (data->done)
        printf("Writer process %d exits.\n", getpid());
      if (current_state == ET_CORRECT)
        shared_data_unlock(data);
      return;
    }
  if (current_state == ET_CORRECT)
    {
      if (data->message_len)
        {
          printf("-> %s\n", data->message_buf);
          data->message_len = 0;
        }
      shared_data_unlock(data);
    }
  sleep(1);
}

/* ----------------------------------------------------------- */

int
main(void)
{
  shared_data *data = shared_data_open();
  if (data)
    {
      writer(data);
      shared_data_close(data);
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
