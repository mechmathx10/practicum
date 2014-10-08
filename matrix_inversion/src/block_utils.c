#include "block_utils.h"

/* ----------------------------------------------------------- */

inline
double *
get_block_start(const struct block_matrix *matrix, int i, int j)
{
  return &matrix->values[(i * matrix->size * matrix->block_size) +
                         (j * matrix->block_size *
                          (i == matrix->size / matrix->block_size
                            ? matrix->size % matrix->block_size
                            : matrix->block_size))];
}

/* ----------------------------------------------------------- */

inline
int
get_block_height(const struct block_matrix *matrix, int i, int j)
{
  UNUSED(j);
  return i == matrix->size / matrix->block_size 
                ? matrix->size % matrix->block_size
                : matrix->block_size;
}

/* ----------------------------------------------------------- */

inline
int
get_block_width(const struct block_matrix *matrix, int i, int j)
{
  UNUSED(i);
  return j == matrix->size / matrix->block_size 
                ? matrix->size % matrix->block_size
                : matrix->block_size;
}

/* ----------------------------------------------------------- */

block get_block(const struct block_matrix *matrix, int i, int j)
{
  block result;
  UNUSED(matrix);
  UNUSED(i);
  UNUSED(j);
  return result;
}

/* ----------------------------------------------------------- */

void
put_block(struct block_matrix *matrix, block *buffer, int i, int j)
{
  UNUSED(matrix);
  UNUSED(buffer);
  UNUSED(i);
  UNUSED(j);
}

/* ----------------------------------------------------------- */

