#ifndef DATATYPES_H
#define DATATYPES_H

#define UNUSED(PARAM) (void) (PARAM)

#define DIV_UP(A, B) (A / B) + ((A) % (B) == 0 ? 0 : 1)

#define DELETE(X) free((X).values)

#define SQUARE_DOUB(A) (A) * (A) * sizeof(double)

/* ----------------------------------------------------------- */

enum error_type
{
  ET_CORRECT = 0,
  ET_ERROR = 1,
  ET_INPUT_ERROR = 2,
  ET_ARG_ERROR = 3,
  ET_FILE_ERROR = 4,
  ET_SINGULAR = 5,
};


enum input_type
{
  IT_FILE = 0,
  IT_CONSOLE = 1,
  IT_GENERATE = 2,
};


enum output_type
{
  OT_FILE = 0,
  OT_CONSOLE = 1,
};



struct vector
{
  double *values;
  int size;
};


struct simple_matrix
{
  double *values;
  int height;
  int width;
};


// Correct initialization ways:
//   make_zero_block_matrix (matrix_utils)
//   make_unit_block_matrix (matrix_utils)
//   read_block_matrix      (input)
//   generate_block_matrix  (input)
struct block_matrix
{
  double *values;
  int size;
  int block_size;
  int full_block_count;
  int residue;
};


typedef struct simple_matrix block;

/* ----------------------------------------------------------- */

#endif // DATATYPES_H
