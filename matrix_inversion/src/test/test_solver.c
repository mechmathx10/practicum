#include "test_solver.h"
#include "../io/input.h"
#include "../io/output.h"
#include "../utils/block_utils.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _DEBUG_ 0

#define NOT_FOUND -1

static const double EPS = 1e-15;

/* ----------------------------------------------------------- */

void
swap_columns(struct simple_matrix *matrix, const int i, const int j)
{
  const int row_size = matrix->width;
  double buff;
  for (int k = 0; k < matrix->height; ++k)
    {
      buff = matrix->values[k * row_size + i];
      matrix->values[k * row_size + i] = matrix->values[k * row_size + j];
      matrix->values[k * row_size + j] = buff;
    }
}

/* ----------------------------------------------------------- */

void
swap_rows(struct simple_matrix *matrix, double *buff, const int i, const int j)
{
  const int row_size = matrix->width;
  memcpy(buff, &matrix->values[i * row_size], row_size * sizeof(double));
  memcpy(&matrix->values[i * row_size],
         &matrix->values[j * row_size],
         row_size * sizeof(double));
  memcpy(&matrix->values[j * row_size], buff, row_size * sizeof(double));
}

/* ----------------------------------------------------------- */

int
find_row_main_element(const struct simple_matrix * const matrix, const int row)
{
  double current_max_abs = 0;
  int current_max_index = -1;

  double current_abs;
  double *row_begin = &matrix->values[row * matrix->width];
  for (int i = 0; i < matrix->width; ++i)
    {
      current_abs = fabs(row_begin[i]);
#if _DEBUG_
      printf("Current_abs %d = %f\n", i, row_begin[i]);
#endif
      if (current_abs > current_max_abs)
        {
          current_max_index = i;
          current_max_abs = current_abs;
        }
    }
  if (current_max_abs < EPS)
    return -1;
#if _DEBUG_
  printf("Returning %d", current_max_index);
#endif
  return current_max_index;
}

/* ----------------------------------------------------------- */

void
apply_row_permutation(struct simple_matrix *matrix, void *buff,
                      struct permutation *permutation)
{
  const int N = matrix->height;
  void *second_buff = malloc(N * sizeof(double));
  int next_index;
  int cur_index;
  for (int i = 0; i < N; ++i)
    {
      if (permutation->values[i] == -1)
        continue;
      cur_index = i;
      next_index = permutation->values[i];
#if _DEBUG_
      printf("Starting permutation cycle: ");
#endif
      if (next_index == i)
        {
          printf("%d\n", i);
          permutation->values[i] = -1;
          continue;
        }
      memcpy(buff, &matrix->values[i * N], N * sizeof(double));
      while (next_index != -1)
        {
#if _DEBUG_
          printf("%d ", cur_index);
#endif
          memcpy(second_buff, &matrix->values[next_index * N],
                 N * sizeof(double));
          memcpy(&matrix->values[next_index * N], buff, N * sizeof(double));
          memcpy(buff, second_buff, N * sizeof(double));
          permutation->values[cur_index] = -1;
          cur_index = next_index;
          next_index = permutation->values[next_index];
        }
#if _DEBUG_
      printf("\n");
#endif
    }
  free(second_buff);
}

/* ----------------------------------------------------------- */

enum error_type
inverse_matrix(struct simple_matrix *matrix, struct simple_matrix *result)
{
  const int N = matrix->height; // == matrix->width
  double *buff = (double *) malloc(N * sizeof(double));
  struct permutation var_perm;
  init_permutation(&var_perm, N);

#if _DEBUG_
  print_simple_matrix_m(stdout, matrix, "Start: source matrix");
  print_simple_matrix_m(stdout, result, "Start: inversed matrix");
  print_permutation(stdout, &var_perm);
#endif

  double cur_elem;
  double cur_row_elem;
  int max_elem_index;
  for (int i = 0; i < N; ++i)
    {
#if _DEBUG_
      printf("STEP %d\n-------------------------------\n", i);
#endif
      max_elem_index = find_row_main_element(matrix, i);
      if (max_elem_index == NOT_FOUND)
        {
          free(buff);
          DELETE(var_perm);
          return ET_SINGULAR;
        }
#if _DEBUG_
      print_simple_matrix_m(stdout, matrix, "Source matrix");
      print_simple_matrix_m(stdout, result, "Inversed matrix");
      printf("Main: %f\tIndex:%d\n", matrix->values[i * N + max_elem_index],
                                     max_elem_index);
#endif
      if (max_elem_index != i)
        {
          swap_columns(matrix, i, max_elem_index);
          add_transposition(&var_perm, i, max_elem_index);
        }

      cur_elem = matrix->values[i * (N + 1)];
      for (int j = i; j < N; ++j)
          matrix->values[i * N + j] /= cur_elem;
      for (int j = 0; j < N; ++j)
          result->values[i * N + j] /= cur_elem;
      for (int l = 0; l < N; ++l)
        {
          if (l == i)
            continue;
          cur_row_elem = matrix->values[l * N + i];
          for (int k = 0; k < N; ++k)
            {
              matrix->values[l * N + k] -=
                  cur_row_elem * matrix->values[i * N + k];
              result->values[l * N + k] -=
                  cur_row_elem * result->values[i * N + k];
            }
        }
    }

#if _DEBUG_
  printf("FINISHED\n-------------------------------\n");
  print_simple_matrix_m(stdout, matrix, "End: source matrix");
  print_simple_matrix_m(stdout, result, "End: inversed matrix");
  print_permutation(stdout, &var_perm);
  printf("Applying permutation...\n");
#endif

  apply_row_permutation(result, buff, &var_perm);

#if _DEBUG_
  printf("-------------------------------\nFinal answer\n");
  print_simple_matrix_m(stdout, matrix, "Final: source matrix");
  print_simple_matrix_m(stdout, result, "Final: inversed matrix");
#endif

  DELETE(var_perm);
  free(buff);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
