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

#define isValidInterval(lower_lim, upper_lim) ((lower_lim) <= (upper_lim))
#define isValidValnum(n) ((n) < MAXIMUM_VALUE_NUMBER)

#define areInRange(element, lower_lim, upper_lim, n)                                   \
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

#define MAXIMUM_VALUE_NUMBER 100


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

int main()
{
  unsigned n;
  double element;
  double lower_lim, upper_lim;
  printf("Enter the number of values: ");
  triggerError(!get_unsigned(&n, stdin),
               "Error when reading number of values.\n");

  printf("Enter the lower limit of the interval: ");
  triggerError(!get_double(&lower_lim, stdin),
               "Error when reading the lower limit of the interval.\n");

  printf("Enter the upper limit of the interval: ");
  triggerError(!get_double(&upper_lim, stdin),
               "Error when reading the upper limit of the interval.\n");

  triggerError(!isValidValnum(n),
               "The number of values (n = %d) is incorrect.\n", n);
  triggerError(!isValidInterval(lower_lim, upper_lim),
               "The interval is not valid (x = %g > y = %g).\n", lower_lim, upper_lim);

  areInRange(element, lower_lim, upper_lim, n);

  printf("Execution ended.\n");
  printf("Exiting...\n");
  return 0;
}
