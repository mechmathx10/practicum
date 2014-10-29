#ifndef INPUT_H
#define INPUT_H

#include "datatypes.h"

#include <stdio.h>

/* ----------------------------------------------------------- */


enum error_type
read_values(FILE *, double *, int, enum input_type);


enum error_type
read_vector(FILE *, struct vector *, enum input_type);


enum error_type
read_simple_matrix(FILE *, struct simple_matrix *, enum input_type);


enum error_type
read_square_matrix(FILE *, struct simple_matrix *, enum input_type);


/* only for square matrices for now */
enum error_type
read_extended_matrix(FILE *,
                     struct simple_matrix *,
                     struct vector *,
                     enum input_type);

/* ----------------------------------------------------------- */

#endif // INPUT_H
