#include "solver.h"
#include "block_utils.h"
#include "matrix_utils.h"
#include "permutation.h"
#include "output.h"

#include <stdlib.h>
#include <string.h>

#define _DEBUG_ 1

#define NOT_FOUND -1



/* ----------------------------------------------------------- */

void swap_block_columns(struct block_matrix *matrix, block *buffer,
                        const int i, const int j)
{
  for (int k = 0; k < matrix->full_block_count; ++k)
    {
      get_block(matrix, buffer, k, i);
      memcpy(get_block_start(matrix, k, i),
             get_block_start(matrix, k, j),
             SQUARE_DOUB(matrix->block_size));
      put_block(matrix, buffer, k, j);
    }
}

/* ----------------------------------------------------------- */

void swap_block_rows(struct block_matrix *matrix, block *buffer,
                     const int i, const int j)
{
  for (int k = 0; k < matrix->full_block_count; ++k)
    {
      get_block(matrix, buffer, i, k);
      memcpy((void *) get_block_start(matrix, i, k),
             (void *) get_block_start(matrix, j, k),
             SQUARE_DOUB(matrix->block_size));
      put_block(matrix, buffer, j, k);
    }
}

/* ----------------------------------------------------------- */

// TODO : fix singular first block case
int
find_row_main_block(const struct block_matrix * const matrix,
                    block *buffer, block *inversed_buffer, const int row)
{
  double current_min_norm = -1;
  double current_norm;
  int current_min_index = 0;
  int current_state = ET_CORRECT;

  int tries_taken;

  for (tries_taken = 0; tries_taken < matrix->full_block_count; ++tries_taken)
    {
      get_block(matrix, buffer, row, tries_taken);
      current_state = inverse_block(buffer, inversed_buffer);
      if (current_state == ET_CORRECT)
        {
          current_min_norm = simple_matrix_norm(inversed_buffer);
          current_min_index = tries_taken;
          break;
        }
      else
        continue;
    }
  if (current_min_norm < 0 && tries_taken == matrix->full_block_count)
      return NOT_FOUND;
  else
    ++tries_taken;

  for (int i = tries_taken; i < matrix->full_block_count; ++i)
    {
      get_block(matrix, buffer, row, i);
      current_state = inverse_block(buffer, inversed_buffer);
      if (current_state == ET_CORRECT)
        {
          current_norm = simple_matrix_norm(inversed_buffer);
          if (current_norm < current_min_norm)
            {
              current_min_norm = current_norm;
              current_min_index = i;
            }
        }
    }
  return current_min_index;
}

/* ----------------------------------------------------------- */

void inverse_block_matrix(struct block_matrix *matrix,
                          struct block_matrix *result)
{
#ifdef _DEBUG_
  print_block_matrix_full_m(stdout, matrix, "Start: source matrix");
  print_block_matrix_full_m(stdout, result, "Start: result matrix");
#endif

  const int block_size = matrix->block_size;
  const int size = matrix->size;
  UNUSED(block_size);
  UNUSED(size);

  struct permutation col_perm;
  init_permutation(&col_perm, matrix->full_block_count);
  print_permutation(stdout, &col_perm);

  DELETE(col_perm);
}

/* ----------------------------------------------------------- */
