#ifndef DATATYPES_H
#define DATATYPES_H


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
  double *blocks;
  int size;
  int block_size;
};


enum error_types
{
  ER_CORRECT = 0,
  ER_ERROR = 1,
};


#endif // DATATYPES_H
