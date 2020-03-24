/**
  Aplicația 5.2: Scrieți un program care copiază conținutul unui fișier sursă
                 într-un fișier destinație. Numele fișierelor se citesc
                 din linia de comandă. Pentru eficiența copierii, programul
                 va citi câte un bloc de maxim 4096 de octeți,
                 pe care îl va scrie în destinație.
  Exemplu: ./copiere src.dat dst.dat         -> copiază src.dat în dst.dat
*/

#include <stdio.h>
#define BLOCK_LIMIT 4096

// The following global variables are used for debugging
// They keep track of malloc()/free() and fopen()/fclose() calls
// Sadly, I could not find a better way of managing these
unsigned OPENED_FILES_COUNTER;

// Function to copy one file to another
/**
  USAGE:
    - copies blocks of BLOCK_LIMIT bytes from source to destination

  ARGUMENTS:
    - FILE * source - handler for the source file
    - FILE * destination - handler for the destination file

  RETURN VALUE:
    - SUCCESS - 1
    - FAILURE - 0
*/
int copyContent(FILE * source, FILE * destination)
{
  char buffer [BLOCK_LIMIT]; // < buffer to store bytes after reading and before writing
  int read_elements_number; // < when reaching EOF in src, the number of read
                            // files will be smaller than BLOCK_LIMIT.
                            // We need to keep that in mind, so we can write the
                            // correct number of elements(bytes)

  // Reading new lines until EOF is reached, or the error flag is set for the file
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
  FILE * source = NULL; // < pointer/handler for the source file
  FILE * destination = NULL; // < pointer/handler for the destination file

  // Number of args must be 3: the name of the program and the names of the files
  // to read from and write to
  if (argc != 3)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    return -1;
  }

  // Opening the source file for reading
  if ((source = fopen(argv [1], "r")) == NULL)
  {
    fprintf(stderr, "Couldn't open source file.\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
    return -1;
  }
  OPENED_FILES_COUNTER++;

  // Opening the destination file for writing
  if ((destination = fopen(argv [2], "w")) == NULL)
  {
    fprintf(stderr, "Couldn't open destination file.\nUnclosed files: %d\nExiting...\n", OPENED_FILES_COUNTER);
    // In case of failure, close the previously opened file
    if (!fclose(source))
    {
      OPENED_FILES_COUNTER--;
    }
    return -1;
  }
  OPENED_FILES_COUNTER++;

  // Copy the content
  if (copyContent(source, destination))
  {
    printf("The operation was successful.\n");
  }
  else
  {
    printf("The operation failed.\n");
  }

  // Close the files and end execution
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
