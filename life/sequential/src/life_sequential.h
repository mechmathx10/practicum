#ifndef LIFE_SEQUENTIAL_H
#define LIFE_SEQUENTIAL_H

#include "../../headers/datatypes.h"

#include <stdio.h>

/* ----------------------------------------------------------- */

enum error_type
init_field(FILE *, struct field *, enum input_type);


enum error_type
print_field(FILE *, const struct field * const);


enum error_type
update_field(struct field *);


enum error_type
delete_field(struct field *);

/* ----------------------------------------------------------- */

#endif // LIFE_SEQUENTIAL_H
