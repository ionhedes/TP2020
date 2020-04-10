/*
  Aplicația 8.5: Să se implementeze o funcție input(const char *fmt,...).
                 În șirul fmt pot fi caractere obișnuite (orice în afară de %)
                 și placeholdere (% urmat de o literă).
                 Pentru fiecare placeholder posibil ( %d - int, %f -float,
                 %c - char ), în lista de argumente variabile se va afla adresa
                 unei variabile de tipul corespunzător. Funcția afișează pe
                 ecran caracterele obișnuite și citește de la tastatură
                 pentru placeholdere.
  Exemplu: input(”n=%dch=%c”,&n,&ch);
           // citește o valoare de tip int în n și de tip char în ch
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "myStdlib.h"

int isValidTypePlaceholder(char * target)
{
  return (*target == 'd' || *target == 'c' || *target == 'f');
}

int input_custom(const char * string, ...)
{
  char * placeholder_position = NULL;
  char * iterator = string;
  char type;

  va_list var_args;
  va_start(var_args, string);

  do
  {
    while(!isalpha(*iterator) && *iterator != '\0')
    {
      iterator++;
    }
    placeholder_position = strchr(iterator, '%');
    if (placeholder_position == NULL)
    {
      break;
    }

    if (!isValidTypePlaceholder(placeholder_position + 1))
    {
      DEB("The input string format is invalid.\n");
      return 0;
    }
    type = *(placeholder_position + 1);

    for (; iterator < placeholder_position; iterator++)
    {
      printf("%c", *iterator);
    }

    switch(type)
    {
      case 'd':
      {
        int * addr = va_arg(var_args, int *);
        if (!get_int(addr, stdin))
        {
          DEB("Failed to get a int value.\n");
          return 0;
        }
        break;
      }
      case 'f':
      {
        double * addr = va_arg(var_args, double *);
        if (!get_double(addr, stdin))
        {
          DEB("Failed to get a double value.\n");
          return 0;
        }
        break;
      }
      case 'c':
      {
        char * addr = va_arg(var_args, char *);
        if ((*addr = getchar()) == EOF)
        {
          DEB("Failed to get a char value.\n");
          return 0;
        }
        break;
      }
      default:
      {
        fprintf(stderr, "Runtime error occured.\n");
        DEB("The switch statement selected the default branch. This should not have happened.\n");
        return 0;
      }
    }

    iterator = placeholder_position + 2;

    putchar('\n');
  }
  while(placeholder_position != NULL);

  va_end(var_args);
  return 1;
}

int main()
{
  int n;
  char ch;
  char * input_string = NULL;

  printf("Enter the formatted input string:\n");
  if ((input_string = getString(stdin)) == NULL)
  {
    DEB("Failed to get string from input.\n");
    printf("Exiting...\n");
  }

  if(!input_custom(input_string, &n, &ch))
  {
    fprintf(stderr, "Failed to read values for the given variables.\n");
    free(input_string);
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
  }

  // Free the memory and end execution
  free(input_string);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
