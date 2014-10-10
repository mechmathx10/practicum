#include "matrix_utils.h"
#include "output.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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
  int n = matrix->height;

  printf("started\n");

  memset(result->values, 0, SQUARE_DOUB(n));
  for (int i = 0; i < n; ++i)
    result->values[i * (n + 1)] = 1;

  double cur_elem;
  double cur_row_elem;
  for (int i = 0; i < n; ++i)
    {
      // TODO: select main element
      cur_elem = matrix->values[i * (n + 1)];
      for (int j = i; j < n; ++j)
        {
          matrix->values[i * n + j] /= cur_elem;
        }
      for (int j = 0; j < n; ++j)
        {
          result->values[i * n + j] /= cur_elem;
        }
      print_simple_matrix(stdout, matrix);
      print_simple_matrix(stdout, result);
      for (int l = 0; l < n; ++l)
        {
          printf("started %d row\n", l);
          if (l == i)
            continue;
          cur_row_elem = matrix->values[l * n + i];
          for (int k = 0; k < n; ++k)
            {
              matrix->values[l * n + k] -=
                  cur_row_elem * matrix->values[i * n + k];
              result->values[l * n + k] -=
                  cur_row_elem * result->values[i * n + k];
              print_simple_matrix(stdout, matrix);
              print_simple_matrix(stdout, result);
            }
          printf("finished %d row\n", l);
        }
    }
  printf("finished\n");
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
multiply_blocks(const block * const first_matrix,
                const block * const second_matrix,
                block *result)
{
  int m = first_matrix->height;
  int n = first_matrix->width;
  int l = second_matrix->width;
  for (int i = 0; i < m; ++i)
    {
      for (int j = 0; j < l; ++j)
        {
          result->values[i * m + j] = 0;
          for (int k = 0; k < n; ++k)
            {
              result->values[i * m + j] +=
                  first_matrix->values[i * n + k] *
                  second_matrix->values[k * l + j];
            }
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
