#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#define DEFAULT_CAPACITY 10

/* now array does not support element deletion. it's completely ok, because
we use arrays to store children lists in trees' nodes. we never delete a 
tree node wo deleting the whole tree. */

struct array
{
  void *data;
  size_t elem_size;
  size_t capacity;
  size_t count;
};


// for debug only
void
print_int_array(struct array *);


void
init_array(struct array *, size_t);


void
add_element(struct array *, void *);


void *
get_element(struct array *, size_t);


void
destroy_array(struct array *);


#endif