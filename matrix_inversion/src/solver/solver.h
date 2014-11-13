#ifndef SOLVER_H
#define SOLVER_H

#include "../datatypes.h"

#include <stdio.h>

/* ----------------------------------------------------------- */

void
swap_block_columns(struct block_matrix *, block *, const int, const int);


void
swap_block_rows(struct block_matrix *, block *, const int, const int);


int
find_row_main_block(const struct block_matrix * const,
                    block *, block *, const int);


/* we assume that both matrices are already initialized */
enum error_type
inverse_block_matrix(struct block_matrix *, struct block_matrix *);

/* ----------------------------------------------------------- */

#endif // SOLVER_H
