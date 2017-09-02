#ifndef TEST_SOLVER_H
#define TEST_SOLVER_H

#include "../datatypes.h"
#include "../utils/permutation.h"

/* ----------------------------------------------------------- */

void
swap_columns(struct simple_matrix *, const int, const int);


void
swap_rows(struct simple_matrix *, double *, const int, const int);


int
find_row_main_element(const struct simple_matrix * const, const int);


void
apply_row_permutation(struct simple_matrix *, void *, struct permutation *);


enum error_type
inverse_matrix(struct simple_matrix *matrix, struct simple_matrix *result);

/* ----------------------------------------------------------- */

#endif // TEST_SOLVER_H
