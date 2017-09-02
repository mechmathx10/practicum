#include "input.h"
#include "../utils/block_utils.h"

#include <stdlib.h>

/* ----------------------------------------------------------- */

enum error_type
read_values(FILE *input_stream, double *values,
            int count, enum input_type source_type)
{
  int r;
  if (source_type == IT_CONSOLE)
      printf("Input %d real values: \n", count);
  for (int i = 0; i < count; ++i)
    {
      r = fscanf(input_stream, "%lf", &values[i]);
      if (r < 1)
        {
          fprintf(stderr, "Cannot read element!\n");
          free(values);
          return ET_INPUT_ERROR;
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
read_vector(FILE *input_stream, struct vector *vector,
            enum input_type source_type)
{
  int r;
  if (source_type == IT_CONSOLE)
      printf("Input vector size: ");
  r = fscanf(input_stream, "%d", &vector->size);
  if (r < 1)
    {
      fprintf(stderr, "Cannot read vector size!\n");
      return ET_INPUT_ERROR;
    }
  if (vector->size < 1)
    {
      fprintf(stderr, "Non-positive vector size!\n");
      vector->size = 0;
      return ET_INPUT_ERROR;
    }

  vector->values = (double *) malloc(vector->size * sizeof(double));
  return read_values(input_stream, vector->values, vector->size, source_type);
}

/* ----------------------------------------------------------- */

enum error_type
read_simple_matrix(FILE *input_stream,
                   struct simple_matrix *matrix,
                   enum input_type source_type)
{
  int r;
  if (source_type == IT_CONSOLE)
      printf("Input matrix height and width: ");
  r = fscanf(input_stream, "%d %d", &matrix->height, &matrix->width);
  if (r < 2)
    {
      fprintf(stderr, "Cannot read matrix sizes!\n");
      return ET_INPUT_ERROR;
    }
  if (matrix->height < 1 || matrix->width < 1)
    {
      fprintf(stderr, "Non-positive matrix size!\n");
      matrix->height = 0;
      matrix->width = 0;
      return ET_INPUT_ERROR;
    }

  matrix->values = (double *) malloc(matrix->height * matrix->width
                                                    * sizeof(double));
  return read_values(input_stream, matrix->values,
                     matrix->height * matrix->width,
                     source_type);
}

/* ----------------------------------------------------------- */

enum error_type
read_square_matrix(FILE *input_stream,
                   struct simple_matrix *matrix,
                   enum input_type source_type)
{
  int r;
  if (source_type == IT_CONSOLE)
      printf("Input matrix size (matrix is square): ");
  r = fscanf(input_stream, "%d", &matrix->height);
  if (r < 1)
    {
      fprintf(stderr, "Cannot read matrix size!\n");
      return ET_INPUT_ERROR;
    }
  if (matrix->height < 1)
    {
      fprintf(stderr, "Non-positive matrix size!\n");
      matrix->height = 0;
      return ET_INPUT_ERROR;
    }

  matrix->width = matrix->height;
  matrix->values = (double *) malloc(matrix->height * matrix->width
                                                    * sizeof(double));
  return read_values(input_stream, matrix->values,
                     matrix->height * matrix->width,
                     source_type);
}

/* ----------------------------------------------------------- */

enum error_type
read_extended_matrix(FILE *input_stream,
                     struct simple_matrix *matrix,
                     struct vector *vector,
                     enum input_type source_type)
{
  int r;
  if (source_type == IT_CONSOLE)
      printf("Input matrix size (matrix is square): ");
  r = fscanf(input_stream, "%d", &matrix->height);
  if (r < 1)
    {
      fprintf(stderr, "Cannot read matrix size!\n");
      return ET_INPUT_ERROR;
    }
  if (matrix->height < 1)
    {
      fprintf(stderr, "Non-positive matrix size!\n");
      matrix->height = 0;
      return ET_INPUT_ERROR;
    }

  matrix->width = matrix->height;
  matrix->values = (double *) malloc(matrix->height * matrix->width
                                                    * sizeof(double));
  vector->size = matrix->height;
  vector->values = (double *) malloc(vector->size * sizeof(double));

  if (source_type == IT_CONSOLE)
    printf("Input %d values (standard extended matrix notation)",
           matrix->height * (matrix->height + 1));
  for (int i = 0; i < matrix->height; ++i)
    {
      for (int j = 0; j < matrix->width; ++j)
        {
          r = fscanf(input_stream, "%lf",
                                   &matrix->values[i * matrix->height + j]);
          if (r < 1)
            {
              fprintf(stderr, "Cannot read matrix element!\n");
              free(matrix->values);
              free(vector->values);
              return ET_INPUT_ERROR;
            }
        }
      r = fscanf(input_stream, "%lf", &vector->values[i]);
      if (r < 1)
        {
          fprintf(stderr, "Cannot read matrix element!\n");
          free(matrix->values);
          free(vector->values);
          return ET_INPUT_ERROR;
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
read_block_matrix(FILE *input_stream,
                  struct block_matrix *matrix,
                  enum input_type source_type)
{
  if (matrix->block_size < 1)
    {
      fprintf(stderr, "Cannot read block matrix: non-positive block size!\n");
      return ET_ARG_ERROR;
    }

  int r;
  if (source_type == IT_CONSOLE)
      printf("Input matrix size (matrix is square): ");
  r = fscanf(input_stream, "%d", &matrix->size);
  if (r < 1)
    {
      fprintf(stderr, "Cannot read matrix size!\n");
      return ET_INPUT_ERROR;
    }
  if (matrix->size < matrix->block_size)
    {
      fprintf(stderr, "One block cannot be bigger than a whole matrix!\n");
      matrix->size = 0;
      return ET_INPUT_ERROR;
    }

  matrix->values = (double *) malloc(SQUARE_DOUB(matrix->size));

  const int N = matrix->size; // for big formulas
  if (source_type == IT_CONSOLE)
      printf("Input %d real values: \n", N * N);

#define MAGIC 100500
  const int M = matrix->block_size;
  const int K = (N / M) + 1;
  int current_index;
  for (int i = 0; i < matrix->size; ++i)
    {
      for (int j = 0; j < matrix->size; ++j)
        {
          current_index = ((((i / M) * K + j / M) / K) * (N * M)) +
                        ((((i / M) * K + j / M)) % K) *
                        (M * ((((i / M) * K + j / M) / K == K - 1) ? N % M : M))
                        + (i - ((((i / M) * K + j / M) / K) * M)) *
                        ((((i / M) * K + j / M) % K == K - 1) ? N % M : M) +
                        j - ((((i / M) * K + j / M) % K) * M);
          r = fscanf(input_stream, "%lf", &matrix->values[current_index]);
          if (r < 1)
            {
              fprintf(stderr, "Cannot read matrix element!\n");
              free(matrix->values);
              return ET_INPUT_ERROR;
            }
        }
    }
#undef MAGIC
  matrix->full_block_count = K - 1;
  matrix->residue = N % M;

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
generate_block_matrix(struct block_matrix *matrix, double (*f)(int, int))
{
  int r;
  if (matrix->size < 1)
    {
      printf("Input matrix size: ");
      r = scanf("%d", &matrix->size);
      if (r < 1)
        {
          fprintf(stderr, "Cannot read matrix size!\n");
          return ET_INPUT_ERROR;
        }
      if (matrix->size < matrix->block_size)
        {
          fprintf(stderr, "One block cannot be bigger than a whole matrix!\n");
          matrix->size = 0;
          return ET_INPUT_ERROR;
        }
    }

  matrix->values = (double *) malloc(SQUARE_DOUB(matrix->size));

  const int N = matrix->size;
  const int M = matrix->block_size;
  const int K = (N / M) + 1;
  int current_index;
  for (int i = 0; i < matrix->size; ++i)
    {
      for (int j = 0; j < matrix->size; ++j)
        {
          current_index = ((((i / M) * K + j / M) / K) * (N * M)) +
                        ((((i / M) * K + j / M)) % K) *
                        (M * ((((i / M) * K + j / M) / K == K - 1) ? N % M : M))
                        + (i - ((((i / M) * K + j / M) / K) * M)) *
                        ((((i / M) * K + j / M) % K == K - 1) ? N % M : M) +
                        j - ((((i / M) * K + j / M) % K) * M);
          matrix->values[current_index] = f(i, j);
        }
    }
  matrix->full_block_count = K - 1;
  matrix->residue = N % M;

  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
