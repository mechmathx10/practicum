#ifndef GENERATOR_H
#define GENERATOR_H

#include "datatypes.h"

/* ----------------------------------------------------------- */

enum error_type
generate_matrix(const char * const, const int, const int,
                double (*f)(const double, const double));

/* ----------------------------------------------------------- */

#endif // GENERATOR_H
