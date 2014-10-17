#ifndef SOLVER_H
#define SOLVER_H

#include "datatypes.h"
#include "block_utils.h"
#include "matrix_utils.h"

/* ----------------------------------------------------------- */

/* we assume that both matrices are already initialized */
void inverse_block_matrix(struct block_matrix *matrix,
                          struct block_matrix *result);

/* ----------------------------------------------------------- */

#endif
