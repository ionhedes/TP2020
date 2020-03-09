#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

unsigned malloc_counter;

typedef enum {Int, Double, Char, String}TypeList;

typedef struct
{
  char * name;
  TypeList type;
  union
  {
    int decimal_number;
    double floating_point_number;
    char character;
    struct
    {
      char * content;
      unsigned length;
    }string;
  }data;
}Variable;

char * getString()
{
  char * string = NULL, * aux = NULL;
  char buffer_char;
  unsigned length = 0;

  while ((buffer_char = getchar()) != '\n')
  {
    if ((aux = (char *)realloc(string, (length + 1) * sizeof(char))) == NULL)
    {
      printf("getString() error..\nUnreleased memory blocks: %d\n", malloc_counter);
      free(string);
      malloc_counter--;
      return NULL;
    }
    string = aux;
    malloc_counter += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;
}

int checkString(char * string)
{
  if(!isalpha(string [0]) || !strchr(string, '='))
  {
    return 0;
  }
  return 1;
}

void extractNumber(void * destination, char * number_starting_point, char * point_position)
{
  int * integer_destination = NULL;
  double * floating_point_destination = NULL;
  char *aux = NULL;

  if (point_position)
  {
    floating_point_destination = (double *)destination;
    for (aux = number_starting_point; isdigit(*aux); aux++)
    {
      *floating_point_destination = *floating_point_destination * 10 + (*aux - '0');
    }
    for(aux = point_position + 1; isdigit(*aux); aux++)
    {
      *floating_point_destination = *floating_point_destination + (*aux - '0') * pow(10, (double)(point_position - aux));
    }
  }
  else
  {
    integer_destination = (int *)destination;
    for (aux = number_starting_point; isdigit(*aux); aux++)
    {
      *integer_destination = *integer_destination * 10 + (*aux - '0');
    }
  }

}

void extractString(char * destination, char * string_starting_point, char * string_ending_point)
{
  int i = 0;
  char * aux = NULL;
  for (aux = string_starting_point + 1; aux < string_ending_point; aux++, i++)
  {
    *(destination + i) = *aux;
  }
  *(destination + i) = '\0';
}

Variable * parseRVal(char * string)
{
  Variable * var = NULL;
  if ((var = (Variable *)malloc(sizeof(Variable))) == NULL)
  {
    return NULL;
  }
  malloc_counter++;

  char alphabet [] = "abcdefghijklmnopqrstuvwxyz";
  char separators [] = "()[]{}\\|_;:,<.>/?`~!@#$%^&*\n";
  char * opening_bracket = NULL, * closing_bracket = NULL, * aux = NULL;
  char * goto_attribution = strchr(string, '='), * goto_rval = NULL, * goto_point = NULL, * goto_null = NULL;
  //printf("Found = at %ld\n", goto_attribution - string);


  goto_rval = goto_attribution + 1;
  //Gets rid of whitespaces
  while (*goto_rval == ' ')
  {
    goto_rval++;
  }
  //printf("Found rvalue at %ld\n", goto_rval - string);

  //Now we have the address where the rvalue should start, we start working on inferring a type
  if (strchr(separators, *goto_rval))
  {
    free(var);
    malloc_counter--;
    return NULL; //Invalid input
  }
  else if (*goto_rval == 34)
  {
    var->type = String;
    opening_bracket = goto_rval;
    printf("Found opening bracket at %ld\n", opening_bracket - string);
    if ((closing_bracket = strchr(opening_bracket + 1, 34)))
    {
      printf("Found closing bracket at %ld\n", closing_bracket - string);
      while((aux = strchr(closing_bracket + 1, 34)))
      {
        closing_bracket = aux;
      }
      printf("Found final closing bracket at %ld\n", closing_bracket - string);
      goto_null = closing_bracket + 1;
      while (*goto_null == ' ')
      {
        goto_null++;
      }
      if (*goto_null)
      {
        free(var);
        malloc_counter--;
        return NULL;
      }
      var->data.string.length = closing_bracket - opening_bracket - 1;
      printf("The length of your string is: %d\n", var->data.string.length);


      if ((var->data.string.content = (char *)malloc((var->data.string.length + 1) * sizeof(char))) == NULL)
      {
        free(var);
        malloc_counter--;
        return NULL;
      }
      malloc_counter++;
      extractString(var->data.string.content, opening_bracket, closing_bracket);
    }
    else
    {
      free(var);
      malloc_counter--;
      return NULL; // Invalid input
    }
  }
  else if (*goto_rval == 39)
  {
    //printf("Found single opening bracket at %ld\n", goto_rval - string);
    var->type = Char;
    opening_bracket = goto_rval;
    closing_bracket = opening_bracket + 2;
    if (*(closing_bracket) != 39)
    {
      free(var);
      malloc_counter--;
      return NULL;
    }
    goto_null = closing_bracket + 1;
    while(*goto_null == ' ')
    {
      goto_null++;
    }
    if (*goto_null)
    {
      free(var);
      malloc_counter--;
      return NULL;
    }
    var->data.character = *(opening_bracket + 1);
  }
  else if (isdigit(*goto_rval))
  {
    if (strpbrk(goto_rval, alphabet) || strchr(goto_rval, 34) || strchr(goto_rval, 39))
    {
      free(var);
      malloc_counter--;
      return NULL;
    }

    //We have a number, finding out whether it is an integer, or a floating point number
    if ((goto_point = strchr (goto_rval + 1, '.')))
    {
      //Fooling the program so I can check for invalid characters a lot easier
      *goto_point = '0';
      if (strpbrk(goto_rval, separators))
      {
        free(var);
        malloc_counter--;
        return NULL;
      }
      *goto_point = '.';
      var->type = Double;

      var->data.floating_point_number = 0;
      extractNumber((void *)&(var->data.floating_point_number), goto_rval, goto_point);

    }
    else
    {
      if (strpbrk(goto_rval, separators))
      {
        free(var);
        malloc_counter--;
        return NULL;
      }
      var->type = Int;

      var->data.decimal_number = 0;
      extractNumber((void *)&(var->data.decimal_number), goto_rval, NULL);
    }
  }
  return var;
}

int main()
{
  Variable * var = NULL;
  char * string = NULL;
  string = getString();
  if (checkString(string))
  {
    if ((var = parseRVal(string)) == NULL)
    {
      free(string);
      malloc_counter--;
      printf("Something went wrong, or the input is incorrect.\nUnreleased memory blocks: %d;\nExiting...\n", malloc_counter);
      exit(EXIT_FAILURE);
    }

    switch(var->type)
    {
      case Int:
      {
        printf("The integer is: %d\n", var->data.decimal_number);
        free(var);
        malloc_counter--;
        break;
      }
      case Char:
      {
        printf("The caracter is: %c\n", var->data.character);
        free(var);
        malloc_counter--;
        break;
      }
      case String:
      {
        printf("The string is: %s\n", var->data.string.content);
        free(var->data.string.content);
        free(var);
        malloc_counter -= 2;
        break;
      }
      case Double:
      {
        printf("The floating point number is: %f\n", var->data.floating_point_number);
        free(var);
        malloc_counter--;
        break;
      }
    }
  }

  free(string);
  malloc_counter--;
  printf("Unreleased memory blocks: %d;\nExiting...\n", malloc_counter);
  return 0;
}
