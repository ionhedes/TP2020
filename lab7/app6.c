#include <stdio.h>
#include <assert.h>
#include "myStdlib.h"
#include "arrayHandler.h"

#define STRINGIFY_AUX(text) #text
#define STRINGIFY(text) STRINGIFY_AUX(text)

#if defined(DEBUG) || defined(_DEBUG)
  #define DEB(...) fprintf(stderr, __FILE__" ["STRINGIFY(__LINE__)"]: "__VA_ARGS__)
#else
  #define DEB(...)
#endif

create_array_handler(int)

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

int deleteCommonOccurences(int * array1, unsigned * array_size1, int * array2, unsigned * array_size2)
{
  DEB("deleteCommonOccurences() has started;\n");

  assert(array1 != NULL);
  assert(array2 != NULL);
  assert(*array_size1);
  assert(*array_size2);

  int i, j;
  int position;
  int * aux_pointer = NULL;

  for (i = 0; i < *array_size2; i++)
  {
    DEB("\t - looking for value %d inside the first array...\n", array2 [i]);
    position = isFound(&array2 [i], array1, array_size1);
    while (position != -1)
    {
      for (j = position; j < *array_size1 - 1; j++)
      {
        array1 [j] = array1 [j + 1];
      }
      (*array_size1)--;
      if ((aux_pointer = (int *)realloc(array1, (*array_size1) * sizeof(int))) == NULL &&
          *array_size1)
      {
        DEB("\t\t - failed to resize array;\n");
        return 0;
      }
      array1 = aux_pointer;
      DEB("\t\t - resizing of the array succesful;\n");

      position = isFound(&array2 [i], array1, array_size1);
    }
    DEB("\n");
  }
  return 1;
}

int main()
{
  int * array1 = NULL;
  int * array2 = NULL;
  unsigned array_size1;
  unsigned array_size2;

  printf("Enter the size of the first array: ");
  if (!get_unsigned(&array_size1, stdin))
  {
    DEB("Error when reading the size of the first array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  printf("Enter the size of the second array: ");
  if (!get_unsigned(&array_size2, stdin))
  {
    DEB("Error when reading the size of the second array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  if((array1 = createArray_int(&array_size1)) == NULL)
  {
    DEB("Error when making space for the first array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  if((array2 = createArray_int(&array_size2)) == NULL)
  {
    DEB("Error when making space for the second array.\n");
    free(array1);
    MALLOC_COUNTER--;
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  if (!fillArray_int(array1, &array_size1))
  {
    DEB("Error when filling the first array.\n");
    free(array1);
    MALLOC_COUNTER--;
    free(array2);
    MALLOC_COUNTER--;
  }

  if (!fillArray_int(array2, &array_size2))
  {
    DEB("Error when filling the second array.\n");
    free(array1);
    MALLOC_COUNTER--;
    free(array2);
    MALLOC_COUNTER--;
  }

  putchar('\n');
  printArray_int(array1, &array_size1);
  printArray_int(array2, &array_size2);
  putchar('\n');

  if (!deleteCommonOccurences(array1, &array_size1, array2, &array_size2))
  {
    fprintf(stderr, "Deleting common occurences between the arrays from the first array has failed.\n");
  }
  else
  {
    putchar('\n');
    printf("The size of the first array, after deleting common occurences between the arrays is %d;\n", array_size1);
    printf("The elements of the first array are now:\n");
    printArray_int(array1, &array_size1);
    putchar('\n');
  }


  free(array1);
  MALLOC_COUNTER--;
  free(array2);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
