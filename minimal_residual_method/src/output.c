#include "output.h"

/* ----------------------------------------------------------- */

void
print_vector(FILE *output_stream, const struct vector * const vector)
{
  for (int i = 0; i < vector->size; ++i)
    fprintf(output_stream, "%f\t", vector->values[i]);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */

void
print_vector_m(FILE *output_stream,
               const struct vector * const vector,
               const char * const message)
{
  fprintf(output_stream, "%s\n", message);
  print_vector(output_stream, vector);
}

/* ----------------------------------------------------------- */

void
print_simple_matrix(FILE *output_stream,
                    const struct simple_matrix * const matrix)
{
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
