#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }

#define INRANGE(text, placeholder, min, max, var)                              \
  do                                                                           \
  {                                                                            \
    printf(text);                                                              \
    if (scanf("%"#placeholder, &var) != 1)                                     \
    {                                                                          \
      fprintf(stderr, "Failed to read data from buffer.\nExiting...\n");       \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  }                                                                            \
  while(var < min || var > max);

int main()
{
  int var;
  int min, max;
  GET(Int, min);
  GET(Int, max);
  if (errno == 5)
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  INRANGE("Enter number: ", d, min, max, var)
  return 0;
}
