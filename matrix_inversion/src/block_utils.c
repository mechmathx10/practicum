#include "block_utils.h"

/* ----------------------------------------------------------- */


inline
double *
get_block_start(struct block_matrix *matrix, int i, int j)
{
  return &matrix->values[(i * matrix->size * matrix->block_size) +
                         (j * matrix->block_size *
                          (i == matrix->size / matrix->block_size
                            ? matrix->size % matrix->block_size
                            : matrix->block_size))];
}


inline
int
get_block_height(struct block_matrix *matrix, int i, int j)
{
  UNUSED(j);
  return i == matrix->size / matrix->block_size 
                ? matrix->size % matrix->block_size
                : matrix->block_size;
}


inline
int
get_block_width(struct block_matrix *matrix, int i, int j)
{
  UNUSED(i);
  return j == matrix->size / matrix->block_size 
                ? matrix->size % matrix->block_size
                : matrix->block_size;
}


block get_block(struct block_matrix *matrix, int i, int j)
{
  block result;
  UNUSED(matrix);
  UNUSED(i);
  UNUSED(j);
  return result;
}


/* ----------------------------------------------------------- */

