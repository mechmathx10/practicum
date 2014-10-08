#ifndef BLOCK_UTILS_H
#define BLOCK_UTILS_H

#include "datatypes.h"

/* ----------------------------------------------------------- */

inline
double *
get_block_start(struct block_matrix *, int, int);


inline
int
get_block_height(struct block_matrix *, int, int);


inline
int
get_block_width(struct block_matrix *, int, int);


block
get_block(struct block_matrix *, int, int);

/* ----------------------------------------------------------- */

#endif
