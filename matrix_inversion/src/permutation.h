#ifndef PERMUTATION_H
#define PERMUTATION_H

/* ----------------------------------------------------------- */

struct permutation
{
  int *values;
  int size;
};


void
add_transposition(struct permutation *, int, int);


void
get_inversed(struct permutation *, struct permutation *);


/* ----------------------------------------------------------- */

#endif
