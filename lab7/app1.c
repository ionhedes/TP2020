/*
  Aplicația 7.2: Să se scrie o aplicație care implementează o funcție abs_real
                 (returnează valoarea absolută a unui double) în două moduri,
                 în funcție de prezența definiției IMPLICIT la compilare.
                 Dacă această definiție există, abs_real va folosi funcția fabs
                 din math.h. Dacă IMPLICIT nu există, abs_real nu va folosi
                 nicio altă funcție.
*/

#include <stdio.h>
#include "myStdlib.h"

/**
    - If the IMPLICIT macro is defined, the absolute value of real numbers is to
    be calculated using the method defined inside math.h
    - Else, we define an inline function for computing the absolute value of a
    real number
*/
#ifdef IMPLICIT
  #include <math.h> // < no use including if we do not use fabs()
  #define abs_real(x) fabs(x)
#else
  #define abs_real(x) ((x) >= 0 ? (x) : (-(x)))
#endif


// Driver program for calculating absolute values
int main()
{
  double num; // < variable for a real number
  printf("Enter a number: ");
  if (!get_double(&num, stdin)) // < reading a double value using get_double
  {
    DEB("Input error.\n"); // < treating input errors
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  printf("The absolute value of the number is: %lf;\n", abs_real(num));
  printf("Execution ended.\n");
  return 0;
}
