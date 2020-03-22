#include <stdio.h>
#include "myHexdump.h"
#include "myStdlib.h"

int printFile(FILE * file)
{
  if (fseek(file, 0, SEEK_SET) == -1)
  {
    fprintf(stderr, "printFile() -> Failed to reset cursor in the file");
    return 0;
  }
  if(!myHexdump(file))
  {
    fprintf(stderr, "printFile() -> Failed to print file.\n");
    return 0;
  }
  return 1;
}

int checkLargerFile(FILE * file1, FILE * file2)
{
  if (!feof(file1) && feof(file2))
  {
    printf("The first file is larger.\n");
    if (!printFile(file1))
    {
      fprintf(stderr, "checkLargerFile() -> Failed to determine the larger file.\n");
      return 0;
    }
  }
  else if (feof(file1) && !feof(file2))
  {
    printf("The second file is larger.\n");
    if (!printFile(file2))
    {
      fprintf(stderr, "checkLargerFile() -> Failed to determine the larger file.\n");
      return 0;
    }
  }
  return 1;
}

int compareFiles(FILE * file1, FILE * file2)
{
  char buffer_file1, buffer_file2;
  long current_position;
  while(!feof(file1) && !feof(file2))
  {
    if ((fread(&buffer_file1, 1, 1, file1) != 1 && ferror(file1)) ||
        (fread(&buffer_file2, 1, 1, file2) != 1 && ferror(file2)))
    {
      return 0;
    }

    if ((current_position = ftell(file1)) == -1)
    {
      return 0;
    }

    if (buffer_file1 != buffer_file2)
    {
      printf("Different byte found:\n\t - position in files: %ld;\n\t - value in first file: %02x;\n\t - value in second file: %02x;\n", current_position, buffer_file1, buffer_file2);
    }
  }
  putchar('\n');

  checkLargerFile(file1, file2);

  return 1;
}

int main(int argc, char * argv [])
{
  FILE * file1, * file2;

  if (argc != 3)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  if ((file1 = fopen(argv [1], "r")) == NULL)
  {
    fprintf(stderr, "Failed to open the first file.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  OPENED_FILES_COUNTER++;
  if ((file2 = fopen(argv [2], "r")) == NULL)
  {
    fprintf(stderr, "Failed to open the second file.\nExiting...");
    exit(EXIT_FAILURE);
  }
  OPENED_FILES_COUNTER++;

  if (!compareFiles(file1, file2))
  {
    fprintf(stderr, "Failed to run compareFiles().\n");
  }

  if (fclose(file1))
  {
    fprintf(stderr, "Failed to close the first file.\n");
  }
  else
  {
    OPENED_FILES_COUNTER--;
  }
  if (fclose(file2))
  {
    fprintf(stderr, "Failed to close the second file.\n");
  }
  else
  {
    OPENED_FILES_COUNTER--;
  }

  printf("Execution ended;\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
  return 0;
}
