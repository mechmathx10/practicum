#include "matrix_utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NOT_FOUND -1

static const double EPS = 1e-15;

/* ----------------------------------------------------------- */

inline
void
set_zero_simple_matrix(struct simple_matrix *matrix)
{
  memset(matrix->values, 0, SQUARE_DOUB(matrix->height));
}

/* ----------------------------------------------------------- */

inline
void
set_unit_simple_matrix(struct simple_matrix *matrix)
{
  const int size = matrix->height;
  memset(matrix->values, 0, SQUARE_DOUB(size));
  for (int i = 0; i < size * size; ++i)
    matrix->values[i * (size + 1)] = 1;
}

/* ----------------------------------------------------------- */

inline
void
make_zero_simple_matrix(struct simple_matrix *matrix, const int size)
{
  matrix->height = size;
  matrix->width = size;
  matrix->values = (double *) malloc(SQUARE_DOUB(size));
  memset(matrix->values, 0, SQUARE_DOUB(size));
}

/* ----------------------------------------------------------- */

inline
void
make_unit_simple_matrix(struct simple_matrix *matrix, const int size)
{
  make_zero_simple_matrix(matrix, size);
  for (int i = 0; i < size; ++i)
    matrix->values[i * (size + 1)] = 1;
}

/* ----------------------------------------------------------- */

void
substract_simple_matrix(struct simple_matrix *minuend,
                        const struct simple_matrix * const subtrahend)
{
  const int count = minuend->height * minuend->width;
  for (int i = 0; i < count; ++i)
      minuend->values[i] -= subtrahend->values[i];
}

/* ----------------------------------------------------------- */

enum error_type
inverse_simple_matrix(struct simple_matrix *matrix,
                      struct simple_matrix *result)
{
  int N = matrix->height;
  result->height = N;
  result->width = N;

  memset(result->values, 0, SQUARE_DOUB(N));
  for (int i = 0; i < N; ++i)
    result->values[i * (N + 1)] = 1;

  double cur_elem;
  double cur_row_elem;
  for (int i = 0; i < N; ++i)
    {
      cur_elem = matrix->values[i * (N + 1)];
      if (abs(cur_elem) < EPS)
          return ET_SINGULAR;
      for (int j = i; j < N; ++j)
          matrix->values[i * N + j] /= cur_elem;
      for (int j = 0; j < N; ++j)
          result->values[i * N + j] /= cur_elem;
      for (int l = 0; l < N; ++l)
        {
          if (l == i)
            continue;
          cur_row_elem = matrix->values[(l * N) + i];
          for (int k = 0; k < N; ++k)
            {
              matrix->values[l * N + k] -=
                  cur_row_elem * matrix->values[i * N + k];
              result->values[l * N + k] -=
                  cur_row_elem * result->values[i * N + k];
            }
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
multiply_simple_matrices(const struct simple_matrix * const first_matrix,
                         const struct simple_matrix * const second_matrix,
                         struct simple_matrix *result)
{
  int m = first_matrix->height;
  int n = first_matrix->width;
  int l = second_matrix->width;
  memset(result->values, 0, m * l * sizeof(double));
  result->height = first_matrix->height;
  result->width = second_matrix->width;
  for (int i = 0; i < m; ++i)
    {
      for (int j = 0; j < l; ++j)
        {
          for (int k = 0; k < n; ++k)
            {
              result->values[i * l + j] +=
                  first_matrix->values[i * n + k] *
                  second_matrix->values[k * l + j];
            }
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

double
simple_matrix_norm(const struct simple_matrix * const matrix)
{
  double current_max = 0;
  double current_sum;
  double *current_ptr = matrix->values;
  for (int i = 0; i < matrix->height; ++i)
    {
      current_sum = 0;
      for (int j = 0; j < matrix->width; ++j)
        current_sum += fabs(current_ptr[j]);
      if (current_sum > current_max)
        current_max = current_sum;
      current_ptr += matrix->width;
    }
  return current_max;
}

/* ----------------------------------------------------------- */
