#include <stdio.h>
#include "myStdlib.h"
#define BLOCK_LIMIT 4096

int appendFile(FILE * destination_file, char * source_file_name)
{
  FILE * source_file = NULL;
  long read_blocks;
  char buffer [BLOCK_LIMIT];

  if ((source_file = fopen(source_file_name, "r")) == NULL)
  {
    fprintf(stderr, "Failed to open one of the source files.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;

  if ((read_blocks = fread(buffer, 1, BLOCK_LIMIT, source_file)) != BLOCK_LIMIT && ferror (source_file))
  {
    fprintf(stderr, "Failed to read from one of the source files.\n");
    if (fclose(source_file))
    {
      fprintf(stderr, "Failed to close one of the source files.\n");
    }
    else
    {
      OPENED_FILES_COUNTER--;
    }
    return 0;
  }

  do
  {
    if (fwrite(buffer, 1, read_blocks, destination_file) != read_blocks)
    {
      fprintf(stderr, "Failed to write to the destination file.\n");
      if (fclose(source_file))
      {
        fprintf(stderr, "Failed to close one of the source files.\n");
      }
      else
      {
        OPENED_FILES_COUNTER--;
      }
      return 0;
    }
  }
  while (!feof(source_file));


  if (fclose(source_file))
  {
    fprintf(stderr, "Failed to close one of the source files.\n");
  }
  else
  {
    OPENED_FILES_COUNTER--;
  }

  return 1;
}

int main(int argc, char * argv [])
{
  int i;
  FILE * destination_file = NULL;
  if (argc <= 1)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  if ((destination_file = fopen(argv [argc - 1], "a")) == NULL)
  {
    fprintf(stderr, "Failed to open the destination file.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  OPENED_FILES_COUNTER++;

  for (i = 1; i < argc - 1; i++)
  {
    if (!appendFile(destination_file, argv [i]))
    {
      if (fclose(destination_file))
      {
        fprintf(stderr, "Failed to close the destination file.\n");
      }
      else
      {
        OPENED_FILES_COUNTER--;
      }
      fprintf(stderr, "Execution failed;\nUnclosed files: %d;\nExiting...\n", OPENED_FILES_COUNTER);
      exit(EXIT_FAILURE);
    }
  }

  if (fclose(destination_file))
  {
    fprintf(stderr, "Failed to close the destination file.\n");
  }
  else
  {
    OPENED_FILES_COUNTER--;
  }


  printf("Execution ended successfully;\nUnclosed files: %d;\nExiting...\n", OPENED_FILES_COUNTER);
  return 0;
}
