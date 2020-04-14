/*
  Aplicația 8.3: Să se scrie o funcție absN(int n,...) care primește un număr n
                 de adrese de tip float și setează la fiecare dintre aceste
                 adrese valoarea absolută de la acea locație.
  Exemplu: absN(2,&x,&y);		// echivalent cu x=fabs(x); y=fabs(y);

*/
#include <stdio.h>
#include <stdarg.h>
#include "arrayHandler.h"

// Macro the calculate the absolute number of a number
#define abs(x) ((x) >= 0 ? (x) : (-x))

// Macro to define the minimum allowed size of an array
#define MINIMUM_SIZE 3

// Creating default array handling functions for the float type
// See arrayHandler.h for more information regarding these macros
create_array_handler(float)

// Function to compute the absolute value for a variable number of arguments
/**
  USAGE:
    - use it to compute the absolute values of the numbers found at certain
      addresses
    - all initially negative values are lost upon calling this function

  ARGUMENTS:
    - unsigned n - number of addresse found in the variable arguments
    - all following arguments must be pointers to values of type int, unsigned,
      float, double

  ADDITIONAL INFORMATION:
    - because of the implicit conversions made by the va_arg function, floats
      will be automatically converted to double; that being said, a cast is
      needed before storing in the array
    - you need to pass it explicit float values (1.0, not 1), because otherwise,
      var_arg will interpret them as int
*/
void absN(unsigned n, ...)
{
  int i;
  float * addr = NULL;
  va_list var_args;
  va_start(var_args, n);

  for (i = 0; i < n; i++)
  {
    addr = (float *) va_arg(var_args, double *);
    *addr = abs(*addr);
  }

  va_end(var_args);
}

// Driver program for the absN() function
int main()
{
  // Needed variables
  unsigned array_size;

  // For practical purposes, I have chosen to use an array, whose addresses I
  // will pass to absN
  float * array = NULL;

  // Read values for the array size until you find one that is valid
  printf("Enter the size of the array (size > %d): ", MINIMUM_SIZE);
  do
  {
    // For details on get_unsigned() and DEB(), check myStdlib.h
    if (!get_unsigned(&array_size, stdin))
    {
      DEB("Error when reading the array size.\n");
      printf("Exiting...\n");
    }
  }
  while (array_size < MINIMUM_SIZE);

  // Create a float array
  // For details on createArray_float() and fillArray_float(),
  // check arrayHandler.h
  if ((array = createArray_float(&array_size)) == NULL)
  {
    DEB("Error when making space for the array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  // Fill the array with random numbers
  if (!fillArray_float(array, &array_size))
  {
    DEB("Error when filling the array.\n");
    free(array);
    MALLOC_COUNTER--;
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  putchar('\n');
  printf("The array before manipulation:\n");
  printArray_float(array, &array_size);

  putchar('\n');
  absN(3, array, array + 1, array + 2); // < Only using the first 3 addresses,
                                        // so the rest of the array will invariable
                                        // remain unchanged
  printf("The array after manipulation:\n");
  printArray_float(array, &array_size);
  putchar('\n');

  // Free the memory and end execution
  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
