#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "datatypes.h"

/* ----------------------------------------------------------- */

// does not allocate memory for struct simple_matrix's values
inline
void
set_zero_simple_matrix(struct simple_matrix *);


// does not allocate memory for struct simple_matrix's values
inline
void
set_unit_simple_matrix(struct simple_matrix *);


// allocates memory for struct simple_matrix's values
inline
void
make_zero_simple_matrix(struct simple_matrix *, const int);


// allocates memory for struct simple_matrix's values
inline
void
make_unit_simple_matrix(struct simple_matrix *, const int);


// first -= second
void
substract_simple_matrix(struct simple_matrix *,
                        const struct simple_matrix * const);


// we assume that the both args are square matrices of the same size
enum error_type
inverse_simple_matrix(struct simple_matrix *, struct simple_matrix *);


enum error_type
multiply_simple_matrices(const struct simple_matrix * const,
                         const struct simple_matrix * const,
                         struct simple_matrix *);


double
simple_matrix_norm(const struct simple_matrix * const);


/* ----------------------------------------------------------- */

#endif // MATRIX_UTILS_H
