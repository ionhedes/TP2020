#include <stdio.h>
#include <stdlib.h>
#include "myHexdump.h"

unsigned OPENED_FILES_COUNTER;

int main(int argc, char * argv [])
{
  FILE * source;

  if (argc != 2)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  if ((source = fopen(argv [1], "r")) == NULL)
  {
    fprintf(stderr, "Failed to open file.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  OPENED_FILES_COUNTER++;

  if(!myHexdump(source))
  {
    fprintf(stderr, "Failed to execute hexdump().\n");
  }
  putchar('\n');

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
