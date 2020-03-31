/*
  Aplicația 6.5: Să se scrie un macro care generează o structură
                 Punct cu componentele {x,y} de un tip specificat în macro și
                 totodată o funcție care primește ca parametru o variabilă de
                 tipul Punct și returnează distanța de la punct la origine.
  Exemplu: PUNCT(float)  -> va genera structura Punct_float
                            și funcția ”float len_float(Punct_float pt)”
*/
#include <stdio.h>
#include "myStdlib.h"
#include <errno.h>
#include <math.h>

// Macro used to read a type value from the input
/**
  USAGE:
    - this macro makes use of the myStdlib header, head there for info on the
      getType() functions, and how they work
    - make sure the type value starts with an uppercase when calling this macro

  ARGUMENTS:
    - type - the type of the variable to be read
    - var - variable to store the read value in
          - you only need to pass the name of the variable, the macro applies
            the reference operator on its own

  WARNING:
    - this is a macro, so there is no return value per se;
    - it is the result of the evaluation of the expression to replace the
      defined term

  RETURN VALUE:
    - when a memory allocation or parsing error occurs, errno is set to EIO
      (error-input/output)(= 5), so these kind of errors can be treated
*/
#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }

// Macro to create a generic type point structure, and an associated function
// to calculate the distance from the origin to the point in a cartesian axis
//system
/*
  USAGE:
    - call this macro outside any functions
    - when called, generates a Point structure with type "type" fields, and a
      distance function for points with coordinates of type "type"
    - do not call this multiple times for the same data type

  ARGUMENTS:
    - type - the type for the fields inside the structure
*/
#define POINT_CONSTRUCTOR(type)                                                \
  typedef struct                                                               \
  {                                                                            \
    type x;                                                                    \
    type y;                                                                    \
  }Point_##type;                                                               \
  type dist_##type(Point_##type pt)                                            \
  {                                                                            \
      return (type)(sqrt(pow((double)pt.x, 2) + pow((double)pt.y, 2)));        \
  }

POINT_CONSTRUCTOR(float) // < Creating a point structure with float fields
                         // and its associated distance function
POINT_CONSTRUCTOR(int)   // < Creating a point structure with int fields
                         // and its associated distance function

// Driver program
int main()
{
  Point_int pt_int;
  Point_float pt_float;

  // Read values for the fields of 2 points
  // - 1 point int
  // - 1 point float
  GET(Int, pt_int.x);
  GET(Int, pt_int.y);
  GET(Float, pt_float.x);
  GET(Float, pt_float.y);

  if (errno == 5) // < Treat input errors
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  printf("The integer point:\n");
  printf("\t - x coordinate: %d;\n", pt_int.x);
  printf("\t - y coordinate: %d;\n", pt_int.y);
  printf("\t - distance from origin: %d;\n", dist_int(pt_int));
  putchar('\n');
  printf("The float point:\n");
  printf("\t - x coordinate: %f;\n", pt_float.x);
  printf("\t - y coordinate: %f;\n", pt_float.y);
  printf("\t - distance from origin: %f;\n", dist_float(pt_float));
  return 0;
}
