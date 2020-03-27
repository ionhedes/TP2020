#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"
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

#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }

#define ARRAY_ALLOC(pointer, size, type)                                       \
  {                                                                            \
    if ((pointer = (type *)malloc(size * sizeof(type))) == NULL)               \
    {                                                                          \
      fprintf(stderr, "Failed to create array.\nExiting...\n");                \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
    MALLOC_COUNTER++;                                                          \
  }

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

SORT(int)

int main()
{
  int * array = NULL;
  int array_size;
  GET(Int, array_size)
  if (errno == 5)
  {
    perror("Failed to read data from buffer.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  ARRAY_ALLOC(array, array_size, int)
  FILL_ARRAY(array, array_size, Int)
  PRINT_ARRAY(array, array_size, d)
  sort_int(array, array_size);
  PRINT_ARRAY(array, array_size, d)

  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nExiting...\n", MALLOC_COUNTER);
  return 0;
}
