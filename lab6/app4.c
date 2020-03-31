/*
  Aplicația 6.4: Să se scrie un macro care primește ca argumente un text,
                 un placeholder pentru print/scanf, un nume de variabilă și
                 2 valori, min și max. Macroul va trebui să ceară de la
                 tastatură o valoare în mod repetat, până când ea se încadrează
                 în intervalul închis dat.
  Exemplu: CITIRE(”x=”,g,x,0,100)      -> citește variabila x până când
                                          valoarea citită se încadrează în [0,100]
*/

#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

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

// Macro to read values until one is in range [min, max]
/*
  USAGE:
    - a number must be read inside var before calling this macro
    - all subsequent reads made by the code inside this macro are made with scanf;
      this was the best option when taking into consideration the restrictions
      put on the argument lsit of the macro
    - if scanf() fails, we got it covered
    - reads will be made until the value inside var is in range [min, max]

  ARGUMENTS:
    - text - string to be printed by the code inside the macro
    - placeholder - used for reading different data types with scanf
    - min, max - limits of the range where var should be
    - var - the value to be compared against the limits of the interval
*/
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

// Driver program
int main()
{
  int var;
  int min, max;
  GET(Int, min) // < read the value for the lower limit of the inverval
  GET(Int, max) // < read the value for the upper limit of the interval
  if (errno == 5) // < treat input errors
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  INRANGE("Enter number: ", d, min, max, var) // < call for the macro
  return 0;
}
