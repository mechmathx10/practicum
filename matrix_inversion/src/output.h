#ifndef OUTPUT_H
#define OUTPUT_H

#include "datatypes.h"

#include <stdio.h>


void
print_vector(FILE *, struct vector *);


void
print_simple_matrix(FILE *, struct simple_matrix *);


void
print_extended_matrix(FILE *, struct simple_matrix *, struct vector *);


#endif
