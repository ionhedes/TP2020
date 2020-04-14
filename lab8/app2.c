/*
  Aplicația 8.2: Să se scrie o funcție float *allocVec(int n,...) care primește
                 pe prima poziție un număr de elemente iar apoi n elemente reale.
                 Funcția va aloca dinamic un vector de tip float în care va
                 depune toate elementele.
  Exemplu: allocVec(3,7.2,-1,0)		=> {7.2, -1, 0}

*/

#include <stdio.h>
#include <stdarg.h>
#include "arrayHandler.h"

// Creating default array handling functions for the float type
// See arrayHandler.h for more information regarding these macros
create_array_handler(float)

// Function to store the given arguments into an array
/**
  USAGE:
    - use this if you really want to complicate yourself while creating and
      filling a float type array
    - the function will dinamically allocate space for the array and will fill
      it

  ARGUMENTS:
    - unsigned array_size - the number of values to be stored inside the array
    - the rest of the arguments should be the values to be stored

  RETURN VALUE:
    - on success - pointer to the array
    - on failure - NULL

  ADDITIONAL INFORMATION:
    - because of the implicit conversions made by the va_arg function, floats
      will be automatically converted to double; that being said, a cast is
      needed before storing in the array
    - you need to pass it explicit float values (1.0, not 1), because otherwise,
      var_arg will interpret them as int
*/
float * allocVec(unsigned array_size, ...)
{
  int i;
  float * array = NULL;
  va_list var_args;

  va_start(var_args, array_size);
  if ((array = createArray_float(&array_size)) == NULL)
  {
    DEB("Failed to allocate array.\n");
    return NULL;
  }
  for (i = 0; i < array_size; i++)
  {
    array [i] = (float) va_arg(var_args, double);
  }
  va_end(var_args);
  return array;
}

// Driver program for the allocVec() function
int main()
{
  // Needed variables
  unsigned array_size = 3;
  float * array = NULL;

  // Run the allocVec() function with a set of data
  // It was not worth it to create another array just to use the function on it,
  // so I just called the function with a set of data
  if ((array = allocVec(array_size, 7.2, -1.0, 0)) == NULL)
  {
    fprintf(stderr, "Creating array failed.\n");
    printf("Exiting...\n");
  }

  // Call the printing function from arrayHandler.h
  printArray_float(array, &array_size);

  // Free the memory and end execution
  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
