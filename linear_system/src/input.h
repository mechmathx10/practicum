#ifndef INPUT_H
#define INPUT_H

#include "output.h"
#include "datatypes.h"

#include <stdio.h>


int read_vector(FILE *, struct vector *);
int read_matrix(FILE *, struct simple_matrix *);


#endif // INPUT_H
