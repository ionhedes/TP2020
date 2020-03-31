/**
  Aplicația 6.2: Să se scrie un macro care să genereze o funcție de sortare
                 pentru un tip de date dat. Funcția va primi ca argumente
                 un vector de elemente și dimensiunea sa și va sorta vectorul
                 în ordine crescătoare.
  Exemplu: FN_SORTARE(unsigned)   -> va genera o funcție de
                                    sortare pentru valori de tip unsigned
*/

#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

// Macro used to define type-specific sorting function from a predefined template
/**
  USAGE:
    - call this macro outside any functions
    - the template is based on the insertion sort algorithm
    - when called, generates a sorting function for arrays of type ”type”
    - do not call this multiple times for the same data type

  ARGUMENTS:
    - type - the type of the array the generated function will be able to sort

  WARNING:
    - this is a macro, so there is no return value per se;
    - especially in this case, where all this macro does is generate a
      function to be used afterwards

  EXPLANATION FOR THE SORTING ALGORITHM:
           - begin from index = 0
           - skip indexes for as long as the numbers are already sorted
           - for every unsorted value, find its place in the array, between two
             sorted values, using another iterator and insert it in that place
*/
#define SORT(type)                                                             \
  void sort_##type(type * array, const int array_size)                         \
  {                                                                            \
    printf("Sorting...\n");                                                    \
    int key;                                                                   \
    int i = 1, j;                                                              \
    while (array [i] > array [i - 1] && i < array_size)                        \
    {                                                                          \
      i++;                                                                     \
    }                                                                          \
    for (; i < array_size; i++)                                                \
    {                                                                          \
      key = array [i];                                                         \
      for (j = i - 1; array [j] >= key && j >= 0; j--)                         \
      {                                                                        \
        array [j + 1] = array [j];                                             \
      }                                                                        \
      array [j + 1] = key;                                                     \
    }                                                                          \
  }

  // Macro used to read a type value from the input
  /**
    USAGE:
      - this macro makes use of the myStdlib header, head there for info on the
        getType() functions, and how they work
      - make sure the type value starts with an uppercase when calling this macro

    ARGUMENTS:
      - type - the type of the variable to be read
      - var - variable to store the read value in
            - you only need to pass the name of the variable, the macro applies
              the reference operator on its own

    WARNING:
      - this is a macro, so there is no return value per se;
      - it is the result of the evaluation of the expression to replace the
        defined term

    RETURN VALUE:
      - when a memory allocation or parsing error occurs, errno is set to EIO
        (error-input/output)(= 5), so these kind of errors can be treated
  */
#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }

// Macro to act as wrapper for a malloc + the asociated error treating
#define ARRAY_ALLOC(pointer, size, type)                                       \
  {                                                                            \
    if ((pointer = (type *)malloc(size * sizeof(type))) == NULL)               \
    {                                                                          \
      fprintf(stderr, "Failed to create array.\nExiting...\n");                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
    MALLOC_COUNTER++;                                                          \
  }

// Macro to fill the array with data
/*
  USAGE:
    - after using malloc() on a pointer, call this macro to fill the block
      pointed by the address in the pointer
    - values to be stored are read with the GET macro
    - input/output errors are treated with code inside this macro
    - make sure the type value starts with an uppercase when calling this macro

  ARGUMENTS:
    - pointer - pointer to the memory address where the array is
    - size - number of elements in the array
    - type - type of the elements of the array
*/
#define FILL_ARRAY(pointer, size, type)                                        \
  {                                                                            \
    int i;                                                                     \
    for (i = 0; i < size; i++)                                                 \
    {                                                                          \
      GET(type, pointer [i])                                                   \
      if (errno == 5)                                                          \
      {                                                                        \
        perror("Failed to read data from buffer.\nExiting...\n");              \
        free(array);                                                           \
        MALLOC_COUNTER--;                                                      \
        exit(EXIT_FAILURE);                                                    \
      }                                                                        \
    }                                                                          \
  }

// Macro to print the elements of the array
/*
  ARGUMENTS:
    - pointer - pointer to the memory address where the array is
    - size - number of elements in the array
    - type - type of the elements of the array
*/
#define PRINT_ARRAY(pointer, size, placeholder)                                \
  {                                                                            \
    printf("The elements of the array are:\n\t");                              \
    int i;                                                                     \
    for (i = 0; i < size; i++)                                                 \
    {                                                                          \
      printf("%"#placeholder" ", *(pointer + i));                              \
    }                                                                          \
    printf(";\n");                                                             \
  }

// Generating a sorting function for int arrays, using the before-defined macro
SORT(int)

// Driver program
int main()
{
  int * array = NULL;
  int array_size;
  GET(Int, array_size) // < read the array size
  if (errno == 5) // < treat possible input errors
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  ARRAY_ALLOC(array, array_size, int) // < allocate space for the array
  FILL_ARRAY(array, array_size, Int) // < fill the memory block with data
  PRINT_ARRAY(array, array_size, d) // < print the array before sorting
  sort_int(array, array_size); // < sort the array
  PRINT_ARRAY(array, array_size, d) // < print the array after sorting

  free(array); // < clear memory after all the operations are finished
  MALLOC_COUNTER--;
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nExiting...\n", MALLOC_COUNTER);
  return 0;
}
