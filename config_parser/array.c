#include "array.h"

#include <stdio.h>
#include <string.h>


static
void
extend_array(struct array *arr)
{
  arr->capacity *= 2;
  arr->data = realloc(arr->data, arr->capacity * arr->elem_size);
}


void
print_int_array(struct array *int_arr)
{
  printf("Capacity = \t%u\nCount = \t%u\nEl_size = \t%u\n", int_arr->capacity,
                                                            int_arr->count,
                                                            int_arr->elem_size);
  for (size_t i = 0; i < int_arr->count; ++i)
    printf("%u:\t%d\n", i, *(int *)get_element(int_arr, i));
}


void
init_array(struct array *arr, size_t elem_size)
{
  
  arr->count = 0;
  arr->capacity = DEFAULT_CAPACITY;
  arr->elem_size = elem_size;
  arr->data = malloc(elem_size * DEFAULT_CAPACITY);
  if (! arr->data)
    {
      fprintf(stderr, "Memory allocation error!\n");
      exit(2);
    }
}


void
add_element(struct array *arr, void *elem_data)
{
  if (arr->count == arr->capacity)
    extend_array(arr);
  memcpy(arr->data + (arr->count * arr->elem_size), elem_data, arr->elem_size);
  ++arr->count;
}


void *
get_element(struct array *arr, size_t index)
{
  return arr->data + (index * arr->elem_size);
}


void
destroy_array(struct array *arr)
{
  free(arr->data);
}

