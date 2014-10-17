#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "datatypes.h"

/* ----------------------------------------------------------- */

// does not allocate memory for block's values
inline
void
make_zero_block(block *, const int);


// does not allocate memory for block's values
inline
void
make_unit_block(block *, const int);


// we assume that the both args are square blocks of the same size
enum error_type
inverse_block(block *, block *);


enum error_type
multiply_blocks(const block * const, const block * const, block *);

double
simple_matrix_norm(const block * const);

double
block_matrix_norm(const struct block_matrix * const);

/* ----------------------------------------------------------- */

#endif
