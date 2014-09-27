#ifndef INPUT_H
#define INPUT_H

#include "output.h"
#include "datatypes.h"

#include <stdio.h>


enum error_type
read_vector(FILE *, struct vector *);


enum error_type
read_simple_matrix(FILE *, struct simple_matrix *);


enum error_type
read_square_matrix(FILE *, struct simple_matrix *);


/* only for square matrices for now */
enum error_type
read_extended_matrix(FILE *, struct simple_matrix *, struct vector *);


#endif // INPUT_H
