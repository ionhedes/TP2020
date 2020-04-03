/*
  Aplicația 7.6: Să se scrie un macro cu număr variabil de argumente, care
                 afișează pe o linie prima oară fișierul și linia curentă,
                 iar apoi argumentele date.
  Exemplu: SRCSHOW(”x=%g, y=%g”,x,y);  va afișa ceva de genul:
           /home/stud/1.c [21]: x=0.5, y=-7.8

*/

#include <stdio.h>

#define STRINGIFY_AUX(text) #text
#define STRINGIFY(text) STRINGIFY_AUX(text)
#define SRCSHOW(...) printf(__FILE__" ["STRINGIFY(__LINE__)"]: "__VA_ARGS__)

int main()
{
  double x = 5, y = 6;
  SRCSHOW("x=%g, y=%g",x,y);
  return 0;
}
