#include <stdio.h>
#include <assert.h>
#include "myStdlib.h"
#include "arrayHandler.h"

create_array_handler(int) // < Calling the array handler for a set of functions
                          // for double type arrays

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

// Delete the elements of array2 from array1
/*
  RESTRICTIONS:
    - both the vectors must exist, their pointers shouldn't be empty (==NULL)
    - there must initially be at least one value in each vector (size > 0)

  USAGE:
    - all the elements from array1, which have been found by isFound() inside
      array2 must be deleted from the former
    - when an element is deleted, following elements are shifted back one position,
      effectively deleting it; the first array is resized with realloc() after,
      and its size adjusted

  ARGUMENTS:
    - int * array1 - the array from which elements must be deleted
    - unsigned * array_size1 - pointer referencing the size of the first array
    - int * array2 - the array containing elements to be deleted from array1
    - unsigned * array_size2 - pointer referencing the size of the second array

  RETURN VALUE:
    - 0 - error when executing function, probably when using realloc()
    - 1 - success
*/
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

  for (i = 0; i < *array_size2; i++) // < For every element inside the second array,
                                     // loop the first array to find its occurences
  {
    DEB("\t - looking for value %d inside the first array...\n", array2 [i]);

    // Find the first occurence of array2[i] inside array1
    position = isFound(&array2 [i], array1, array_size1);
    while (position != -1)
    {
      // Loop for repositioning elements and deletion of marked value
      for (j = position; j < *array_size1 - 1; j++)
      {
        array1 [j] = array1 [j + 1];
      }
      (*array_size1)--; // < Adjustment of the array size

      // Array shrinking
      if ((aux_pointer = (int *)realloc(array1, (*array_size1) * sizeof(int))) == NULL &&
          *array_size1)
      {
        DEB("\t\t - failed to resize array;\n"); // < Treating the shrinking error
        return 0;
      }
      array1 = aux_pointer;
      DEB("\t\t - resizing of the array succesful;\n");

      // Find subsequent occurences of array2[i] inside array1
      position = isFound(&array2 [i], array1, array_size1);
    }
    DEB("\n");
  }
  return 1;
}

// Driver program for the previous function
int main()
{
  // Variables to point to/hold the arrays and sizes needed
  int * array1 = NULL;
  int * array2 = NULL;
  unsigned array_size1;
  unsigned array_size2;

  // Get the size of the first array
  printf("Enter the size of the first array: ");
  if (!get_unsigned(&array_size1, stdin)) // < myStdlib.h for details on i/o
  {
    DEB("Error when reading the size of the first array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  // Get the size of the second array
  printf("Enter the size of the second array: ");
  if (!get_unsigned(&array_size2, stdin)) // < myStdlib.h for details on i/o
  {
    DEB("Error when reading the size of the second array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  // Initialize the arrays with their respective data
  // See arrayHandler.h for array operation descriptions
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

  // Print the arrays before deleting common occurences from the first one
  putchar('\n');
  printArray_int(array1, &array_size1);
  printArray_int(array2, &array_size2);
  putchar('\n');

  // Delete the common occurences
  if (!deleteCommonOccurences(array1, &array_size1, array2, &array_size2))
  {
    // Treat errors regarding the failure of the tested function
    fprintf(stderr, "Deleting common occurences between the arrays from the first array has failed.\n");
  }
  else
  {
    // If the operation is succesful, we print the new size of the first array,
    // as well as the remaining elements inside the array
    putchar('\n');
    printf("The size of the first array, after deleting common occurences between the arrays is %d;\n", array_size1);
    printf("The elements of the first array are now:\n");
    printArray_int(array1, &array_size1);
    putchar('\n');
  }

  // Free the memory and end execution
  free(array1);
  MALLOC_COUNTER--;
  free(array2);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
