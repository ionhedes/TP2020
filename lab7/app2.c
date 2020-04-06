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

create_array_handler(double) // < Calling the array handler for a set of functions
                             // for double type arrays

// Function to determine the maximum value found in an array
/*
  USAGE:
    - returns the maximum value found inside a double array
    - the array must be double

  ARGUMENTS:
    - const double * array - pointer to the array
    - const unsigned * array_size - pointer referencing
      the array size variable

  RETURN VALUE:
    - the maximum value found in the array
    - this function is not supposed to fail, only if one of the assertions fails,
      case in which the execution is terminated with a proper message
*/
double maximumValue(const double * array, const unsigned * array_size)
{
  assert(array != NULL); // < The array must exist in the memory
  assert(*array_size > 0); // < There needs to be a non-null number of values

  int i;

  DEB_PRT_ARRAY(double, array, array_size); // < In debug mode, print the array

  double max = array [0]; // < Always start with considering the first value of
                          // array as its maximum one

  for (i = 1; i < *array_size; i++) // < Look for the actual maxval
  {
    if(max < array [i])
    {
      max = array [i];
    }
  }
  return max;
}

// Driver program for the previous function
int main()
{
  double * array = NULL; // < Pointer to be used as malloc() return value
  unsigned array_size; // < Size of the array

  // Get the size of the array
  printf("Enter the number of elements the array will have: ");
  if (!get_unsigned(&array_size, stdin)) // < myStdlib.h for details on i/o
  {
    DEB("Error when reading the number of array elements.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  // Initialize the array with the respective data
  // See arrayHandler.h for array operation descriptions
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

  // Call the function in testing
  printf("The maximum value found in this array is: %lf\n", maximumValue(array, &array_size));

  // Free the memory and end execution
  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
