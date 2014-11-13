#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "../datatypes.h"

/* ----------------------------------------------------------- */

// does not allocate memory for block's values
inline
void
set_zero_block(block *);


// does not allocate memory for block's values
inline
void
set_unit_block(block *);


// allocates memory for block's values
inline
void
make_zero_block(block *, const int);


// allocates memory for block's values
inline
void
make_unit_block(block *, const int);


// first -= second
void
substract_block(block *, const block * const);


// we assume that the both args are square blocks of the same size
enum error_type
inverse_block(block *, block *);


// TODO : make efficient block multiplication
enum error_type
multiply_blocks(const block * const, const block * const, block *);


double
simple_matrix_norm(const block * const);


double
block_matrix_norm(const struct block_matrix * const);


void
make_zero_block_matrix(struct block_matrix *, const int);


void
make_unit_block_matrix(struct block_matrix *, const int);


void
make_block_matrix_copy(const struct block_matrix * const,
                       struct block_matrix *);


void
substract_unit_block_matrix(struct block_matrix *);


// This function is used for debug only, not optimized at all
inline
double
get_block_matrix_element(const struct block_matrix * const,
                         const int, const int);


void
multiply_block_matrices(const struct block_matrix * const,
                        const struct block_matrix * const,
                        struct block_matrix *);

/* ----------------------------------------------------------- */

#endif // MATRIX_UTILS_H
