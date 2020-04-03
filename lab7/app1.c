/*
  Aplicația 7.2: Să se scrie o aplicație care implementează o funcție abs_real
                 (returnează valoarea absolută a unui double) în două moduri,
                 în funcție de prezența definiției IMPLICIT la compilare.
                 Dacă această definiție există, abs_real va folosi funcția fabs
                 din math.h. Dacă IMPLICIT nu există, abs_real nu va folosi
                 nicio altă funcție.
*/

#ifdef IMPLICIT
  #include <math.h>
  #define abs_real(x) fabs(x)
#else
  #define abs_real(x) ((x) >= 0 ? (x) : (-(x)))
#endif
#include <stdio.h>
#include "myStdlib.h"

int main()
{
  double num;
  printf("Enter a number: ");
  if (!get_double(&num, stdin))
  {
    fprintf(stderr, "Input error.\n");
    exit(EXIT_FAILURE);
  }
  printf("The absolute value of the number is: %lf;\n", abs_real(num));
  fprintf(stderr, "Execution ended.\n");
  return 0;
}
