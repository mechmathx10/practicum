#include "matrix_utils.h"
#include "block_utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NOT_FOUND -1

static const double EPS = 1e-15;

/* ----------------------------------------------------------- */

inline
void
set_zero_block(block *matrix)
{
  memset(matrix->values, 0, SQUARE_DOUB(matrix->height));
}

/* ----------------------------------------------------------- */

inline
void
set_unit_block(block *matrix)
{
  const int size = matrix->height;
  memset(matrix->values, 0, SQUARE_DOUB(size));
  for (int i = 0; i < size * size; ++i)
    matrix->values[i * (size + 1)] = 1;
}

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

void
substract_block(block *minuend, const block * const subtrahend)
{
  const int count = minuend->height * minuend->width;
  for (int i = 0; i < count; ++i)
      minuend->values[i] -= subtrahend->values[i];
}

/* ----------------------------------------------------------- */

enum error_type
inverse_block(block *matrix, block *result)
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
multiply_blocks(const block * const first_matrix,
                const block * const second_matrix,
                block *result)
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
simple_matrix_norm(const block * const matrix)
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

// TODO : optimize block_matrix_norm
double
block_matrix_norm(const struct block_matrix * const matrix)
{
  double current_max = 0;
  double current_sum;
  for (int i = 0; i < matrix->size; ++i)
    {
      current_sum = 0;
      for (int j = 0; j < matrix->size; ++j)
        current_sum += fabs(get_block_matrix_element(matrix, i, j));
      if (current_sum > current_max)
        current_max = current_sum;
    }
  return current_max;
}

/* ----------------------------------------------------------- */

void
make_zero_block_matrix(struct block_matrix *matrix, const int size)
{
  matrix->size = size;
  matrix->values = (double *) malloc(SQUARE_DOUB(matrix->size));
  memset(matrix->values, 0, SQUARE_DOUB(size));
  matrix->full_block_count = matrix->size / matrix->block_size;
  matrix->residue = matrix->size % matrix->block_size;
}

/* ----------------------------------------------------------- */

void
make_unit_block_matrix(struct block_matrix *matrix, const int size)
{
  make_zero_block_matrix(matrix, size);
  int N = size;
  int M = matrix->block_size;
  int block_count = N / M;
  for (int i = 0; i < block_count; ++i)
    {
      double *current_ptr = get_block_start(matrix, i, i);
      for (int j = 0; j < M; ++j)
        current_ptr[j * (M + 1)] = 1;
    }
  if (matrix->residue)
    {
      double *current_ptr = get_block_start(matrix, block_count, block_count);
      for (int j = 0; j < matrix->residue; ++j)
        current_ptr[j * (matrix->residue + 1)] = 1;
    }
}

/* ----------------------------------------------------------- */

void
make_block_matrix_copy(const struct block_matrix * const sample_matrix,
                       struct block_matrix *copy_matrix)
{
  copy_matrix->size = sample_matrix->size;
  copy_matrix->block_size = sample_matrix->block_size;
  copy_matrix->residue = sample_matrix->residue;
  copy_matrix->full_block_count = sample_matrix->full_block_count;
  copy_matrix->values = (double *) malloc(SQUARE_DOUB(copy_matrix->size));
  memcpy(copy_matrix->values, sample_matrix->values,
         SQUARE_DOUB(copy_matrix->size));
}

/* ----------------------------------------------------------- */

void
substract_unit_block_matrix(struct block_matrix *matrix)
{
  double *cur_block_ptr;
  const int block_size = matrix->block_size;
  for (int i = 0; i < matrix->full_block_count; ++i)
    {
      cur_block_ptr = get_block_start(matrix, i, i);
      for (int j = 0; j < block_size; ++j)
          cur_block_ptr[j * (block_size + 1)] -= 1;
    }
  if (matrix->residue > 0)
    {
      cur_block_ptr = get_block_start(matrix, matrix->full_block_count,
                                              matrix->full_block_count);
      for (int j = 0; j < matrix->residue; ++j)
        cur_block_ptr[j * (matrix->residue + 1)] -= 1;
    }
}

/* ----------------------------------------------------------- */

inline
double
get_block_matrix_element(const struct block_matrix * const matrix,
                         const int i, const int j)
{
  const int N = matrix->size;
  const int M = matrix->block_size;
  return matrix->values[M * ((N * (i / M)) +
                            ((i / M == N / M ? N % M : M) * (j / M))) +
                        ((j / M == N / M ? N % M : M) * (i % M)) + j % M];
}

/* ----------------------------------------------------------- */

void
multiply_block_matrices(const struct block_matrix * const first_matrix,
                        const struct block_matrix * const second_matrix,
                        struct block_matrix *result)
{
  block buffer;
  make_zero_block(&buffer, first_matrix->block_size);
  UNUSED(first_matrix);
  UNUSED(second_matrix);
  UNUSED(result);
  DELETE(buffer);
}

/* ----------------------------------------------------------- */
