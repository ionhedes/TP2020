#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"
#define MAX(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))
#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }

int main()
{
  int a, b, c;
  GET(Int, a)
  GET(Int, b)
  GET(Int, c)

  if (errno == 5)
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  printf("The maximum value is: %d;\n", MAX(a, b, c));
  return 0;
}
