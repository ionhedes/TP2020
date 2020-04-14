/*
  Aplicația 8.1: Să se modifice exemplul 2 astfel încât funcția err să
                 încapsuleze și condiția de eroare. Pentru aceasta,
                 ea primește în plus pe prima poziție o variabilă de tip int
                 care reprezintă o valoare logică. Dacă valoarea este true,
                 se va executa err ca în exemplu, altfel err nu va avea
                 niciun efect.
  Exemplu de folosire: err(n<=0||n>100,"n invalid: %d\n",n);
                       // fără if în față, deoarece err încapsulează condiția
*/

#include <stdio.h>
#include "myStdlib.h"
#include <stdarg.h>
#define MAXIMUM_VALUE_NUMBER 100


// Macro to check if 2 given values form a valid interval
/*
  USAGE:
    - checks required conditions for two numbers to determine an interval

  ARGUMENTS:
    - lower_lim - can be int, float, char or unsigned
    - upper_lim - can be int, float, char or unsigned

  WARNING:
    - this is a macro, so there is no return value per se;

  FINAL VALUE:
    - 1, true -  the interval is valid
    - 0, false - the interval is invalid
*/
#define isValidInterval(lower_lim, upper_lim) ((lower_lim) <= (upper_lim))

// Macro to check if a number doesn't break a previously defined limit
#define isValidValnum(n) ((n) < MAXIMUM_VALUE_NUMBER)

// Macro to find the maximum and minimum from n numbers belonging to the interval
/*
  USAGE:
    - pass it a valid interval and a number of values to find the marginal values
    - the n values are provided by stdin
    - if a read number doesn't belong to the interval, it is printed and the
      triggerError() function is caled, stopping the execution

  ARGUMENTS:
    - element - variable to subsequently store the read variable
    - lower_lim, upper_lim - boundaries of the interval
    - n - number of values to be read

  ADDITIONAL INFORMATION:
    - check myStdlib.h for information on get_double and DEB()
*/
#define marginalValues(element, lower_lim, upper_lim, n)                               \
  {                                                                                    \
    int i;                                                                             \
    double min_val = upper_lim;                                                        \
    double max_val = lower_lim;                                                        \
    for (i = 0; i < n; i++)                                                            \
    {                                                                                  \
      printf("\t - enter a number in range [x = %g, y = %g]: ", lower_lim, upper_lim); \
                                                                                       \
      triggerError(!get_double(&element, stdin),                                       \
                   "Error when reading the element %d.\n", i);                         \
      triggerError((element < lower_lim || element > upper_lim),                       \
                   "Invalid element (e = %g).\n", element);                            \
                                                                                       \
      if (element < min_val)                                                           \
      {                                                                                \
        DEB("\t - new minimum value set: min_val = %g;\n", element);                   \
        min_val = element;                                                             \
      }                                                                                \
      if (element > max_val)                                                           \
      {                                                                                \
        DEB("\t - new maximum value set: max_val = %g;\n", element);                   \
        max_val = element;                                                             \
      }                                                                                \
    }                                                                                  \
    printf("The marginal values of the interval are:\n");                              \
    printf("\t - minimum value: %g;\n", min_val);                                      \
    printf("\t - maximum value: %g;\n", max_val);                                      \
  }

// Function to handle conditional errors
/**
  USAGE:
    - use this in case you need to decide on the continuation of the execution
      of the program, based on the result of a condition

  ARGUMENTS:
    - const int err_condition - the condition to be met for the program to
      continue execution
    - const char * message - a message to be displayed in case the condition
      is not met; it uses the same kind of formatting as printf
    - the rest of the arguments should be values of variables, in an order that
      matches the order of expected values in the format of the message string
*/

void triggerError(const int err_condition, const char * message, ...)
{
  if (err_condition)
  {
    DEB("Error condition met.\n");
    va_list var_args;
    va_start(var_args, message);
    vfprintf(stderr, message, var_args);
    va_end(var_args);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
}

// Driver program
int main()
{
  // Needed variables
  unsigned n;
  double element;
  double lower_lim, upper_lim;

  // Get the number of values to be read
  printf("Enter the number of values: ");
  triggerError(!get_unsigned(&n, stdin),
               "Error when reading number of values.\n");

  // Get the limit of the interval
  printf("Enter the lower limit of the interval: ");
  triggerError(!get_double(&lower_lim, stdin),
               "Error when reading the lower limit of the interval.\n");

  printf("Enter the upper limit of the interval: ");
  triggerError(!get_double(&upper_lim, stdin),
               "Error when reading the upper limit of the interval.\n");

  // Check the interval and the number of values, using triggerError()
  triggerError(!isValidValnum(n),
               "The number of values (n = %d) is incorrect.\n", n);
  triggerError(!isValidInterval(lower_lim, upper_lim),
               "The interval is not valid (x = %g > y = %g).\n", lower_lim, upper_lim);

  // Call the macro to read n values and determine the lowest and the biggest one
  marginalValues(element, lower_lim, upper_lim, n);

  // End execution
  printf("Execution ended.\n");
  printf("Exiting...\n");
  return 0;
}
