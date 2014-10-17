#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdio.h>

/* ----------------------------------------------------------- */

struct permutation
{
  int *values;
  int size;
};


/* makes identity permutation of a given size */
void
init_permutation(struct permutation *, const int);


void
print_permutation(FILE *, const struct permutation * const);


void
add_transposition(struct permutation *, int, int);


// result permutation's values must be allocated
void
get_inversed(const struct permutation * const, struct permutation *);


/* ----------------------------------------------------------- */

#endif // PERMUTATION_H
