#include "solver.h"
#include "matrix_utils.h"
#include "output.h"

#include <stdlib.h>
#include <string.h>

#define _DEBUG_ 1

#define NOT_FOUND -1

/* ----------------------------------------------------------- */

enum error_type
solve_linear_system(const struct simple_matrix * const coefficients,
                    const struct vector * const free_terms,
                    struct vector *solution,
                    const double eps)
{
  UNUSED(coefficients);
  UNUSED(free_terms);
  UNUSED(solution);
  UNUSED(eps);
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
