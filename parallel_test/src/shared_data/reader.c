#include "shared_data.h"

/* ----------------------------------------------------------- */

void
reader(shared_data *data)
{
  enum error_type current_state;
  while (1)
    {
      current_state = shared_data_lock(data);
      if (current_state == ET_CORRECT)
        {
          if (data->message_len == 0)
            {
              if (! fgets(data->message_buf, BUF_LEN, stdin))
                break;
              data->message_len = strlen(data->message_buf) + 1;
            }
          shared_data_unlock(data);
        }
      sleep(1);
    }
  data->done = 1;
  printf("Reader process %d exits.\n", getpid());
  shared_data_unlock(data);
}

/* ----------------------------------------------------------- */

int
main(void)
{
  shared_data *data = shared_data_open();
  if (data)
    {
      reader(data);
      shared_data_close(data);
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
