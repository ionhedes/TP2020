#include <stdio.h>
#include "myStdlib.h"
#include <errno.h>
#include <math.h>

#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }


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

POINT_CONSTRUCTOR(float)
POINT_CONSTRUCTOR(int)

int main()
{
  Point_int pt_int;
  Point_float pt_float;

  GET(Int, pt_int.x);
  GET(Int, pt_int.y);
  GET(Float, pt_float.x);
  GET(Float, pt_float.y);

  if (errno == 5)
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
