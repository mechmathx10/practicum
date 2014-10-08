#ifndef DATATYPES_H
#define DATATYPES_H

#define UNUSED(PARAM) (void) (PARAM);

#define DIV_UP(A, B) (A / B) + ((A) % (B) == 0 ? 0 : 1)

#define DELETE(X) free((X).values)

/* ----------------------------------------------------------- */

enum error_type
{
  ET_CORRECT = 0,
  ET_ERROR = 1,
  ET_INPUT_ERROR = 2,
  ET_ARG_ERROR = 3,
};


struct vector
{
  double *values;
  int size;
};


struct simple_matrix
{
  double *values;
  int height;
  int width;
};


struct block_matrix
{
  double *values;
  int size;
  int block_size;
};


typedef struct simple_matrix block;

/* ----------------------------------------------------------- */

#endif // DATATYPES_H
