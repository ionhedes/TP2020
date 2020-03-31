/**
  Aplicația 6.1: Să se scrie un macro care returnează maximul a 3 argumente.
*/

#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

// Macro used to return the maximum value between 3 values
/**
  USAGE:
    - use it inside a function
    - semicolon after the usage is optional

  ARGUMENTS:
    - a, b, c - the values to compare

  WARNING:
    - this is a macro, so there is no return value per se;
    - it is the result of the evaluation of the expression to replace the
      defined term
*/
#define MAX(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

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

// Driver program for the before-defined macros
int main()
{
  int a, b, c;

  // Read the values stdin
  // Observe that semicolons are not necesarry here, as the macro is an instruction
  // block, encapsulated in curly brackets
  GET(Int, a)
  GET(Int, b)
  GET(Int, c)

  // Treat reading errors
  if (errno == 5)
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  // Print the maximum value
  printf("The maximum value is: %d;\n", MAX(a, b, c));
  return 0;
}
