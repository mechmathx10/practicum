#ifndef OUTPUT_H
#define OUTPUT_H

#include "../datatypes.h"

#include <stdio.h>

/* ----------------------------------------------------------- */
/* everywhere f_m functions prints given message and then calls f */

void
print_vector(FILE *, const struct vector * const);


void
print_vector_m(FILE *, const struct vector * const, const char * const);


void
print_simple_matrix(FILE *, const struct simple_matrix * const);


void
print_simple_matrix_m(FILE *,
                      const struct simple_matrix * const,
                      const char * const);


void
print_simple_matrix(FILE *, const struct simple_matrix * const);


void
print_simple_matrix_m(FILE *,
                      const struct simple_matrix * const,
                      const char * const);


void
print_extended_matrix(FILE *,
                      const struct simple_matrix * const,
                      const struct vector * const);


void
print_extended_matrix_m(FILE *,
                        const struct simple_matrix * const,
                        const struct vector * const,
                        const char * const);


void
print_block_matrix(FILE *, const struct block_matrix * const);


void
print_block_matrix_m(FILE *,
                     const struct block_matrix * const,
                     const char * const);


// This function is used for debug only, not optimized at all
void
print_block_matrix_full(FILE *,
                        const struct block_matrix * const);


// This function is used for debug only, not optimized at all
void
print_block_matrix_full_m(FILE *,
                          const struct block_matrix * const,
                          const char * const);

/* ----------------------------------------------------------- */

#endif // OUTPUT_H
