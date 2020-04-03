/*
  Aplicația 7.5: Să se scrie o funcție care primește ca argument un pointer la
                 un vector v de 4 octeți de tip unsigned char. Programul va
                 putea fi compilat doar cu gcc. Dacă se folosește alt compilator,
                 se va genera eroare. Dacă platforma pe care se face compilarea
                 este big endian, atunci funcția va afișa octeții de la stânga
                 la dreapta, altfel octeții vor fi afișați de la dreapta la
                 stânga. Se poate folosi documentația cu macrourile
                 predefinite de gcc.

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef __GNUC__
 #error "The program can only be compiled with gcc.\n"
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define ORDER -1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define ORDER 1
#else
  #error "Unknown architecture.\n";
#endif

#define ARRAY_SIZE 4

void showBits(unsigned value)
{
  assert(value >=0 && value <= 255);
  int i;
  printf("The bits of %c are:\n", value);
  if (ORDER == 1)
  {
    printf("--big endian--\n\t - ");
    for (i = 7; i >= 0; i--)
    {
      if (i == 4)
      {
        putchar(' ');
      }
      printf("%d", (value >> i) & 1);
    }
  }
  else if (ORDER == -1)
  {
    printf("--little endian--\n\t - ");
    for (i = 0; i <= 7; i++)
    {
      if (i == 4)
      {
        putchar(' ');
      }
      printf("%d", (value >> i) & 1);
    }
  }
  putchar('\n');
}

void doSomething(const unsigned char * array)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    showBits(array [i]);
    putchar('\n');
  }
}

int readElements(unsigned char * array, const unsigned array_size)
{
  int i;
  for (i = 0; i < array_size; i++)
  {
    if((array [i] = getchar()) == EOF)
    {
      return 0;
    }
  }

  return 1;
}

int main()
{
  unsigned char array [ARRAY_SIZE];
  printf("Enter the array of chars: ");
  if(!readElements(array, ARRAY_SIZE))
  {
    fprintf(stderr, "Failed to get array elements.\n");
    exit(EXIT_FAILURE);
  }
  doSomething(array);
  return 0;
}
