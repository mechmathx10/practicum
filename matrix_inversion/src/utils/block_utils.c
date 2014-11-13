#include "block_utils.h"

#include <string.h>

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

void
get_block(const struct block_matrix *matrix, block *buffer, int i, int j)
{
  const int height = get_block_height(matrix, i, j);
  const int width = get_block_width(matrix, i, j);
  buffer->height = height;
  buffer->width = width;
  memcpy(buffer->values, get_block_start(matrix, i, j),
         height * width * sizeof(double));
}

/* ----------------------------------------------------------- */

void
put_block(struct block_matrix *matrix, block *buffer, int i, int j)
{
  const int height = get_block_height(matrix, i, j);
  const int width = get_block_width(matrix, i, j);
  memcpy(get_block_start(matrix, i, j), buffer->values,
         height * width * sizeof(double));
}

/* ----------------------------------------------------------- */
