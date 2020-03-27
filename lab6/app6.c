#include <stdio.h>
#include "myStdlib.h"

#define ARRAY_ALLOC(pointer, size, type)                                       \
  {                                                                            \
    if ((pointer = (type *)malloc(size * sizeof(type))) == NULL)               \
    {                                                                          \
      fprintf(stderr, "Failed to create array.\nExiting...\n");                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
    MALLOC_COUNTER++;                                                          \
  }

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

CAST(int, float)

int main()
{
  float * array_float = NULL;
  int array_int [] = {1, 4, 5, 6, 10};
  int array_size = 5;

  array_float = castArray(array_int, array_size);
  PRINT_ARRAY(array_float, array_size, .2f);

  free(array_float);
  MALLOC_COUNTER--;
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nExiting...\n", MALLOC_COUNTER);
  return 0;
}
