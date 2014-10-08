#include "output.h"

#define EXTENDED_OUTPUT 1

/* ----------------------------------------------------------- */

void
print_vector(FILE *output_stream, struct vector *vector)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Vector size: %d\n", vector->size);
#endif
  for (int i = 0; i < vector->size; ++i)
    fprintf(output_stream, "%f\n", vector->values[i]);
}

/* ----------------------------------------------------------- */

void
print_simple_matrix(FILE *output_stream, struct simple_matrix *matrix)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Matrix sizes: %dx%d\n", matrix->height,
                                                  matrix->width);
#endif
  for (int i = 0; i < matrix->height; ++i)
    {
      for (int j = 0; j < matrix->width; ++j)
        fprintf(output_stream, "%f ", matrix->values[i * matrix->height + j]);
      fprintf(output_stream, "\n");
    }
}

/* ----------------------------------------------------------- */

void
print_extended_matrix(FILE *output_stream,
                      struct simple_matrix *matrix,
                      struct vector * vector)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Variables count: %d\n", matrix->height);
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
print_block_matrix(FILE *output_stream, struct block_matrix *matrix)
{
  /* blocks linear count */
  int blc = DIV_UP(matrix->size, matrix->block_size);
  /* last block size */
  int lbs = matrix->size % matrix->block_size == 0 ? matrix->block_size :
            matrix->size % matrix->block_size;
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Size: %d\tBlock size:%d\n", matrix->size,
                                                    matrix->block_size);
#endif
  for (int i = 0; i < blc; ++i)
    {
      for (int j = 0; j < blc; ++j)
        {
          int height = i == blc - 1 ? lbs : matrix->block_size;
          int width = j == blc - 1 ? lbs : matrix->block_size;
          double *block_ptr = &matrix->values[
                                  (i * matrix->size * matrix->block_size) +
                                  (j * ((i == blc - 1) ? lbs : matrix->block_size) * matrix->block_size)];
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
