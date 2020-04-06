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

// Macro to test if the used compiler is GCC
/*
  DETAILED:
    - when compiling with GCC, some macros are defined by the compiler, including
      ones regarding the GCC version
    - as stated in the GCC documentation, to discern a compilation done by GCC
      from one done by another compiler, it is enough to test the macro __GNUC__
      for definition
    - the limitations of this program force us to compile it with GCC, thus requiring
      this macro
*/
#ifndef __GNUC__
 #error "The program can only be compiled with gcc.\n"
#endif

// Macro to find out the order of bits inside bytes (MSB/LSB)
/*
  DETAILED:
    - different CPU architectures have different ways of storing and dealing
      with bits inside bytes
    - all the main architectures (ARM, x86) use little endian, but just to be sure,
      we test it
*/
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define ORDER -1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define ORDER 1
#else
  #error "Unknown architecture.\n";
#endif

#define ARRAY_SIZE 4 // < Program restriction

// Function to show the bits inside a byte
/*
  USAGE:
    - use on unsigned values
    - prints the bits inside a byte, in MSB or LSB, depending on the ORDER macro
    - for printing bits, bitwise barrel shifts and ANDs are used

  ARGUMENTS:
    - unsigned value - the byte
*/
void showBits(unsigned value)
{
  assert(value >=0 && value <= 255);
  int i;
  printf("The bits of %c are:\n", value);
  if (ORDER == 1)
  {
    DEB("--big endian--\n\t - ");
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
    DEB("--little endian--\n\t - ");
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

// The function requested by the problem
/*
  USAGE:
    - use it to print all the bits inside a 4 unsigned chars long array,
      with subsequent calls to showBits()

  ARGUMENTS:
    - const unsigned char * array - pointer to the aforementioned array
*/
void doSomething(const unsigned char * array)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    showBits(array [i]);
    putchar('\n');
  }
}

// Function to fill the 4-char array
/*
  ARGUMENTS:
    - unsigned char * array - pointer to the array
    - const unsigned array_size - size of the array (here fixed to 4)

  RETURN VALUE:
    - 1 - success, all chars have been read
    - 0 - getc() error
*/
int readElements(unsigned char * array, const unsigned array_size)
{
  int i;
  for (i = 0; i < array_size; i++)
  {
    if((array [i] = getchar()) == EOF) // < Treat input errors
    {
      return 0;
    }
  }
  return 1;
}

// Driver program for the previous functions
int main()
{
  unsigned char array [ARRAY_SIZE];
  printf("Enter the array of chars: ");
  if(!readElements(array, ARRAY_SIZE)) // < Read the 4 chars
  {
    DEB(stderr, "Failed to get array elements.\n"); // < Treating input errors
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  doSomething(array); // < Print the bits from the array
  printf("Exiting...\n");
  return 0;
}
