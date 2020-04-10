/*
  Aplicația 8.6: Să se scrie o funcție char *concat(int n,...) care primește
                 un număr de șiruri de caractere și apoi șirurile propriu-zise.
                 Funcția va concatena într-un nou șir, alocat dinamic,
                 conținuturile tuturor șirurilor date, cu câte un spațiu între
                 ele și va returna acest șir.
  Exemplu: concat(3,”Ion”,”si”,”Ana”)		=> “Ion si Ana”
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "myStdlib.h"

char * concat(unsigned n, ...)
{
  int i;
  unsigned concat_string_size = 0;
  char * concat_string = NULL;
  char * aux_string = NULL;
  char * current_string = NULL;
  va_list var_args;
  va_start(var_args, n);

  if ((concat_string = (char *)malloc(sizeof(char))) == NULL)
  {
    DEB("Failed to initialize concatenated string.\n");
    return NULL;
  }
  MALLOC_COUNTER++;
  *concat_string = '\0';

  for (i = 0; i < n; i++)
  {
    current_string = va_arg(var_args, char *);
    concat_string_size += strlen(current_string);
    //resize and concat

    if ((aux_string = (char *)realloc(concat_string, (concat_string_size + 2) * sizeof(char))) == NULL)
    {
      DEB("Failed resizing array at step %d.\n", i + 1);
      free(concat_string);
      MALLOC_COUNTER--;
      return NULL;
    }
    concat_string = aux_string;
    strcat(concat_string, current_string);
    strcat(concat_string, " ");
  }

  va_end(var_args);
  return concat_string;
}

int main()
{
  char * concat_string = NULL;
  if ((concat_string = concat(3, "Ion", "si", "Ana")) == NULL)
  {
    fprintf(stderr, "Error when concatenating strings.\n");
    exit(EXIT_FAILURE);
  }
  printf("The string concatenated strings result the following message:\n%s\n",
         concat_string);

  free(concat_string);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
