/*
  Aplicația 7.3: Pentru exemplul 2, să se scrie în funcția vmax la început o
                 buclă care afișează conținutul vectorului v. Toată această
                 buclă va trebui să existe în program doar dacă este definit
                 DEBUG sau _DEBUG.
*/
#include <stdio.h>
#include <assert.h>
#include "myStdlib.h"
#include "arrayHandler.h"

#if defined(DEBUG) || defined(_DEBUG)
  #define DEB(...) fprintf(stderr, __VA_ARGS__)
#else
  #define NDEBUG 1 // < Switch for assert()
  #define DEB(...)
#endif

create_array_handler(double)

double maximumValue(const double * array, const unsigned * array_size)
{
  int i;
  assert(array != NULL);
  assert(*array_size > 0);

  DEB_PRT_ARRAY(double, array, array_size);

  double max = array [0];

  for (i = 1; i < *array_size; i++)
  {
    if(max < array [i])
    {
      max = array [i];
    }
  }
  return max;
}

int main()
{
  double * array = NULL;
  unsigned array_size;
  printf("Enter the number of elements the array will have: ");
  if (!get_unsigned(&array_size, stdin))
  {
    DEB("Error when reading the number of array elements.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((array = createArray_double(&array_size)) == NULL)
  {
    DEB("Error when allocating memory for the array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if (!fillArray_double(array, &array_size))
  {
    free(array);
    MALLOC_COUNTER--;
    DEB("Error when filling the array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  printf("The maximum value found in this array is: %lf\n", maximumValue(array, &array_size));

  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
