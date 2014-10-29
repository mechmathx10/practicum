#ifndef SOLVER_H
#define SOLVER_H

#include "datatypes.h"

#include <stdio.h>

/* ----------------------------------------------------------- */

// arguments are:
//  coefficient matrix
//  free terms vector
//  result vector
//  solution precision
int
solve_linear_system(FILE *,
                    const struct simple_matrix * const,
                    const struct vector * const,
                    struct vector *,
                    const double);

/* ----------------------------------------------------------- */

#endif // SOLVER_H
