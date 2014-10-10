#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "datatypes.h"

/* ----------------------------------------------------------- */

// we assume that the first arg is a square block
enum error_type
inverse_block(block *, block *);


enum error_type
multiply_blocks(const block * const, const block * const, block *);

/* ----------------------------------------------------------- */

#endif
