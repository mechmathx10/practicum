#include "solver.h"
#include "../utils/block_utils.h"
#include "../utils/matrix_utils.h"
#include "../utils/permutation.h"
#include "../io/output.h"

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

enum error_type
inverse_block_matrix(struct block_matrix *matrix, struct block_matrix *result)
{
  int current_state;
#ifdef _DEBUG_
  print_block_matrix_full_m(stdout, matrix, "Start: source matrix");
  print_block_matrix_full_m(stdout, result, "Start: result matrix");
#endif

  const int M = matrix->block_size;
  const int N = matrix->size;
  const int K = N / M;
  const int R = N % M;

  struct permutation col_perm;
  init_permutation(&col_perm, matrix->full_block_count);
  print_permutation(stdout, &col_perm);

  int main_block_index;
  block main_block;
  make_zero_block(&main_block, M);
  block buffer_alpha;
  make_zero_block(&buffer_alpha, M);
  block buffer_beta;
  make_zero_block(&buffer_beta, M);
  block buffer_inversed;
  make_zero_block(&buffer_inversed, M);

  for (int i = 0; i < K; ++i)
    {
#if _DEBUG_
      printf("STEP %d\n-------------------------------\n", i);
#endif
      main_block_index = find_row_main_block(matrix, &buffer_alpha,
                                             &buffer_beta, i);
      if (main_block_index == NOT_FOUND)
        {
          fprintf(stderr, "Cannot pick main block, matrix is singular.\n");
          DELETE(col_perm);
          DELETE(main_block);
          DELETE(buffer_alpha);
          DELETE(buffer_beta);
          DELETE(buffer_inversed);
          return ET_SINGULAR;
        }
#if _DEBUG_
      get_block(matrix, &main_block, i, main_block_index);
      print_block_matrix_full_m(stdout, matrix, "Source matrix");
      print_block_matrix_full_m(stdout, result, "Inversed matrix");
      print_simple_matrix_m(stdout, &main_block, "Main block");
      get_block(matrix, &buffer_alpha, i, main_block_index);
      inverse_block(&buffer_alpha, &buffer_beta);
      print_simple_matrix_m(stdout, &buffer_beta, "Inversed main block");
      fprintf(stdout, "Inversed norm: %f\n", simple_matrix_norm(&buffer_beta));
#endif
      if (main_block_index != i)
        {
          swap_block_columns(matrix, &buffer_alpha, i, main_block_index);
          add_transposition(&col_perm, i, main_block_index);
        }

      get_block(matrix, &main_block, i, main_block_index);
      inverse_block(&main_block, &buffer_inversed);
      for (int j = i; j < K; ++j)
        {
          get_block(matrix, &buffer_alpha, i, j);
          multiply_blocks(&buffer_inversed, &buffer_alpha, &buffer_beta);
          put_block(matrix, &buffer_beta, i, j);
        }
      for (int j = 0; j < K; ++j)
        {
          get_block(result, &buffer_alpha, i, j);
          multiply_blocks(&buffer_inversed, &buffer_alpha, &buffer_beta);
          put_block(result, &buffer_beta, i, j);
        }
      if (R != 0)
        {
          get_block(matrix, &buffer_alpha, i, K);
          multiply_blocks(&buffer_inversed, &buffer_alpha, &buffer_beta);
          put_block(matrix, &buffer_beta, i, K);
          get_block(result, &buffer_alpha, i, K);
          multiply_blocks(&buffer_inversed, &buffer_alpha, &buffer_beta);
          put_block(result, &buffer_beta, i, K);
        }

      for (int l = 0; l < K; ++l)
        {
          if (l == i)
            continue;
          get_block(matrix, &main_block, l, i);
          for (int k = 0; k < K; ++k)
            {
              get_block(matrix, &buffer_inversed, i, k);
              multiply_blocks(&main_block, &buffer_inversed, &buffer_alpha);
              get_block(matrix, &buffer_beta, l, k);
              substract_block(&buffer_beta, &buffer_alpha);
              put_block(matrix, &buffer_beta, l, k);

              get_block(result, &buffer_inversed, i, k);
              multiply_blocks(&main_block, &buffer_inversed, &buffer_alpha);
              get_block(result, &buffer_beta, l, k);
              substract_block(&buffer_beta, &buffer_alpha);
              put_block(result, &buffer_beta, l, k);
            }
        }
      if (R != 0)
        {
          get_block(matrix, &main_block, K, i);
          for (int k = 0; k < K; ++k)
            {
              get_block(matrix, &buffer_inversed, i, k);
              multiply_blocks(&main_block, &buffer_inversed, &buffer_alpha);
              get_block(matrix, &buffer_beta, K, k);
              substract_block(&buffer_beta, &buffer_alpha);
              put_block(matrix, &buffer_beta, K, k);

              get_block(result, &buffer_inversed, i, k);
              multiply_blocks(&main_block, &buffer_inversed, &buffer_alpha);
              get_block(result, &buffer_beta, K, k);
              substract_block(&buffer_beta, &buffer_alpha);
              put_block(result, &buffer_beta, K, k);
            }
        }
    }

  print_block_matrix_full_m(stdout, matrix, "Source matrix");
  print_block_matrix_full_m(stdout, result, "Inversed matrix");

  get_block(matrix, &main_block, K, K);
  current_state = inverse_block(&main_block, &buffer_inversed);
  if (current_state != ET_CORRECT)
    {
      fprintf(stderr, "Cannot pick main block, matrix is singular.\n");
      DELETE(col_perm);
      DELETE(main_block);
      DELETE(buffer_alpha);
      DELETE(buffer_beta);
      DELETE(buffer_inversed);
      return ET_SINGULAR;
    }
  /*memset(main_block.values, 0, SQUARE_DOUB(R));
  for (int i = 0; i < R; ++i)
    main_block.values[i * (R + 1)] = 1;
  put_block(matrix, &main_block, K, K);*/
  for (int j = 0; j < K; ++j)
    {
      get_block(result, &buffer_alpha, K, j);
      multiply_blocks(&buffer_inversed, &buffer_alpha, &buffer_beta);
      put_block(result, &buffer_beta, K, j);
    }
  if (R != 0)
    {
      get_block(result, &buffer_alpha, K, K);
      multiply_blocks(&buffer_inversed, &buffer_alpha, &buffer_beta);
      put_block(result, &buffer_beta, K, K);
    }

  for (int l = 0; l < K; ++l)
    {
      get_block(matrix, &main_block, l, K);
      for (int k = 0; k < K; ++k)
        {
          get_block(result, &buffer_inversed, K, k);
          multiply_blocks(&main_block, &buffer_inversed, &buffer_alpha);
          get_block(result, &buffer_beta, l, k);
          substract_block(&buffer_beta, &buffer_alpha);
          put_block(result, &buffer_beta, l, k);
        }
    }

  print_block_matrix_full_m(stdout, matrix, "Source matrix");
  print_block_matrix_full_m(stdout, result, "Inversed matrix");

  DELETE(main_block);
  DELETE(buffer_alpha);
  DELETE(buffer_beta);
  DELETE(col_perm);
  DELETE(buffer_inversed);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
