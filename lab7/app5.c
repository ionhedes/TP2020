/*
  Aplicația 7.6: Să se scrie un macro cu număr variabil de argumente, care
                 afișează pe o linie prima oară fișierul și linia curentă,
                 iar apoi argumentele date.
  Exemplu: SRCSHOW(”x=%g, y=%g”,x,y);  va afișa ceva de genul:
           /home/stud/1.c [21]: x=0.5, y=-7.8

*/

#include <stdio.h>

// Macros used to convert different text to strings in the compilation phase
#define STRINGIFY_AUX(text) #text
#define STRINGIFY(text) STRINGIFY_AUX(text)

// Debug intenteded macros
/**
  SRCSHOW - prints data about the place where the call to it has been made,
            mentioning the file and line, as well as an user input message
*/
#define SRCSHOW(...) printf(__FILE__" ["STRINGIFY(__LINE__)"]: "__VA_ARGS__)

// Driver program for SRCSHOW
int main()
{
  double x = 5, y = 6;
  SRCSHOW("x=%g, y=%g",x,y);
  return 0;
}
