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

// Function to concatenate strings found inside the variable argument list
/*
  USAGE:
    - use it to concatenate n strings inside a newly allocated string
    - spaces will be put between each word

  ARGUMENTS:
    - unsigned n - number of string to be concatenated
    - the rest of the arguments will be the strings to be concatenated
*/
char * concat(unsigned n, ...)
{
  int i;
  unsigned concat_string_size = 0;
  char * concat_string = NULL;
  char * aux_string = NULL;
  char * current_string = NULL;
  va_list var_args;
  va_start(var_args, n);

  // Initialize the resulting string, and put a '\0' inside, so strcat()
  // can be used with it
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
    concat_string_size += strlen(current_string); // < Before adding a new string,
                                                  // adjust the size of the resulting
                                                  // string

    // Resize the resulting array according to the adjusted array size
    if ((aux_string = (char *)realloc(concat_string, (concat_string_size + 2) * sizeof(char))) == NULL)
    {
      DEB("Failed resizing array at step %d.\n", i + 1);
      free(concat_string);
      MALLOC_COUNTER--;
      return NULL; // < In case of realloc() error, return NULL
    }
    concat_string = aux_string;

    // After the resizings have been done, concatenate the current parsed string
    // to the resulting one
    strcat(concat_string, current_string);
    strcat(concat_string, " ");
  }

  va_end(var_args);
  return concat_string; // < Return a pointer to the resulting concatenated string
}

// Driver program for the concatenation function
int main()
{
  // Needed variable
  char * concat_string = NULL;

  // Call the concatenation function with a set of data
  // I am lazy again, I am sorry
  if ((concat_string = concat(3, "Ion", "si", "Ana")) == NULL)
  {
    fprintf(stderr, "Error when concatenating strings.\n"); // < treat concat errors
    exit(EXIT_FAILURE);
  }
  printf("The string concatenated strings result the following message:\n%s\n",
         concat_string);

  // Free the memory and end execution
  free(concat_string);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
