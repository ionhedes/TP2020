/////////////////////////////////////////////////
/// arrayHandler - library created for easing the work with different data type
///                arrays
///              - covers some basic operations like, allocating space for an array,
///                filling the array with data, or printing the data from the array;
/////////////////////////////////////////////////


// myStdlib.h should be included for this library to operate
#ifndef MYSTDLIB_ON
  #include "myStdlib.h"
#endif

// Some printing macros intended for debugging

// NOTE: At the moment calling them is no different than simply calling the
// printing method implemented in this library
#if defined(DEBUG) || defined(_DEBUG)
  #define DEB_PRT_ARRAY(type, array, array_size) printArray_##type(array, array_size)
#else
  #define DEB_PRT_ARRAY(type, array, array_size)
#endif

// Usual data type string placeholders macros, implemented for automating prints
#define PLACEHOLDER_int "%d"
#define PLACEHOLDER_float "%f"
#define PLACEHOLDER_double "%lf"
#define PLACEHOLDER(type) PLACEHOLDER_##type

// Main macro creating a template for array handling functions
/**
  USAGE:
    - call with the appropiate data type to create the necessary functions to
      work with arrays
    - for the handling of character arrays/strings, use the getString() method,
      defined in myStdlib.h (for input), or (f)printf (for output)

  ARGUMENTS:
    - type - one of the currently supported data types:
           - int
           - unsigned
           - float
           - double

  FUNCTION DESCRIPTIONS:
    - createArray_##type: - allocates memory for a give size array of type "type"
                          - argument * array_size is a pointer referencing the
                            array size variable
                          - uses malloc(), so you might call it a wrapper
                          - returns NULL on malloc() error, pointer to array on
                            success

    - fillArray_##type:   - reads appropiate data and fills the array with it
                          - gets a pointer to the array and a pointer referencing
                            the array size variable as arguments
                          - uses get_##type() functions, implemented in myStdlib.h
                          - returns 0 on error, and 1 on success

    - printArray_##type:  - prints the data from an array
                          - gets a pointer to the array and a pointer referencing
                            the array size variable as arguments
                          - nothing special about it, just a print function
                          - makes use of the placeholder macros defined at the
                            beginning of this file

*/
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
