/**
  Aplicația 5.5: Să se scrie un program similar cu hexdump, care primește un
                nume de fișier în linia de comandă, citește câte 16 octeți din
                el și îi afișează pe câte o linie. Fiecare linie începe cu
                offsetul ei în fișier, afișat în hexazecimal pe 8 cifre cu 0
                în față, după care valorile hexa ale celor 16 octeți pe câte 2
                cifre și în final caracterele. Dacă codul unui caracter este în
                intervalul 32-255 se va afișa ca atare, altfel se va afișa
                un punct în locul său. Dacă ultima linie este incompletă,
                se va completa cu octeți de 0.
*/

// All the functions regarding this program are insider myHexdump.h

#include <stdio.h>
#include <stdlib.h>
#include "myHexdump.h"

// The following global variables are used for debugging
// They keep track of malloc()/free() and fopen()/fclose() calls
// Sadly, I could not find a better way of managing these
unsigned OPENED_FILES_COUNTER;

int main(int argc, char * argv [])
{
  FILE * source; // < pointer/handler for the source file

  // Number of args must be 2: the name of the program and the name of the file
  // to read from
  if (argc != 2)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  // Opening the source file
  if ((source = fopen(argv [1], "r")) == NULL)
  {
    fprintf(stderr, "Failed to open file.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  OPENED_FILES_COUNTER++;

  // Running the hexdump clone on the file
  if(!myHexdump(source))
  {
    fprintf(stderr, "Failed to execute hexdump().\n");
  }
  putchar('\n');

  // Closing the source file
  if (fclose(source))
  {
    fprintf(stderr, "Failed to close file.\n");
  }
  else
  {
    OPENED_FILES_COUNTER--;
  }

  printf("Execution ended;\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
  return 0;
}
