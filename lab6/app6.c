/*
  Aplicația 6.6: Să se scrie un macro care primește ca argumente două tipuri,
                 tip_dst și tip_src. Macroul va genera o funcție care primește
                 ca argument un vector cu elemente de tipul tip_src și numărul
                 de elemente din vector. Funcția va aloca dinamic un nou vector
                 cu elemente de tipul tip_dst și va depune în el toate elementele
                 din vectorul inițial, convertite la tip_dst.
                 Funcția va returna vectorul alocat.
  Exemplu: V_CONV(int,float)       -> va genera funcția
                                      ”int *v_conv_int_float(float *v,int n)”
*/

#include <stdio.h>
#include "myStdlib.h"

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


// Macro to create a csat function between two data types
/*
  USAGE:
    - call this macro outside any functions
    - when called, generates a generic function that casts a src_type array to a newly
      allocated dest_type array
    - do not call this multiple times for the same data type

  ARGUMENTS:
    - src_type - the data type of the array to be cast
    - dest_type - the data type of the destination array

  EXPLANATION ABOUT CASTING FUNCTION:
    - firstly, using the malloc wrapper macro, memory is allocated for the
      destination array
    - secondly, one by one, elements are taken from the source array, cast and
      assigned to the same index location in the destination array
    - lastly, a pointer to the destination/cast array is returned
*/
#define CAST(src_type, dest_type)                                              \
  dest_type * castArray(const src_type src_array [], const int array_size)     \
  {                                                                            \
    int i;                                                                     \
    dest_type * dest_array = NULL;                                             \
                                                                               \
    ARRAY_ALLOC(dest_array, array_size, dest_type)                             \
                                                                               \
    for (i = 0; i < array_size; i++)                                           \
    {                                                                          \
      dest_array [i] = (dest_type)src_array [i];                               \
    }                                                                          \
                                                                               \
    return dest_array;                                                         \
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

// Generating a casting function for int arrays to float arrays,
// using the before-defined macro
CAST(int, float)

// Driver program
int main()
{
  float * array_float = NULL;
  int array_size = 5;
  int array_int [] = {1, 4, 5, 6, 10}; // < Was too lazy to read another array,
                                       // so I statically assigned some values
                                       // to one

  // My laziness can be cured by using macros available in app2.c, or by using
  // functions from myStdlib.h

  array_float = castArray(array_int, array_size); // < creating the cast array
                                                  // from the source array
  PRINT_ARRAY(array_float, array_size, .2f); // prints the produced array

  free(array_float); // < clear memory after all the operations are finished
  MALLOC_COUNTER--;
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nExiting...\n", MALLOC_COUNTER);
  return 0;
}
