/**
  Aplicația 5.7: Un program primește în linia de comandă un nume de fișier,
                 un offset dat în hexazecimal și apoi o serie de octeți,
                 specificați tot în hexazecimal. Programul va scrie în fișierul
                 specificat, începând de la offsetul dat toți octeții primiți
                 în linia de comandă. Restul fișierului va rămâne nemodificat.
  Exemplu: ./inlocuire 1.dat 4a0f 9e b0 51    -> scrie octeții {9e, b0, 51} în fișierul 1.dat, începând cu poziția 4a0f
*/

#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

// Function to extract the address to write at from argv[i]
int extractAddress(char * addr_sform, long * addr_iform)
{
  long aux_integer;
  int i;
  for (i = 0; i < strlen(addr_sform); i++)
  {
    if (!isValidHexChar(&addr_sform [i]))
    {
      return 0;
    }
  }
  aux_integer = strtol(addr_sform, NULL, 16);
  if (errno == ERANGE || errno == EINVAL)
  {
    perror("Error when converting a parameter to a byte.\n");
    return 0;
  }

  *addr_iform = aux_integer;
  return 1;
}

// Function to extract the byte sequence to write from argv[i] and put it inside
// unsigned an char array
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

int areByteSeqEqual(const unsigned char * seq1, const unsigned char * seq2)
{
  for (; *seq1 != '\0 && *seq2 != '\0; seq1++, seq2++)
  {
    if (*seq1 != *seq2)
    {
      return 0;
    }
  }
  return 1;
}

int writeInFile(FILE * file, long * address, const unsigned char * bytes, const int bytes_no)
{
  if (fseek(file, *address, SEEK_SET) == -1)
  {
    perror("Could not set cursor in to the writing address.\n");
    return 0;
  }
  if (fwrite(bytes, 1, bytes_no, file) != bytes_no)
  {
    return 0;
  }
  return 1;
}

int findSequence(FILE * file, const unsigned char * bytes, const int bytes_no)
{
  long position;
  unsigned char buffer [bytes_no];
  while(!feof(file))
  {
    if (fread(buffer, bytes_no, 1, file) != 1 || ferror(file))
    {
      return 0;
    }
    if (areByteSeqEqual(bytes, buffer) == 1)
    {
      if ((position = ftell(file)) == -1)
      {
        return 0;
      }
      printf("Found sequence at position: %08lx;\n");
    }
  }
  return 1;
}

int main(int argc, char * argv [])
{
  FILE * file = NULL; // < file handler
  unsigned char * bytes = NULL; // < the string with the bytes to be written
  long address; // < address to write them at

  /* Number of args must be higher than 2:
      - the name of the program
      - the name of the file to write to
      - the address where to write in the file
      - sequence of bits to be written
  */
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

  if (!extractBytes(argv + 3, argc - 3, bytes))
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
  if (!extractAddress(argv [2], &address))
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
    fprintf(stderr, "Could not parse bits.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
    exit(EXIT_FAILURE);
  }

  if (!writeInFile(file, &address, bytes, argc - 3))
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
    fprintf(stderr, "Failed to write to file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
    exit(EXIT_FAILURE);
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
