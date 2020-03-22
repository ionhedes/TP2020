#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

int extractBytes(char ** bytes_strform, const int bytes_no, unsigned char * bytes_intform)
{
  long aux_integer;
  int i;
  for (i = 0; i < bytes_no; i++)
  {
    if (!isValidHexByte(bytes_strform [i]))
    {
      fprintf(stderr, "One of the parameters is invalid.\n");
      return 0;
    }
    aux_integer = strtol(bytes_strform [i], NULL, 16);
    if (errno == ERANGE || errno == EINVAL)
    {
      perror("Error when converting a parameter to a byte.\n");
      return 0;
    }
    bytes_intform [i] = (unsigned char)aux_integer;
  }
  return 1;
}

int areByteSeqEqual(const unsigned char * seq1, const unsigned char * seq2, const int bytes_no)
{
  int i;
  for (i = 0; i < bytes_no; i++)
  {
    if (seq1 [i] != seq2 [i])
    {
      return 0;
    }
  }
  return 1;
}

void shiftBuffer(unsigned char * buffer, const int * bytes_no)
{
  int i;
  for (i = 0; i < *bytes_no - 1; i++)
  {
    buffer [i] = buffer [i + 1];
  }
}

int findSequence(FILE * file, const unsigned char * bytes, const int bytes_no)
{
  long position;
  unsigned char buffer [bytes_no];

  if (fread(buffer, 1, bytes_no, file) != bytes_no)
  {
    return 0;
  }
  do
  {
    if (areByteSeqEqual(bytes, buffer, bytes_no) == 1)
    {
      if ((position = ftell(file)) == -1)
      {
        return 0;
      }
      printf("Found sequence at position: %08lx;\n", position);
    }
    shiftBuffer(buffer, &bytes_no);
    if (fread(&buffer [bytes_no - 1], 1, 1, file) != 1 && ferror(file))
    {
      return 0;
    }
  }
  while(!feof(file));
  return 1;
}

int main(int argc, char * argv [])
{
  FILE * file = NULL;
  unsigned char * bytes = NULL;

  if (argc < 3)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  if ((file = fopen(argv [1], "r+")) == NULL)
  {
    fprintf(stderr, "Failed to open the file.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  OPENED_FILES_COUNTER++;

  if ((bytes = (unsigned char *)malloc((argc - 2) * sizeof(unsigned char))) == NULL)
  {
    if (fclose(file))
    {
      fprintf(stderr, "Failed to close the file.\n");
    }
    else
    {
      OPENED_FILES_COUNTER--;
    }
    fprintf(stderr, "Could not set up space for the storage of a buffer.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
    exit(EXIT_FAILURE);
  }
  MALLOC_COUNTER++;

  if (!extractBytes(argv + 2, argc - 2, bytes))
  {
    free(bytes);
    MALLOC_COUNTER--;
    if (fclose(file))
    {
      fprintf(stderr, "Failed to close the file.\n");
    }
    else
    {
      OPENED_FILES_COUNTER--;
    }
    fprintf(stderr, "Could not parse bits.\n");
    exit(EXIT_FAILURE);
  }


  if (!findSequence(file, bytes, argc - 2))
  {
    fprintf(stderr, "Failed to parse file.\n");
  }

  if (fclose(file))
  {
    fprintf(stderr, "Failed to close the file.\n");
  }
  else
  {
    OPENED_FILES_COUNTER--;
  }

  free(bytes);
  MALLOC_COUNTER--;
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
  return 0;
}
