/*
  Aplicația 8.7: Să se scrie o funcție int comune(int nVec,...) care primește
                 un număr de vectori cu valori de tip int și vectorii
                 propriu-ziși. Fiecare vector se dă prin 2 argumente: un pointer
                 la elementele sale și dimensiunea. Funcția va returna numărul
                 de elemente comune care se regăsesc în toți vectorii.
  Exemplu: comune(3,v1,2,v2,3,v3,3)
           => returnează 2 pentru v1={5,8}, v2={8,3,5}, v3={5,0,8}
*/

#include <stdio.h>
#include <stdarg.h>
#include "myStdlib.h"

typedef struct
{
  int * address;
  unsigned size;

}Array;

#define areEqualArrays(arr1, arr2) ((arr1).size == (arr2).size && (arr1).address == (arr2).address)

Array * allocArrayArray(unsigned * size)
{
  Array * arr_arr = NULL;
  if ((arr_arr = (Array *)malloc((*size) * sizeof(Array))) == NULL)
  {
    DEB("Failed to create an array of Array structures.\n");
    return NULL;
  }
  MALLOC_COUNTER++;

  return arr_arr;
}

// Function to return the position of the first occurence of a number inside an
// array, if it exists
/*
  ARGUMENTS:
    - const int * value - pointer referencing the value to be found inside the
                          array
    - int * array - pointer to the array
    - unsigned * array_size - pointer referencing the array size variable

  RETURN VALUE:
    - any other positive value - the position of the first occurence of the value
                                 inside the array
    - -1 - the value hasn't been found
*/
int isFound(const int * value, int * array, unsigned * array_size)
{
  int i;
  for (i = 0; i < *array_size; i++)
  {
    if (array [i] == *value)
    {
      DEB("\t\t - found value %d at position %d;\n", *value, i);
      return i;
    }
  }
  return -1;
}

void wipeDuplicates(Array * arr)
{
  int i, j;
  for (i = 0; i < arr->size; i++)
  {
    for (j = i + 1; j < arr->size; j++)
    {
      if (arr->address [i] == arr->address [j])
      {
        DEB("\t\t - elements indexed with %d and %d have the same value (%d = %d).\n", i, j, arr->address [i], arr->address [j]);
        arr->address [arr->size - 1] = arr->address [arr->size - 1] ^ arr->address [j];
        arr->address [j] = arr->address [j] ^ arr->address [arr->size - 1];
        arr->address [arr->size - 1] = arr->address [arr->size - 1] ^ arr->address [j];
        arr->size--;
        j--;
      }
    }
  }
}

int findCommonElements(unsigned array_num, ...)
{
  int i, j;
  int common_elements_counter;
  Array minimum_size_array;
  Array * arr_arr = NULL;
  va_list var_args;
  va_start(var_args, array_num);

  if ((arr_arr = allocArrayArray(&array_num)) == NULL)
  {
    return -1;
  }


  DEB("Finding shortest array...\n");
  DEB("Keep in mind that duplicate elements inside the same array must be ignored...\n");
  for (i = 0; i < array_num; i++)
  {
    arr_arr [i].address = va_arg(var_args, int *);
    arr_arr [i].size = va_arg(var_args, unsigned);

    DEB("\t - checking for duplicates inside array %d\n", i);
    wipeDuplicates(&arr_arr [i]);
    if (i == 0)
    {
      minimum_size_array = arr_arr [i];
    }
    else if (arr_arr [i].size < minimum_size_array.size)
    {
      minimum_size_array = arr_arr [i];
    }

  }



  DEB("The maximum numbers of common elements between the %d arrays is the size of the smallest among them: %d.\n\n", array_num, minimum_size_array.size);
  common_elements_counter = minimum_size_array.size;

  DEB("Finding values from the shortest array not found in the others...\n");
  for (i = 0; i < minimum_size_array.size; i++)
  {
    DEB("\t - testing for value %d.\n", minimum_size_array.address [i]);
    for (j = 0; j < array_num; j++)
    {
      if (areEqualArrays(minimum_size_array, arr_arr [j]))
      {
        continue;
      }
      DEB("\t\t - in array %d.\n", j);
      if (isFound(&minimum_size_array.address [i], arr_arr [j].address, &arr_arr [j].size) == -1)
      {
        DEB("\t\t - element %d was not found inside array %d.\n", minimum_size_array.address [i], j);
        common_elements_counter--;
        break;
      }
    }
  }
  putchar('\n');

  free(arr_arr);
  MALLOC_COUNTER--;
  va_end(var_args);
  return common_elements_counter;
}

int main()
{
  int v1 [] = {5, 1, 3, 4, 4, 2, 2, 2};
  int v2 [] = {8, 2, 5};
  int v3 [] = {5, 0, 2};
  int return_value;


  return_value = findCommonElements(3, v1, 8, v2, 3, v3, 3);
  if (return_value == -1)
  {
    fprintf(stderr, "Failed to find the number of common occurences between the arrays.\n");
  }
  else
  {
    printf("The common number of occurences between the arrays is: %d.\n", return_value);
  }

  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
