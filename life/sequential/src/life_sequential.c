#include "life_sequential.h"
#include "../../headers/datatypes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ----------------------------------------------------------- */

enum error_type
init_field(FILE *input_stream, struct field *field, enum input_type in_type)
{
  int r;
  int height;
  int width;
  if (in_type == IT_CONSOLE)
    printf("Input field height and width: ");
  r = fscanf(input_stream, "%d %d", &height, &width);
  if (r < 2)
    {
      fprintf(stderr, "Cannot read field sizes!\n");
      return ET_INPUT_ERROR;
    }
  if (height < 1 || width < 1)
    {
      fprintf(stderr, "Field cannot have non-positive sizes!\n");
      return ET_INPUT_ERROR;
    }

  field->old_values = (int **) malloc ((height + 2) * sizeof(int *));
  field->new_values = (int **) malloc ((height + 2) * sizeof(int *));
  for (int i = 0; i < height + 2; ++i)
    {
      field->old_values[i] = (int *) malloc ((width + 2) * sizeof(int));
      field->new_values[i] = (int *) malloc ((width + 2) * sizeof(int));
      for (int j = 0; j < width + 2; ++j)
        {
          field->old_values[i][j] = DEAD;
          field->new_values[i][j] = DEAD;
        }
    }
  field->width = width;
  field->height = height;

  int alive_count;
  if (in_type == IT_CONSOLE)
    printf("Input start alive cells count: ");
  r = fscanf(input_stream, "%d", &alive_count);
  if (r < 1)
    {
      fprintf(stderr, "Cannot read cell count!\n");
      delete_field(field);
      return ET_INPUT_ERROR;
    }
  if (alive_count < 0)
    {
      fprintf(stderr, "Alive count cannot be negative!\n");
      delete_field(field);
      return ET_INPUT_ERROR;
    }
  field->alive_count = alive_count;

  int current_row;
  int current_column;
  if (in_type == IT_CONSOLE)
    printf("Note: top left corner is (1, 1) cell.\n");
  for (int i = 0; i < alive_count; ++i)
    {
      if (in_type == IT_CONSOLE)
        printf("Input cell #%d position: ", i);
      r = fscanf(input_stream, "%d %d", &current_row, &current_column);
      if (r < 2)
        {
          fprintf(stderr, "Cannot read cell position!\n");
          delete_field(field);
          return ET_INPUT_ERROR;
        }
      if (current_row < 1 || current_row > height ||
          current_column < 1 || current_column > width)
        {
          fprintf(stderr, "Wrong cell position!\n");
          delete_field(field);
          return ET_INPUT_ERROR;
        }
      field->old_values[current_row][current_column] = ALIVE;
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

static inline
char
cell_symbol(enum state state)
{
  if (state == DEAD)
    return ' ';
  return 'O'; // state == DEAD
}


enum error_type
print_field(FILE *output_stream, const struct field * const field)
{
  if (! output_stream)
    {
      fprintf(stderr, "Cannot print field: zero output stream pointer!\n");
      return ET_FILE_ERROR;
    }

  for (int i = 1; i < field->height + 1; ++i)
    {
      for (int j = 0; j < field->width + 1; ++j)
          fprintf(output_stream, "%c", cell_symbol(field->old_values[i][j]));
      fprintf(output_stream, "\n");
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
update_field(struct field *field)
{
  const int height = field->height;
  const int width = field->width;
  int neighbor_count;
  field->alive_count = 0;
  for (int i = 1; i < height + 1; ++i)
    {
      for (int j = 1; j < width + 1; ++j)
        {
          neighbor_count = 0;
          for (int k = i - 1; k < i + 2; ++k)
            {
              for (int l = j - 1; l < j + 2; ++l)
                {
                  if (i == k && j == l)
                    continue;
                  if (field->old_values[k][l] == ALIVE)
                    ++neighbor_count;
                }
            }
          if (field->old_values[i][j] == ALIVE)
            {
              if (neighbor_count == 2 || neighbor_count == 3)
                {
                  field->new_values[i][j] = ALIVE;
                  ++field->alive_count;
                }
              else
                field->new_values[i][j] = DEAD;
            }
          else // field->old_values[i][j] == DEAD
            {
              if (neighbor_count == 3)
                {
                  field->new_values[i][j] = ALIVE;
                  ++field->alive_count;
                }
              else
                field->new_values[i][j] = DEAD;
            }
        }
    }
  for (int i = 1; i < height + 1; ++i)
    {
      for (int j = 0; j < width + 1; ++j)
        {
          field->old_values[i][j] = field->new_values[i][j];
        }
    }
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */

enum error_type
delete_field(struct field *field)
{
  for (int i = 0; i < field->height + 2; ++i)
    free(field->old_values[i]);
  free(field->old_values);
  for (int i = 0; i < field->height + 2; ++i)
    free(field->new_values[i]);
  free(field->new_values);
  field->height = 0;
  field->width = 0;
  field->alive_count = 0;
  return ET_CORRECT;
}

/* ----------------------------------------------------------- */
