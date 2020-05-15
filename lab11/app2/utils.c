#include "utils.h"

unsigned MALLOC_COUNTER;
unsigned OPENED_FILES_COUNTER;

char * getString(FILE * stream)
{
  char * string = NULL, * aux = NULL; // < Pointers needed for realloc() and
                                      //   storing the input
  char buffer_char; // < Stores the read item at every step
  unsigned length = 0; // < Length of the string

  // Strings are considered to span until \n - that is why reading from binary
  // files won't cut it
  buffer_char = getc(stream);
  while(buffer_char != '\n' && buffer_char != EOF)
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

    buffer_char = getc(stream);
  }

  // No use setting up the \0 character if we don't have a valid string
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;
}

int isValidUnsigned(char * number)
{
  char * iterator = number; // < Pointer to go char-by-char

  for (; *iterator != '\0'; iterator++)
  {
    // Any nondigit char found means the string isn't a valid unsigned integer
    if (!isdigit(*iterator))
    {
      return 0;
    }
  }
  return 1;
}

int isValidInt(char * number)
{
  char * iterator = number; // < Pointer to go char-by-char

  // If the number is negative, we skip its sign
  if (*iterator == '-')
  {
    iterator++;
  }

  // After skipping the minus sign, if it exists, the problem reduces itself to
  // checking the validity of an unsigned
  if (!isValidUnsigned(iterator))
  {
    return 0;
  }
  return 1;
}

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

int isValidDouble(char * number)
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

int isValidHexChar(char * x)
{
  char * accepted_chars = "0123456789abcdefABCDEF";
  return (strchr(accepted_chars, *x) != NULL);
}

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

void err(const char * format, ...)
{
  va_list var_args;
  va_start(var_args, format);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, format, var_args);
  fputc('\n', stderr);
  va_end(var_args);
  exit(EXIT_FAILURE);
}

generate_input_function(int)
generate_input_function(float)
generate_input_function(double)
generate_input_function(unsigned)
