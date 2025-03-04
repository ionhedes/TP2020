/////////////////////////////////////////////////
/// myStdlib - library created so I can get a better grasp of error handling and
///            how memory and reading/writing data are linked
///          - also it was bothersome to rewrite some methods that I would use
///            in every program
/////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// The following global variables are used for debugging
// They keep track of malloc()/free() and fopen()/fclose() calls
// Sadly, I could not find a better way of managing these
unsigned MALLOC_COUNTER;
unsigned OPENED_FILES_COUNTER;

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
char * getString(FILE * stream)
{
  char * string = NULL, * aux = NULL; // < Pointers needed for realloc() and
                                      //   storing the input
  char buffer_char; // < Stores the read item at every step
  unsigned length = 0; // < Length of the string

  // Strings are considered to span until \n - that is why reading from binary
  // files won't cut it
  while((buffer_char = getc(stream)) != '\n')
  {
    // Increment the size of the string by one everytime we read a char
    if ((aux = (char *)realloc(string, (length + 1) * sizeof(char))) == NULL)
    {
      fprintf(stderr, "getString() error...\n");
      free(string);
      MALLOC_COUNTER--;
      return NULL;
    }
    string = aux;

    //MALLOC_COUNTER should only be increased once, on the first iteration
    MALLOC_COUNTER += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }

  // No use setting up the \0 character if we don't have a valid string
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;
}

// Function to parse a string that should contain an integer
/*
  ARGUMENTS:
    -   char * number - the input string to be parsed

  RETURN VALUE:
    - 1 - the string contains a valid integer
    - 0 - the string doesn't contain a valid integer
*/
int isValidInt(char * number)
{
  char * iterator = number; // < Pointer to go char-by-char

  // If the number is negative, we skip its sign
  if (*iterator == '-')
  {
    iterator++;
  }
  for (; *iterator != '\0'; iterator++)
  {
    // Any nondigit char found means the string isn't a valid integer
    if (!isdigit(*iterator))
    {
      return 0;
    }
  }
  return 1;
}

// Function to parse a string that should contain a floating point number
/*
  ARGUMENTS:
    -   char * number - the input string to be parsed

  RETURN VALUE:
    - 1 - the string contains a valid float
    - 0 - the string doesn't contain a valid float
*/
int isValidFloat(char * number)
{
  char * iterator = number; // < Pointer to go char-by-char
  char * point_location = strchr(number, '.'); // Pointer to the address of the
                                               // point inside the number
                                               // - if it exists

  // If the number is negative, we skip its sign
  if (*iterator == '-')
  {
    iterator++;
  }
  if (point_location)
  {
    // Checking the chars before the point
    for (; iterator < point_location ; iterator++)
    {
      if (!isdigit(*iterator))
      {
        return 0;
      }
    }
    // Checking the chars after the point
    for (iterator = point_location + 1; *iterator != '\0'; iterator++)
    {
      if (!isdigit(*iterator))
      {
        return 0;
      }
    }
  }
  // If the string doesn't contain a point, the problem reduces itself to
  // checking the validity of an integer
  else
  {
    if (!isValidInt(number))
    {
      return 0;
    }
  }
  return 1;
}

// Function to check if a char is a hex digit or not
/*
  ARGUMENTS:
    -   char * x - the input char to be checked

  RETURN VALUE:
    - 1 - the char is a valid hex digit
    - 0 - the char is a valid hex digit
*/
int isValidHexChar(char * x)
{
  char * accepted_chars = "0123456789abcdefABCDEF";
  return (strchr(accepted_chars, *x) != NULL);
}

// Function to check if the value of a 2-char string can represent a byte of
// data
/*
  ARGUMENTS:
    -   char * x - the input string to be checked

  RETURN VALUE:
    - 1 - the string contains a valid hex byte
    - 0 - the string doesn't contain a valid hex byte
*/
int isValidHexByte(char * number)
{
  char * iterator = number;
  // No use checking further if the string contains more than two chars
  if (strlen(number) != 2)
  {
    return 0;
  }
  for (; *iterator != '\0'; iterator++)
  {
    // For every char in the string, we use isValidHexChar() to determine the
    // validity
    if (!isValidHexChar(iterator))
    {
      return 0;
    }
  }
  return 1;
}

// Function to check, cast and store the string given by getString() into
// an int
/*
  ARGUMENTS:
    -   int * location - address to storing place of the integer
    -   int (* validationFunction) - function to check if the string represents
        a valid int
    -   FILE * stream - specifies the input stream

  RETURN VALUE:
    - 1 - operation succesful
    - 0 - error along the way, if a message isn't printed to stderr, sorry, my bad
*/
int getInt(int * location, int (* validationFunction)(char * number), FILE * stream)
{
  char * aux_element = NULL; // < buffer string spanning only this function

  // Calls getString() to get new strings until the validation function confirms
  // that the string represents a valid integer, or an error occurs
  if ((aux_element = getString(stream)) == NULL)
  {
    return 0;
  }
  if (!validationFunction(aux_element))
  {
    do
    {
      free(aux_element);
      MALLOC_COUNTER--;
      printf("\t ! Enter a valid integer: ");
      if ((aux_element = getString(stream)) == NULL)
      {
        return 0;
      }
    }
    while (!validationFunction(aux_element));
  }
  // When a valid int is eventually given, cast it and store it in the desired
  // location
  *location = atoi(aux_element);

  // Free the string generated by getString(), now that the int is safely placed
  // in its desired location
  free(aux_element);
  MALLOC_COUNTER--;
  return 1;
}

int getFloat(float * location, int (* validationFunction)(char * number), FILE * stream)
{
  char * aux_element = NULL; // < buffer string spanning only this function

  // Calls getString() to get new strings until the validation function confirms
  // that the string represents a valid integer, or an error occurs
  if ((aux_element = getString(stream)) == NULL)
  {
    return 0;
  }
  if (!validationFunction(aux_element))
  {
    do
    {
      free(aux_element);
      MALLOC_COUNTER--;
      printf("\t ! Enter a valid integer: ");
      if ((aux_element = getString(stream)) == NULL)
      {
        return 0;
      }
    }
    while (!validationFunction(aux_element));
  }
  // When a valid int is eventually given, cast it and store it in the desired
  // location
  *location = atof(aux_element);

  // Free the string generated by getString(), now that the int is safely placed
  // in its desired location
  free(aux_element);
  MALLOC_COUNTER--;
  return 1;
}
