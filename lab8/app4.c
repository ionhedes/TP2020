/*
  Aplicația 8.4: Să se scrie o funcție crescator(int n,char tip,...) care
                 primește un număr n de valori și returnează 1 dacă ele sunt în
                 ordine strict crescătoare, altfel 0. Caracterul tip indică
                 tipul valorilor și poate fi ‘d’ - int, ‘f’ - double.
  Exemplu: printf(“%d”,crescator(3,’d’,-1,7,9));
*/

#include <stdio.h>
#include <stdarg.h>
#include "arrayHandler.h"

#define MINIMUM_SIZE 3
create_array_handler(float)

int isAscendingOrder(unsigned n, char type, ...)
{
  int i;
  va_list var_args;
  va_start(var_args, type);

  switch(type)
  {
    case 'd':
    {
      int previous_val, current_val;
      previous_val = va_arg(var_args, int);
      for (i = 1; i < n; i++)
      {
        current_val = va_arg(var_args, int);
        if (previous_val > current_val)
        {
          DEB("At position %d, the ascending order is broken (%d > %d).\n", i,
              previous_val, current_val);
          return 0;
        }
        previous_val = current_val;
      }
      break;
    }

    case 'u':
    {
      unsigned previous_val, current_val;
      previous_val = va_arg(var_args, unsigned);
      for (i = 1; i < n; i++)
      {
        current_val = va_arg(var_args, unsigned);
        if (previous_val > current_val)
        {
          DEB("At position %d, the ascending order is broken (%d > %d).\n", i,
              previous_val, current_val);
          return 0;
        }
        previous_val = current_val;
      }
      break;
    }

    case 'f':
    {
      double previous_val, current_val;
      previous_val = va_arg(var_args, double);
      for (i = 1; i < n; i++)
      {
        current_val = va_arg(var_args, double);
        if (previous_val > current_val)
        {
          DEB("At position %d, the ascending order is broken (%g > %g).\n", i,
              previous_val, current_val);
          return 0;
        }
        previous_val = current_val;
      }
      break;
    }

    default:
    {
      fprintf(stderr, "Runtime error occured.\n");
      DEB("The switch statement selected the default branch. This should not have happened.\n");
      return -1;
    }
  }
  va_end(var_args);
  return 1;
}

int main()
{
  unsigned array_size;
  int return_value;
  float * array = NULL;
  printf("Enter the size of the array (size > %d): ", MINIMUM_SIZE);
  do
  {
    if (!get_unsigned(&array_size, stdin))
    {
      DEB("Error when reading the array size.\n");
      printf("Exiting...\n");
    }
  }
  while (array_size < MINIMUM_SIZE);

  if ((array = createArray_float(&array_size)) == NULL)
  {
    DEB("Error when making space for the array.\n");
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if (!fillArray_float(array, &array_size))
  {
    DEB("Error when filling the array.\n");
    free(array);
    MALLOC_COUNTER--;
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  putchar('\n');
  printArray_float(array, &array_size);

  putchar('\n');

  return_value = isAscendingOrder(3, 'f', array [0], array [1], array [2]);
  if (return_value == -1)
  {
    free(array);
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
  }
  else if (!return_value)
  {
    printf("The numbers are not in ascending order.\n");
  }
  else
  {
    printf("The numbers are in ascending order.\n");
  }
  printf("Keep in mind!\n");
  printf("Because the static nature of the implementation, imposed on by this task, ");
  printf("it is possible that the function has only checked a subset of numbers from the array, ");
  printf("thus making the returned value possibly incorrect.\n");
  putchar('\n');

  // Free the memory and end execution
  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
