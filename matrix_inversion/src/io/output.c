#include "output.h"
#include "../utils/block_utils.h"
#include "../utils/matrix_utils.h"

#define EXTENDED_OUTPUT 1

/* ----------------------------------------------------------- */

void
print_vector(FILE *output_stream, const struct vector * const vector)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Vector size: %d\n", vector->size);
#endif
  for (int i = 0; i < vector->size; ++i)
    fprintf(output_stream, "%f\n", vector->values[i]);
}

/* ----------------------------------------------------------- */

void
print_vector_m(FILE *output_stream,
               const struct vector * const vector,
               const char * const message)
{
  fprintf(output_stream, "%s\n", message);
  print_vector(output_stream, vector);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */

void
print_simple_matrix(FILE *output_stream,
                    const struct simple_matrix * const matrix)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Matrix size: %dx%d\n", matrix->height,
                                                  matrix->width);
#endif
  for (int i = 0; i < matrix->height; ++i)
    {
      for (int j = 0; j < matrix->width; ++j)
        fprintf(output_stream, "%f ", matrix->values[i * matrix->width + j]);
      fprintf(output_stream, "\n");
    }
}

/* ----------------------------------------------------------- */

void
print_simple_matrix_m(FILE *output_stream,
                      const struct simple_matrix * const matrix,
                      const char * const message)
{
  fprintf(output_stream, "%s\n", message);
  print_simple_matrix(output_stream, matrix);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */

void
print_extended_matrix(FILE *output_stream,
                      const struct simple_matrix * const matrix,
                      const struct vector * const vector)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Matrix(extended) size: %d\n", matrix->height);
#endif
  for (int i = 0; i < matrix->height; ++i)
    {
      for (int j = 0; j < matrix->width; ++j)
        fprintf(output_stream, "%f ", matrix->values[i * matrix->height + j]);
      fprintf(output_stream, " | %f\n", vector->values[i]);
    }
}

/* ----------------------------------------------------------- */

void
print_extended_matrix_m(FILE *output_stream,
                        const struct simple_matrix * const matrix,
                        const struct vector * const vector,
                        const char * const message)
{
  fprintf(output_stream, "%s\n", message);
  print_extended_matrix(output_stream, matrix, vector);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */

void
print_block_matrix(FILE *output_stream,
                   const struct block_matrix * const matrix)
{
  /* blocks linear count */
  int blc = DIV_UP(matrix->size, matrix->block_size);
#if EXTENDED_OUTPUT
  fprintf(output_stream, "\nSize: %d\nBlock size: %d\n", matrix->size,
                                                    matrix->block_size);
#endif
  for (int i = 0; i < blc; ++i)
    {
      for (int j = 0; j < blc; ++j)
        {
          int height = get_block_height(matrix, i, j);
          int width = get_block_width(matrix, i, j);
          double *block_ptr = get_block_start(matrix, i, j);
#if EXTENDED_OUTPUT
          fprintf(output_stream, "------------------\nBlock (%d, %d)\n", i, j);
#endif
          for (int k = 0; k < height; ++k)
            {
              for (int l = 0; l < width; ++l)
                {
                  fprintf(output_stream, "%f ", block_ptr[k * width + l]);
                }
              fprintf(output_stream, "\n");
            }
        }
    }
}

/* ----------------------------------------------------------- */

void
print_block_matrix_m(FILE *output_stream,
                     const struct block_matrix * const matrix,
                     const char * const message)
{
  fprintf(output_stream, "%s\n", message);
  print_block_matrix(output_stream, matrix);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */

void
print_block_matrix_full(FILE *output_stream,
                        const struct block_matrix * const matrix)
{
  const int N = matrix->size;
  for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
        {
          fprintf(output_stream, "%.3lf\t",
                  get_block_matrix_element(matrix, i, j));
        }
      fprintf(output_stream, "\n");
    }
}

/* ----------------------------------------------------------- */

void
print_block_matrix_full_m(FILE *output_stream,
                          const struct block_matrix * const matrix,
                          const char * const message)
{
  fprintf(output_stream, "%s\n", message);
  print_block_matrix_full(output_stream, matrix);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */
