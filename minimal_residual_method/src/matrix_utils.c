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
init_vector(struct vector *vector, const int size)
{
  vector->values = (double *) malloc(size * sizeof(double));
  vector->size = size;
}

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

// first -= second * third
void
substract_multiplied_vector(struct vector *minuend,
                            const struct vector * const subtrahend,
                            const double multiplier)
{
  const int N = minuend->size;
  for (int i = 0; i < N; ++i)
    {
      minuend->values[i] -= subtrahend->values[i] * multiplier;
      // printf("%f\t%f\n", minuend->values[i], subtrahend->values[i]);
    }
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
multiply_matrix_and_vector(const struct simple_matrix * const matrix,
                           const struct vector * const vector,
                           struct vector *result)
{
  const int N = vector->size;
  result->size = N;
  memset(result->values, 0, N * sizeof(double));
  for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
        result->values[i] += matrix->values[i * N + j] * vector->values[j];
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
inverse_simple_matrix(struct simple_matrix *matrix,
                      struct simple_matrix *result)
{
  const int N = matrix->height;
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
  const int m = first_matrix->height;
  const int n = first_matrix->width;
  const int l = second_matrix->width;
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
scalar_product(const struct vector * const first_vector,
               const struct vector * const second_vector)
{
  const int N = first_vector->size;
  double result = 0;
  for (int i = 0; i < N; ++i)
    result += first_vector->values[i] * second_vector->values[i];
  return result;
}

/* ----------------------------------------------------------- */

double
vector_norm(const struct vector * const vector)
{
  const int N = vector->size;
  double result = 0;
  for (int i = 0; i < N; ++i)
    {
      double current_abs = fabs(vector->values[i]);
      if (current_abs > result)
        result = current_abs;
    }
  return result;
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
