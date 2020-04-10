/*
  Aplicația 8.8: Să se scrie o funcție sortare(int n,...) care pe prima poziție
                 are numărul de elemente de sortat, iar pe următoarele poziții
                 n adrese de valori de tip int. Funcția va sorta crescător
                 valorile de la adresele date.
  Exemplu: sortare(3,&i,&j,&k);			// sortează crescător valorile lui i, j și k
*/

#include <stdio.h>
#include <stdarg.h>
#include "myStdlib.h"

void insertionSort(int ** array, const unsigned * array_size)
{
  int key;
  int i = 1, j;
  while(*(array [i]) > *(array [i - 1]))
  {
    i++;
  }
  for (; i < *array_size; i++)
  {
    key = *(array [i]);
    DEB("The current key is %d (index %d).\n", key, i);
    for (j = i - 1; j >= 0; j--)
    {
      if (*(array [j]) >= key)
      {
        DEB("\t - found element to move: position %d, value %d.\n", j, *(array [j]));
        *(array [j + 1]) = *(array [j]);
      }
      else
      {
        break;
      }
    }
    *(array [j + 1]) = key;
  }
}

int sort(const unsigned n, ...)
{
  int i;
  va_list var_args;
  va_start(var_args, n);

  int ** array = NULL;
  if ((array = (int **)malloc(n * sizeof(int *))) == NULL)
  {
    DEB("Failed to allocate memory for the array of addresses.\n");
    return 0;
  }
  MALLOC_COUNTER++;

  for (i = 0; i < n; i++)
  {
    array [i] = va_arg(var_args, int *);
  }

  insertionSort(array, &n);

  free(array);
  MALLOC_COUNTER--;
  va_end(var_args);
  return 1;
}

int main()
{
  int x = 4, y = 3, z = 5;
  if (!sort(3, &x, &y, &z))
  {
    fprintf(stderr, "Failed to sort elements.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("The elements ordered:\n");
    printf("\t - %d, %d, %d;\n", x, y, z);
  }

  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
