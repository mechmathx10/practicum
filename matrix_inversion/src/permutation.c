#include "permutation.h"

#include <stdlib.h>

/* ----------------------------------------------------------- */

void
init_permutation(struct permutation *permutation, const int size)
{
  permutation->size = size;
  permutation->values = (int *) malloc(size * sizeof(double));
  for (int i = 0; i < size; ++i)
    permutation->values[i] = i;
}

/* ----------------------------------------------------------- */

void
print_permutation(FILE *output_stream,
                  const struct permutation * const permutation)
{
  for (int i = 0; i < permutation->size; ++i)
    fprintf(output_stream, "%d\t", i);
  fprintf(output_stream, "\n");
  for (int i = 0; i < permutation->size; ++i)
    fprintf(output_stream, "%d\t", permutation->values[i]);
  fprintf(output_stream, "\n");
}

/* ----------------------------------------------------------- */

void
add_transposition(struct permutation *permutation, int i, int j)
{
  int temp = permutation->values[i];
  permutation->values[i] = permutation->values[j];
  permutation->values[j] = temp;
}

/* ----------------------------------------------------------- */

void
get_inversed(const struct permutation * const permutation,
             struct permutation *inversed)
{
  for (int i = 0; i < permutation->size; ++i)
    inversed->values[permutation->values[i]] = i;
}

/* ----------------------------------------------------------- */
