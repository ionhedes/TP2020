#ifndef MYSTDLIB_ON
  #include "myStdlib.h"
#endif

#if defined(DEBUG) || defined(_DEBUG)
  #define DEB_PRT_ARRAY(type, array, array_size) printArray_##type(array, array_size)
#else
  #define DEB_PRT_ARRAY(type, array, array_size)
#endif

#define PLACEHOLDER_int "%d"
#define PLACEHOLDER_float "%f"
#define PLACEHOLDER_double "%lf"
#define PLACEHOLDER(type) PLACEHOLDER_##type

#define create_array_handler(type)                                             \
  type * createArray_##type(const unsigned * array_size)                       \
  {                                                                            \
    type * array = NULL;                                                       \
    if ((array = (type *)malloc(*array_size * sizeof(type))) == NULL)          \
    {                                                                          \
      return NULL;                                                             \
    }                                                                          \
    MALLOC_COUNTER++;                                                          \
    return array;                                                              \
  }                                                                            \
                                                                               \
  int fillArray_##type(type * array, const unsigned * array_size)              \
  {                                                                            \
    int i;                                                                     \
    printf("Enter the elements of the array:\n");                              \
    for (i = 0; i < *array_size; i++)                                          \
    {                                                                          \
      printf("\t - arr [%d]: ", i);                                            \
      if (!get_##type(&array [i] , stdin))                                     \
      {                                                                        \
        return 0;                                                              \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  void printArray_##type(const type * array, const unsigned * array_size)      \
  {                                                                            \
    int i;                                                                     \
    printf("The elements of the array:\n");                                    \
    printf("\t - ");                                                           \
    for (i = 0; i < *array_size; i++)                                          \
    {                                                                          \
      printf(""PLACEHOLDER(type)", ", array [i]);                              \
    }                                                                          \
    putchar('\n');                                                             \
  }
