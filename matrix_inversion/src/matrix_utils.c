#include "matrix_utils.h"

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
