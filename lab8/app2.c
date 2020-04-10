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

create_array_handler(float)

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

int main()
{
  unsigned array_size = 3;
  float * array = NULL;
  if ((array = allocVec(array_size, 7.2, -1.0, 0)) == NULL)
  {
    fprintf(stderr, "Creating array failed.\n");
    printf("Exiting...\n");
  }
  printArray_float(array, &array_size);

  // Free the memory and end execution
  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
