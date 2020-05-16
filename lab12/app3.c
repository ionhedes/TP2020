/**
  Aplicația 12.3: Definim reducerea unei matrici cu elemente întregi ca fiind
                  valoarea calculată astfel: dacă matricea nu are niciun element,
                  reducerea este 0. Dacă matricea are un element, reducerea este
                  valoarea acelui element. Pentru orice alte cazuri, matricea
                  se subîmparte în 4 matrici prin tăierea ei în cruce, prin
                  mijlocul matricii. Reducerea va fi maximul reducerilor celor
                  două matrici superioare, minus minimul reducerilor celor două
                  matrici inferioare. Să se calculeze reducerea unei matrici de
                  dimensiuni m,n citită de la tastatură.
*/

#include "dependencies/utils.h"
#include <math.h>

// Macros for computing minimum and maximum of two values;
/**
  USAGE:
    - use them for finding the max/min of two values;
    - has protection to double evaluation of expressions whose returned values
      will be compared;

  ARGUMENTS:
    - x, y, two comparable values;

*/
#define MAX(x, y)         \
  ({                      \
    typeof (x) _x = (x);  \
    typeof (y) _y = (y);  \
    (_x > _y) ? (_x) : (_y); \
  })

#define MIN(x, y)         \
  ({                      \
    typeof (x) _x = (x);  \
    typeof (y) _y = (y);  \
    (_x < _y) ? (_x) : (_y); \
  })

int mat [100][100]; // < Matrix to be reduced

// Function to compute the reduction value of a matrix
/**
  - based on the Divide et Impera paradigm: the matrix is recursively split into
    submatrices, whose reduction value will be calculated first, slowly building
    the desired result from smaller results;
  - every function call will have its own submatrix;

  USAGE:
    - computes the reduction value of a matrix, as described in the problem's text;


  ARGUMENTS:
    - unsigned starting_col - the starting column of the current submatrix,
                              relative to the initial matrix;
    - unsigned starting_line - the starting line of the current submatrix,
                               relative to the initial matrix;
    - unsigned col_num - the number of columns of the submatrix;
    - unsigned line_num - the number of lines of the submatrix;
    - int level - intended for debugging and screen printing in a meaningful
                  way - for every 'deeper' recursive call, a tab is printed
                  before debug messages;

  RETURN VALUE:
    - each call returns the reduction value of its assigned submatrix;
    - it will return the reduction value of the original matrix to its original
      caller;

  SIDE EFFECTS:
    - no outside variables will be affected by this function;
    - calling this function will produce a lot of messages, because I thought
      it is more important to see the steps it took, rather than only seeing the
      result of the reduction: displayed messages will be indented based on
      the 'depth' of the recursive call;
*/
int reduceMatrix(unsigned starting_col, unsigned starting_line, unsigned col_num, unsigned line_num, int level)
{
  // Printing tabs for clarity of displayed messages;
  unsigned level_cnt = 0;
  while(level_cnt++ < level)
  {
    printf("\t");
  }
  printf(" - current submatrix dimensions: [col. num: %d; line num: %d];\n", col_num, line_num);
  level_cnt = 0;

  // The submatrix has zero elements -> reduction value = 0;
  if (col_num == 0 || line_num == 0)
  {
    return 0;
  }
  // The submatrix has 1 element -> reduction value = that element;
  else if (col_num == 1 && line_num == 1)
  {
    // Printing tabs for clarity of displayed messages;
    while(level_cnt++ < level)
    {
      printf("\t");
    }
    printf("\t - returned element: %d;\n", mat [starting_line][starting_col]);
    return mat [starting_line][starting_col];
  }
  // The submatrix has more elements -> splitting it in four;
  // The reduction value of the split submatrix is calculated like this:
  // max(upper subm. 1, upper subm. 2) - min(lower subm. 1, lower subm. 2);
  else
  {
    // Printing tabs for clarity of displayed messages;
    while(level_cnt++ < level)
    {
      printf("\t");
    }
    printf("\t - current starting element: %d;\n", mat [starting_line][starting_col]);

    int term1;
    int term2;

    // For every submatrix, a different function call
    term1 = MAX(
      reduceMatrix(starting_col, starting_line, col_num - col_num / 2, line_num - line_num / 2, level + 1),
      reduceMatrix(starting_col + col_num - col_num / 2, starting_line, col_num / 2, line_num - line_num / 2, level + 1));

    term2 = MIN(
      reduceMatrix(starting_col, starting_line + line_num - line_num / 2, col_num - col_num / 2, line_num / 2, level + 1),
      reduceMatrix(starting_col + col_num - col_num / 2, starting_line + line_num - line_num / 2, col_num / 2, line_num / 2, level + 1));

    // Printing tabs for clarity of displayed messages;
    level_cnt = 0;
    while(level_cnt++ < level)
    {
      printf("\t");
    }
    printf("\t - returned point: %d - %d = %d;\n", term1, term2, term1 - term2);

    return term1 - term2;
  }
}

// Function to fill the matrix with values;
// Further comments for this method are out of scope;
int fillMatrix(const unsigned * col_num, const unsigned * line_num)
{
  int i, j;

  printf("Filling up the matrix...\n");
  for (i = 0; i < *line_num; i++)
  {
    printf("\t - line %d:\n", i);
    for (j = 0; j < *col_num; j++)
    {
      printf("\t\t - column %d: ", j);
      if (!get_int(&(mat [i][j]), stdin))
      {
        DEB("\t - failed to get one element of the matrix;\n");
        return 0;
      }
    }
  }
  return 1;
}

// Driver program for the reduction function;
// I/O handled by functions defined in utils.h;
int main()
{
  unsigned col_num;
  unsigned line_num;
  int return_value;

  // Getting the dimensions of the matrix;
  printf("Enter the number of lines of the matrix: ");
  if (!get_unsigned(&line_num, stdin))
  {
    err("I/O error when reading the number of lines of the matrix.\n");
  }
  printf("Enter the number of columns of the matrix: ");
  if (!get_unsigned(&col_num, stdin))
  {
    err("I/O error when reading the number of columns of the matrix.\n");
  }
  // Getting the elements of the matrix and filling it;
  if (!fillMatrix(&col_num, &line_num))
  {
    err("I/O error when filling up the matrix.\n");
  }

  // Finding the reduction value of the matrix;
  putchar('\n');
  printf("Finding the reduction of the matrix...\n");
  return_value = reduceMatrix(0, 0, col_num, line_num, 0);
  printf("The reduction of the matrix is: %d.\n", return_value);

  printf("Execution ended.\nExiting...\n");
  return 0;
}
