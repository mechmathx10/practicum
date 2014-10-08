#include "output.h"

#define EXTENDED_OUTPUT 1


void
print_vector(FILE *output_stream, struct vector *vector)
{
#if EXTENDED_OUTPUT
  fprintf(output_stream, "Vector size: %d\n", vector->size);
#endif
  for (int i = 0; i < vector->size; ++i)
    fprintf(output_stream, "%f\n", vector->values[i]);
}


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
