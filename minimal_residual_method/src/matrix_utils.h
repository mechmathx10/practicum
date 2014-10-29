#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "datatypes.h"

/* ----------------------------------------------------------- */

inline
void
init_vector(struct vector *, const int);

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


// first -= second * third
void
substract_multiplied_vector(struct vector *,
                            const struct vector * const,
                            const double);


// first -= second
void
substract_simple_matrix(struct simple_matrix *,
                        const struct simple_matrix * const);


// we assume that the both args are square matrices of the same size
enum error_type
inverse_simple_matrix(struct simple_matrix *, struct simple_matrix *);


enum error_type
multiply_matrix_and_vector(const struct simple_matrix * const,
                           const struct vector * const,
                           struct vector *);


enum error_type
multiply_simple_matrices(const struct simple_matrix * const,
                         const struct simple_matrix * const,
                         struct simple_matrix *);


double
scalar_product(const struct vector * const, const struct vector * const);


double
vector_norm(const struct vector * const);


double
simple_matrix_norm(const struct simple_matrix * const);


/* ----------------------------------------------------------- */

#endif // MATRIX_UTILS_H
