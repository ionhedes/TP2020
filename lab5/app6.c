/*
  Aplicația 5.6: Să se scrie un program care primește 2 fișiere în linia de comandă
                 și le compară între ele. Pentru fiecare octet diferit găsit,
                 se afișează offsetul acestuia și valorile sale din fiecare fișier,
                 în hexazecimal, pe câte 2 cifre. Dacă fișierele sunt de dimensiuni
                 diferite, în final se vor afișa toți octeții din fișierul mai mare.
                 Dacă fișierele sunt identice, programul nu va afișa nimic.
*/

#include <stdio.h>
#include "myHexdump.h"
#include "myStdlib.h"

// Function to print a whole file using the custom hexdump from the previous exercise
/*
  ARGUMENTS:
    - FILE * file - handler for the file

  RETURN VALUE:
    - SUCCESS - 1
    - FAILURE - 0
*/
int printFile(FILE * file)
{
  // Resetting the cursor to the beginning of the file
  if (fseek(file, 0, SEEK_SET) == -1)
  {
    fprintf(stderr, "printFile() -> Failed to reset cursor in the file");
    return 0;
  }

  // Calling hexdump on our file
  if(!myHexdump(file))
  {
    fprintf(stderr, "printFile() -> Failed to print file.\n");
    return 0;
  }
  return 1;
}

// Function to check which file is larger
/*
  USAGE:
    - only to be used after compareFiles()
    - EOF must be set for at least one of the files for this function to work
    - after determining which file is larger, the function calls printFile() on it

  ARGUMENTS:
    - FILE * file1 - handler for the first file
    - FILE * file2 - handler for the second file

  RETURN VALUE:
    - SUCCESS - 1
    - FAILURE - 0
*/
int checkLargerFile(FILE * file1, FILE * file2)
{
  // Case 1: the first file is larger - EOF hasn't been reached for it
  if (!feof(file1) && feof(file2))
  {
    printf("The first file is larger.\n");
    if (!printFile(file1))
    {
      fprintf(stderr, "checkLargerFile() -> Failed to determine the larger file.\n");
      return 0;
    }
  }
  // Case 2: the second file is larger - EOF hasn't been reached for it
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

// Function to find the differences between 2 files
/*
  USAGE:
    - for every different byte found between the files, the position offset, and
      the values are printed

  ARGUMENTS:
    - FILE * file1 - handler for the first file
    - FILE * file2 - handler for the second file

  RETURN VALUE:
    - SUCCESS - 1
    - FAILURE - 0
*/
int compareFiles(FILE * file1, FILE * file2)
{
  char buffer_file1, buffer_file2; // < buffer for storing the current byte from each file
  long current_position; // < variable to store current position offset in

  // Reading and comparing bytes until we reach EOF in one of the files
  while(!feof(file1) && !feof(file2))
  {
    if ((fread(&buffer_file1, 1, 1, file1) != 1 && ferror(file1)) ||
        (fread(&buffer_file2, 1, 1, file2) != 1 && ferror(file2)))
    {
      return 0;
    }

    // Get current position offset in file using ftell()
    if ((current_position = ftell(file1)) == -1)
    {
      return 0;
    }

    // Where there are different bytes at the same position, print the difference and the position
    if (buffer_file1 != buffer_file2)
    {
      printf("Different byte found:\n\t - position in files: %ld;\n\t - value in first file: %02x;\n\t - value in second file: %02x;\n", current_position, buffer_file1, buffer_file2);
    }
  }
  putchar('\n');

  // After checking differences, print the larger file (if they're not equal,
  // don't print anything then)
  checkLargerFile(file1, file2);

  return 1;
}

int main(int argc, char * argv [])
{
  FILE * file1, * file2; // < buffer for storing the current byte from each file

  // Number of args must be 3: the name of the program and the names of the two files
  if (argc != 3)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  // Opening the files, treating opening errors
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

  // Compare files
  if (!compareFiles(file1, file2))
  {
    fprintf(stderr, "Failed to run compareFiles().\n");
  }

  // Closing the files and exiting
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
