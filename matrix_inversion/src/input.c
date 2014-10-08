#include "input.h"

#include <stdlib.h>

/* ----------------------------------------------------------- */

static
enum error_type
read_values(FILE *input_stream, double *values, int count)
{
  int r;
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
read_vector(FILE *input_stream, struct vector *vector)
{
  int r;
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
  return read_values(input_stream, vector->values, vector->size);
}

/* ----------------------------------------------------------- */

enum error_type
read_simple_matrix(FILE *input_stream, struct simple_matrix *matrix)
{
  int r;
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
                     matrix->height * matrix->width);
}

/* ----------------------------------------------------------- */

enum error_type
read_square_matrix(FILE *input_stream, struct simple_matrix *matrix)
{
  int r;
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
                     matrix->height * matrix->width);
}

/* ----------------------------------------------------------- */

enum error_type
read_extended_matrix(FILE *input_stream,
                     struct simple_matrix *matrix,
                     struct vector *vector)
{
  int r;
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
read_block_matrix(FILE *input_stream, struct block_matrix *matrix)
{
  if (matrix->block_size < 1)
    {
      fprintf(stderr, "Cannot read block matrix: non-positive block size!\n");
      return ET_ARG_ERROR;
    }

  int r;
  r = fscanf(input_stream, "%d", &matrix->size);
  if (r < 1)
    {
      fprintf(stderr, "Cannot read matrix size!\n");
      return ET_INPUT_ERROR;
    }
  if (matrix->size < 1)
    {
      fprintf(stderr, "Non-positive matrix size!\n");
      matrix->size = 0;
      return ET_INPUT_ERROR;
    }

  matrix->values = (double *) malloc(matrix->size * matrix->size
                                                  * sizeof(double));

#define MAGIC 100500
  int N = matrix->size;
  int M = matrix->block_size;
  int K = (N / M) + 1;
  int current_index;
  for (int i = 0; i < matrix->size; ++i)
    {
      for (int j = 0; j < matrix->size; ++j)
        {
          /*((i / M) * K + j / M)
          (M * ((((i / M) * K + j / M) / K == K - 1) ? N % M : M))
          ((((i / M) * K + j / M) % K == K - 1) ? N % M : M)*/
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
  return ET_CORRECT;
}
