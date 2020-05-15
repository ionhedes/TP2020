/////////////////////////////////////////////////
/// utils    - library created so I can get a better grasp of error handling and
///            how memory and reading/writing data are linked
///          - also it was bothersome to rewrite some methods that I would use
///            in every program
/////////////////////////////////////////////////

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// Macro constant to be used in include guards in other programs
// It being defined means this library is included
#define UTILS_ON 1

// The following global variables are used for debugging
// They keep track of malloc()/free() and fopen()/fclose() calls
// Sadly, I could not find a better way of managing these
extern unsigned MALLOC_COUNTER;
extern unsigned OPENED_FILES_COUNTER;

// Macros used to convert different text to strings in the compilation phase
#define STRINGIFY_AUX(text) #text
#define STRINGIFY(text) STRINGIFY_AUX(text)

// Debug intenteded macros
/**
  DEB - prints error messages to stderr, only in debug mode, mentioning the
        file and line that produces an error
*/
#if defined(DEBUG) || defined(_DEBUG)
  #define DEB(...) fprintf(stderr, __FILE__" ["STRINGIFY(__LINE__)"]: "__VA_ARGS__)
#else
  #define DEB(...)
#endif

// Functionality intended macros
/*
  USAGE:
    - used internally by the functions inside this library
    - they simplify the external calls needed to be made to the internal functions
    - e.g.: without these, you would need to know which validation and conversion
            functions to use for each data type; with them, the argument lists of
            these functions is skinned down, because the functions know what other
            functions to call for validation and conversion
*/
#define VALIDATION_FUNC_int isValidInt
#define VALIDATION_FUNC_unsigned isValidUnsigned
#define VALIDATION_FUNC_float isValidFloat
#define VALIDATION_FUNC_double isValidDouble
#define VALIDATION_FUNC(type) VALIDATION_FUNC_##type

#define CONVERSION_FUNC_int atoi
#define CONVERSION_FUNC_unsigned atoi
#define CONVERSION_FUNC_float atof
#define CONVERSION_FUNC_double atof
#define CONVERSION_FUNC(type) CONVERSION_FUNC_##type

#define generate_input_function(type)                                                                       \
  int get_##type(type * location, FILE * stream)                                                            \
  {                                                                                                         \
    char * aux_element = NULL; /* < buffer string spanning only this function */                            \
                                                                                                            \
    /*Calls getString() to get new strings until the validation function confirms                           \
   that the string represents a valid integer, or an error occurs*/                                         \
    if ((aux_element = getString(stream)) == NULL)                                                          \
    {                                                                                                       \
      return 0;                                                                                             \
    }                                                                                                       \
    if (!VALIDATION_FUNC(type)(aux_element))                                                                \
    {                                                                                                       \
      do                                                                                                    \
      {                                                                                                     \
        free(aux_element);                                                                                  \
        MALLOC_COUNTER--;                                                                                   \
        printf("\t ! Enter a valid value: ");                                                               \
        if ((aux_element = getString(stream)) == NULL)                                                      \
        {                                                                                                   \
          return 0;                                                                                         \
        }                                                                                                   \
      }                                                                                                     \
      while (!VALIDATION_FUNC(type)(aux_element));                                                          \
    }                                                                                                       \
    /* When a valid int is eventually given, cast it and store it in the desired                            \
    location*/                                                                                              \
    *location = CONVERSION_FUNC(type)(aux_element);                                                         \
                                                                                                            \
    /* Free the string generated by getString(), now that the int is safely placed                          \
    in its desired location*/                                                                               \
    free(aux_element);                                                                                      \
    MALLOC_COUNTER--;                                                                                       \
    return 1;                                                                                               \
  }

// Main function for reading input
/*
  RESTRICTIONS:
    This method only works using a text file, or stdin
    For binary files, use fread()

  ARGUMENTS:
     - FILE * stream - the input stream is passed to this method, as an argument

  RETURN VALUE:
     - SUCCESSS - char * - returns a pointer to a dinamically allocated string,
       containing the input
     - this 'raw' string MUST be further parsed, checked and casted to its
       desired type before being used in computations
     - FAILURE - returns NULL

*/
char * getString(FILE *);

int get_int(int *, FILE *);

int get_float(float *, FILE *);

int get_double(double *, FILE *);

int get_unsigned(unsigned *, FILE *);

// Function to parse a string that should contain an unsigned integer
/*
  ARGUMENTS:
    -   char * number - the input string to be parsed

  RETURN VALUE:
    - 1 - the string contains a valid unsigned integer
    - 0 - the string doesn't contain a valid unsigned integer
*/
int isValidUnsigned(char * number);

// Function to parse a string that should contain an integer
/*
  ARGUMENTS:
    -   char * number - the input string to be parsed

  RETURN VALUE:
    - 1 - the string contains a valid integer
    - 0 - the string doesn't contain a valid integer
*/
int isValidInt(char * number);

// Function to parse a string that should contain a floating point number
/*
  ARGUMENTS:
    -   char * number - the input string to be parsed

  RETURN VALUE:
    - 1 - the string contains a valid float
    - 0 - the string doesn't contain a valid float
*/
int isValidFloat(char * number);

// Function to parse a string that should contain a double precision floating
// point number
/*
  ARGUMENTS:
    -   char * number - the input string to be parsed

  RETURN VALUE:
    - 1 - the string contains a valid double
    - 0 - the string doesn't contain a valid double
*/
int isValidDouble(char * number);

// Function to check if a char is a hex digit or not
/*
  ARGUMENTS:
    -   char * x - the input char to be checked

  RETURN VALUE:
    - 1 - the char is a valid hex digit
    - 0 - the char is a valid hex digit
*/
int isValidHexChar(char * x);

// Function to check if the value of a 2-char string can represent a byte of
// data
/*
  ARGUMENTS:
    -   char * x - the input string to be checked

  RETURN VALUE:
    - 1 - the string contains a valid hex byte
    - 0 - the string doesn't contain a valid hex byte
*/
int isValidHexByte(char * number);

void err(const char * format, ...);
