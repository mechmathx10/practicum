#include "solver.h"
#include "matrix_utils.h"
#include "output.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _DEBUG_ 1

#define NOT_FOUND -1

/* ----------------------------------------------------------- */

int
solve_linear_system(FILE *output_stream,
                    const struct simple_matrix * const coefficients,
                    const struct vector * const free_terms,
                    struct vector *solution,
                    const double eps)
{
  const int size = free_terms->size;
  struct vector r_current;
  init_vector(&r_current, size);
  struct vector r_transformed;  //  coefficients * r_current
  init_vector(&r_transformed, size);
  struct vector x_current;
  init_vector(&x_current, size);
  struct vector x_next;
  init_vector(&x_next, size);
  double tau;

  int n = 0; // iteration
  tau = 0;
  for (int i = 0; i < size; ++i)
    x_current.values[i] = (double) 0;

  fprintf(output_stream, "n\t");
  for (int i = 0; i < size; ++i)
    fprintf(output_stream, "x_%d\t\t", i);
  fprintf(output_stream, "\n");
  while (1)
    {
      fprintf(output_stream, "%d\t", n);
      print_vector(output_stream, &x_current);

      multiply_matrix_and_vector(coefficients, &x_current, &r_current);
      for (int i = 0; i < size; ++i)
        r_current.values[i] -= free_terms->values[i];
      if (vector_norm(&r_current) < eps * eps)
        {
          memcpy(x_next.values, x_current.values, size * sizeof(double));
          fprintf(output_stream, "||r^%d|| = 0\nStopped.\n", n);
          ++n;
          break;
        }

      multiply_matrix_and_vector(coefficients, &r_current, &r_transformed);
      double norm = vector_norm(&r_transformed);

      tau = scalar_product(&r_transformed, &r_current) / (norm * norm);
      fprintf(output_stream, "d = %f\n", tau);
      memcpy(x_next.values, x_current.values, size * sizeof(double));
      for (int i = 0; i < size; ++i)
        x_next.values[i] = x_current.values[i] - (r_current.values[i] * tau);

      for (int i = 0; i < size; ++i)
          x_current.values[i] -= x_next.values[i];
      double residual = vector_norm(&x_current);
      fprintf(output_stream, "||x^%d - x^%d|| = %f\n", n + 1, n, residual);
      fprintf(output_stream, "||r^%d|| = %f\n", n, vector_norm(&r_current));
      if (residual < eps)
        {
          break;
        }
      for (int i = 0; i < size; ++i)
          x_current.values[i] += x_next.values[i];
      fprintf(output_stream, "||x^%d - x^%d|| / ||x^%d|| = %f\n", n + 1, n, n,
              residual / vector_norm(&x_current));
      fprintf(output_stream, "-------------------------------------\n");
      ++n;
      memcpy(x_current.values, x_next.values, size * sizeof(double));
    }

  memcpy(solution->values, x_next.values, size * sizeof(double));
  fprintf(output_stream, "-------------------------------------\n");
  fprintf(output_stream, "Solution:\n");
  for (int i = 0; i < size; ++i)
      fprintf(output_stream, "%f\t", solution->values[i]);
  fprintf(output_stream, "\nIterations taken:\n%d\n", n);
  multiply_matrix_and_vector(coefficients, solution, &r_current);
  fprintf(output_stream, "Residual:\n");
  for (int i = 0; i < size; ++i)
    {
      r_current.values[i] -= free_terms->values[i];
      fprintf(output_stream, "%e\t", (-1) * r_current.values[i]);
    }
  fprintf(output_stream, "\nResidual norm:\n%e\n", vector_norm(&r_current));

  DELETE(r_current);
  DELETE(r_transformed);
  DELETE(x_current);
  DELETE(x_next);
  return n;
}

/* ----------------------------------------------------------- */
