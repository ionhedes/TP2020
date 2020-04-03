/*
  Aplicația 7.4: Pentru exemplul 2, să se scrie o funcție test care primește ca
                 parametri un vector de elemente de tip double, numărul de
                 elemente din vector și o valoare x de tip double.
                 Funcția va returna 1 dacă x este maximul elementelor din vector,
                 altfel 0. Folosind această funcție, să se scrie un assert în
                 main care să verifice că vmax returnează într-adevăr maximul
                 elementelor din vector.
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

int testMaximumValue(const double * array, const unsigned * array_size, const double * maximum_value)
{
  int i;
  for (i = 0; i < *array_size; i++)
  {
    if(array [i] > *maximum_value)
    {
      return 0;
    }
  }
  return 1;
}

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
  double max;
  double * array = NULL;
  unsigned array_size;
  printf("Enter the number of elements the array will have: ");
  if (!get_unsigned(&array_size, stdin))
  {
    fprintf(stderr, "Error when reading the number of array elements.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((array = createArray_double(&array_size)) == NULL)
  {
    fprintf(stderr, "Error when allocating memory for the array.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if (!fillArray_double(array, &array_size))
  {
    free(array);
    MALLOC_COUNTER--;
    fprintf(stderr, "Error when filling the array.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  max = maximumValue(array, &array_size);
  printf("The maximum value found in this array is: %lf\n", max);
  assert(testMaximumValue(array, &array_size, &max) == 1);

  free(array);
  MALLOC_COUNTER--;
  DEB("Execution ended.\nUnreleased memory blocks: %d;\nExiting...\n", MALLOC_COUNTER);
  return 0;
}
