#ifndef DATATYPES_H
#define DATATYPES_H

#define UNUSED(ARG) (void) (ARG)

/* ----------------------------------------------------------- */

enum error_type
{
  ET_CORRECT = 0,
  ET_ERROR = 1,
  ET_INPUT_ERROR = 2,
  ET_ARG_ERROR = 3,
  ET_FILE_ERROR = 4,
};

enum input_type
{
  IT_FILE = 0,
  IT_CONSOLE = 1,
};


enum output_type
{
  OT_FILE = 0,
  OT_CONSOLE = 1,
};

enum state
{
  DEAD = 0,
  ALIVE = 1,
};

struct field
{
  int **old_values;
  int **new_values;
  int height;
  int width;
  int alive_count;
};

/* ----------------------------------------------------------- */

#endif // DATATYPES_H
