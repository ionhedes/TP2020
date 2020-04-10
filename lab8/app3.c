/*
  Aplicația 8.3: Să se scrie o funcție absN(int n,...) care primește un număr n
                 de adrese de tip float și setează la fiecare dintre aceste
                 adrese valoarea absolută de la acea locație.
  Exemplu: absN(2,&x,&y);		// echivalent cu x=fabs(x); y=fabs(y);

*/
#include <stdio.h>
#include <stdarg.h>
#include "arrayHandler.h"

#define abs(x) ((x) >= 0 ? (x) : (-x))
#define MINIMUM_SIZE 3
create_array_handler(float)

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

int main()
{
  unsigned array_size;
  float * array = NULL;
  printf("Enter the size of the array (size > %d): ", MINIMUM_SIZE);
  do
  {
    if (!get_unsigned(&array_size, stdin))
    {
      DEB("Error when reading the array size.\n");
      printf("Exiting...\n");
    }
  }
  while (array_size < MINIMUM_SIZE);

  if ((array = createArray_float(&array_size)) == NULL)
  {
    DEB("Error when making space for the array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
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
  absN(3, array, array + 1, array + 2);
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
