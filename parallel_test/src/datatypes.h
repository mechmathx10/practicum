#ifndef DATATYPES_H
#define DATATYPES_H

#define UNUSED(PARAM) (void) (PARAM)

/* ----------------------------------------------------------- */

enum error_type
{
  ET_CORRECT = 0,
  ET_ERROR = 1,
  ET_INPUT_ERROR = 2,
  ET_ARG_ERROR = 3,
  ET_FILE_ERROR = 4,
  ET_FORK_ERROR = 5,
  ET_EXEC_ERROR = 6,
  ET_SHM_ERROR = 7,
  ET_SEM_ERROR = 8,
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

/* ----------------------------------------------------------- */

#endif
