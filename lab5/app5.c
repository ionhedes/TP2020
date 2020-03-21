#include <stdio.h>
#include <stdlib.h>
#define ROW_LENGTH 16

unsigned opened_files_counter;

int printRow(FILE * source)
{
  int i;
  int eof_index = -1;
  char buffer [16];

  // Offset
  long current_position = ftell(source);
  if (current_position == -1)
  {
    return 0;
  }
  printf("%08lx", current_position);
  printf("  ");

  // Hex data
  for (i = 0; i < ROW_LENGTH; i++)
  {
    if (i == 8)
    {
      putchar(' ');
    }
    if (fread(&buffer [i], 1, 1, source) != 1 && ferror(source))
    {
      fprintf(stderr, "\nFailed to read from file.\n");
      return 0;
    }
    if (feof(source))
    {
      if (eof_index == -1)
      {
        eof_index = i;
      }
      printf("%02x ", 0);
    }
    else
    {
      printf("%02x ", buffer [i]);
    }
  }
  printf(" |");

  // ASCII data
  for (i = 0; i < ROW_LENGTH && i != eof_index; i++)
  {
    if (buffer [i] >= 32 && buffer [i] <= 255)
    {
      putchar(buffer [i]);
    }
    else
    {
      putchar('.');
    }
  }
  putchar('|');

  return 1;
}

int myHexdump(FILE * source)
{
  while(!feof(source) && !ferror(source))
  {
    if (!printRow(source))
    {
      return 0;
    }
    putchar('\n');
  }
  return 1;
}

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
