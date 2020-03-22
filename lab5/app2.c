#include <stdio.h>
#define BLOCK_LIMIT 4096

unsigned OPENED_FILES_COUNTER;

int copyContent(FILE * source, FILE * destination)
{
  char buffer [4096];
  int read_elements_number;
  while (!feof(source) && !ferror(source))
  {
    read_elements_number = fread(buffer, sizeof(char), BLOCK_LIMIT, source);

    if (fwrite(buffer, sizeof(char), read_elements_number, destination) != read_elements_number)
    {
      fprintf(stderr, "Error while writing data to the destination file.\n");
      return 0;
    }
  }
  if (ferror(source))
  {
    fprintf(stderr, "Error while reading data from the source.\n");
    return 0;
  }

  return 1;
}

int main(int argc, char * argv [])
{
  FILE * source, * destination;
  if (argc != 3)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    return -1;
  }

  if ((source = fopen(argv [1], "r")) == NULL)
  {
    fprintf(stderr, "Couldn't open source file.\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
    return -1;
  }
  OPENED_FILES_COUNTER++;
  if ((destination = fopen(argv [2], "w")) == NULL)
  {
    fprintf(stderr, "Couldn't open destination file.\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
    if (!fclose(source))
    {
      OPENED_FILES_COUNTER--;
    }
    return -1;
  }
  OPENED_FILES_COUNTER++;

  if (copyContent(source, destination))
  {
    printf("The operation was successful.\n");
  }
  else
  {
    printf("The operation failed.\n");
  }

  if (fclose(source))
  {
    fprintf(stderr, "Couldn't close the source file.\n");
  }
  OPENED_FILES_COUNTER--;
  if (fclose(destination))
  {
    fprintf(stderr, "Couldn't close the destination file.\n");
  }
  OPENED_FILES_COUNTER--;

  printf("Execution ended;\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
  return 0;
}
