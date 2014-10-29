#include "solver.h"
#include "matrix_utils.h"
#include "output.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _DEBUG_ 1

#define NOT_FOUND -1

/* ----------------------------------------------------------- */

enum error_type
solve_linear_system(FILE *output_stream,
                    const struct simple_matrix * const coefficients,
                    const struct vector * const free_terms,
                    struct vector *solution,
                    const double eps)
{
  UNUSED(output_stream);
  UNUSED(coefficients);
  UNUSED(free_terms);
  UNUSED(solution);
  UNUSED(eps);
  const int size = free_terms->size;
  struct vector r_current;
  init_vector(&r_current, size);
  struct vector r_next;
  init_vector(&r_next, size);
  struct vector r_transformed;  //  coefficients * r_current
  init_vector(&r_transformed, size);
  struct vector x_current;
  init_vector(&x_current, size);
  struct vector x_next;
  init_vector(&x_next, size);
//  double tau_current;
//  double tau_next;

  int n = 0; // iteration
  int stop_flag = 1;
  while (1)
    {
//      for (int i = 0; i < size; ++i)
//        {
//          if (fabs(x_current->values[i] - x_next->values[i]) < eps)
//            {
//              stop_flag = 1;
//              break;
//            }
//        }
      if (stop_flag)
        break;
      ++n;
    }

  memcpy(solution->values, x_next.values, size * sizeof(double));
  DELETE(r_current);
  DELETE(r_next);
  DELETE(r_transformed);
  DELETE(x_current);
  DELETE(x_next);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
