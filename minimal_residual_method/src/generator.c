#include "generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------- */

static inline
void
print_row(FILE *output_stream, const double * const coef,
          const double free_term, const int len)
{
  for (int i = 0; i < len; ++i)
    fprintf(output_stream, "%f\t", coef[i]);
  fprintf(output_stream, "%f\n", free_term);
}


enum error_type
generate_matrix(const char * const filename, const int I, const int J,
                double (*f)(const double, const double))
{
  UNUSED(f);

  FILE *output_stream;
  if (! filename)
    {
      output_stream = stdout;
    }
  else
    {
      output_stream = fopen(filename, "w");
      if (! output_stream)
        {
          fprintf(stderr, "Cannot output file %s for output (generator mode)\n",
                  filename);
          return ET_FILE_ERROR;
        }
      printf("File %s opened\n", filename);
    }

  const double hx = 1.0 / (double) I;
  const double hy = 1.0 / (double) J;
  const double hx2 = hx * hx;
  const double hy2 = hy * hy;

  const int equation_count = (I + 1) * (J + 1);
  fprintf(output_stream, "%d\n", equation_count);
  double *coefficient_row = (double *) malloc(equation_count * sizeof(double));
  double free_term;

  for (int i = 0; i < I + 1; ++i)
    {
      for (int j = 0; j < J + 1; ++j)
        {
          memset(coefficient_row, 0, equation_count * sizeof(double));
          if (i == 0 || j == 0 || i == I || j == J)
            {
              coefficient_row[(I + 1) * i + j] = 1;
              free_term = 0;
            }
          else
            {
              coefficient_row[(I + 1) * i + j] = (2 / hx2) + (2 / hy2);
              coefficient_row[(I + 1) * (i + 1) + j] = (-1) / hx2;
              coefficient_row[(I + 1) * (i - 1) + j] = (-1) / hx2;
              coefficient_row[(I + 1) * i + j + 1] = (-1) / hy2;
              coefficient_row[(I + 1) * i + j - 1] = (-1) / hy2;
              free_term = f(i * hx, j * hy);
            }
          print_row(output_stream, coefficient_row, free_term, equation_count);
        }
    }

  free(coefficient_row);
  if (filename)
    {
      fclose(output_stream);
      printf("Generation completed, file closed\n");
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
