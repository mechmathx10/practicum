#ifndef INPUT_H
#define INPUT_H

#include "datatypes.h"

#include <stdio.h>

/* ----------------------------------------------------------- */

enum error_type
read_vector(FILE *, struct vector *, enum stream_type);


enum error_type
read_simple_matrix(FILE *, struct simple_matrix *, enum stream_type);


enum error_type
read_square_matrix(FILE *, struct simple_matrix *, enum stream_type);


/* only for square matrices for now */
enum error_type
read_extended_matrix(FILE *,
                     struct simple_matrix *,
                     struct vector *,
                     enum stream_type);


// here we assume that block_matrix.block_size is already set
enum error_type
read_block_matrix(FILE *, struct block_matrix *, enum stream_type);

/* ----------------------------------------------------------- */

#endif // INPUT_H
