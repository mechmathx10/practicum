#include "matrix_utils.h"

#include <string.h>
#include <stdlib.h>

/* ----------------------------------------------------------- */

inline
void
make_zero_block(block *matrix, const int size)
{
  matrix->height = size;
  matrix->width = size;
  matrix->values = (double *) malloc(SQUARE_DOUB(size));
  memset(matrix->values, 0, SQUARE_DOUB(size));
}

/* ----------------------------------------------------------- */

inline
void
make_unit_block(block *matrix, const int size)
{
  make_zero_block(matrix, size);
  for (int i = 0; i < size; ++i)
    matrix->values[i * (size + 1)] = 1;
}

/* ----------------------------------------------------------- */

enum error_type
inverse_block(block *matrix, block *result)
{
  UNUSED(matrix);
  UNUSED(result);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
multiply_blocks(const block * const first_matrix,
                const block * const second_matrix,
                block *result)
{
  UNUSED(first_matrix);
  UNUSED(second_matrix);
  UNUSED(result);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
