#include <stdio.h>
#include <stdlib.h>
#include "myHexdump.h"

unsigned opened_files_counter;

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
  opened_files_counter++;

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
    opened_files_counter--;
  }

  printf("Execution ended;\nUnclosed files: %d\nExiting...\n", opened_files_counter);
  return 0;
}
