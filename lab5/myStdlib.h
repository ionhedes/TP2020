#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned MALLOC_COUNTER;
unsigned OPENED_FILES_COUNTER;

char * getString(FILE * stream)
{
  char * string = NULL, * aux = NULL;
  char buffer_char;
  unsigned length = 0;

  // Strings are considered to span until \n
  while((buffer_char = getc(stream)) != '\n')
  {
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

int isValidInteger(char * number)
{
  char * iterator = number;
  if (*iterator == '-')
  {
    iterator++;
  }
  for (; *iterator != '\0'; iterator++)
  {
    if (!isdigit(*iterator))
    {
      return 0;
    }
  }
  return 1;
}

int isValidFloat(char * number)
{
  char * iterator = number;
  char * point_location = strchr(number, '.');

  if (*iterator == '-')
  {
    iterator++;
  }
  if (point_location)
  {
    for (; iterator < point_location ; iterator++)
    {
      if (!isdigit(*iterator))
      {
        return 0;
      }
    }
    for (iterator = point_location + 1; *iterator != '\0'; iterator++)
    {
      if (!isdigit(*iterator))
      {
        return 0;
      }
    }
  }
  else
  {
    if (!isValidInteger(number))
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
  if (strlen(number) != 2)
  {
    return 0;
  }
  for (; *iterator != '\0'; iterator++)
  {
    if (!isValidHexChar(iterator))
    {
      return 0;
    }
  }
  return 1;
}

int getInt(int * location, int (* validationFunction)(char * number), FILE * stream)
{
  char * aux_element = NULL;

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
  *location = atoi(aux_element);
  free(aux_element);
  MALLOC_COUNTER--;
  return 1;
}
