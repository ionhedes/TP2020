/**
  Aplicația 5.3: Se citesc m și n de la tastatură, iar apoi o matrice a[m][n]
                 cu elemente de tip întreg. Să se scrie matricea într-un fișier binar, prima
                 oară scriindu-se m și n, iar apoi elementele, așa cum sunt dispuse ele în memorie.
                 Să se citească matricea din fișier într-o variabilă b și să se afișeze.
*/

#include <stdio.h>
#include "myStdlib.h"

// Function used to free the memory blocks used for the matrix
/*
  USAGE:
    - use before ending execution, or when an error occured to free the memory
      used by the matrix
  ARGUMENTS:
    - int ** mat - the matrix
    - const int * line_no - pointer to the number of lines in the matrix
*/
void freeMat(int ** mat, const int * line_no)
{
  // First we free every line array, and then we free the pointer array representing
  // the matrix itself
  int i;
  for (i = 0; i < *line_no; i++)
  {
    free(mat [i]);
    MALLOC_COUNTER--;
  }
  free(mat);
  MALLOC_COUNTER--;
}

// Function used to create the matrix
/*
    USAGE:
      - creates an array of pointers for every line of the matrix
      - for every entry in the pointer array, creates an int array of size
         collumn_number
    ARGUMENTS:
      - const int * line_no - number of lines of the matrix
      - const int * matrix_no - number of collumns of the matrix
*/
int ** createMatrix(const int * line_no, const int * collumn_no)
{
  int i;
  int ** mat = NULL; // < pointer for the matrix (a bit vague, sorry)

  // Creating an array of pointers, each pointer for a row
  if ((mat = (int **)malloc(*line_no * sizeof(int *))) == NULL)
  {
    return NULL;
  }
  MALLOC_COUNTER++;

  // For each row pointer, create an array to which it points to
  for (i = 0; i < *line_no; i++)
  {
    if ((mat [i] = (int *)malloc(*collumn_no * sizeof(int))) == NULL)
    {
      freeMat(mat, &i);
      return NULL;
    }
    MALLOC_COUNTER++;
  }
  return mat;
}

// Function to fill matrix with data from stdin or a text file
/*
    ARGUMENTS:
      - int ** mat - pointer to the the matrix
      - const int * line_no - pointer to the line number
      - const int * collumn_no - pointer to the collumn number
      - FILE * stream - file handler for where to get the data from

    RETURN VALUE:
      - SUCCESS - 1
      - FAILURE - 0
*/
int fillMatrixFromTextFile(int ** mat, const int * line_no, const int * collumn_no, FILE * stream)
{
  int i, j;
  printf("Populating the matrix:\n");
  for (i = 0; i < *line_no; i++)
  {
    for (j = 0; j < *collumn_no; j++)
    {
      printf("\t - enter element mat [%d][%d]: ", i, j);
      // Check myStdlib.h for details on getInt()
      if (!getInt(&mat [i][j], isValidInteger, stream))
      {
        return 0;
      }
    }
  }
  return 1;
}

// Function to fill matrix with data from a binary file
/*
    USAGE:
      - only works with binary files

    ARGUMENTS:
      - int ** mat - pointer to the the matrix
      - const int * line_no - pointer to the line number
      - const int * collumn_no - pointer to the collumn number
      - const char * storage_file_name - string with name of the file handler
                                         for where to get the data from

    RETURN VALUE:
      - SUCCESS - 1
      - FAILURE - 0
*/
int fillMatrixFromBinaryFile(int ** mat, const int * line_no, const int * collumn_no, const char * storage_file_name)
{
  int i;
  FILE * storage_file = NULL; // < pointer/handler for the source file

  // Opening the storage file
  if ((storage_file = fopen(storage_file_name, "r")) == NULL)
  {
    fprintf(stderr, "Failed to open file.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;

  // At offset 0, the file should have data about the number of lines and collumns
  // We skip that because we already receive those from the command line, and I
  // had no time for a workaround
  if (fseek(storage_file, 2 * sizeof(int), SEEK_CUR) == -1)
  {
    if (fclose(storage_file))
    {
      fprintf(stderr, "Failed to close file.\n");
    }
    OPENED_FILES_COUNTER--;
    return 0;
  }

  // Reading one whole line at a time
  for (i = 0; i < *line_no; i++)
  {
    if (fread(mat [i], sizeof(int), *collumn_no, storage_file) != *collumn_no || ferror(storage_file))
    {
      fprintf(stderr, "Could not read line %d.\n", i);
      if (fclose(storage_file))
      {
        fprintf(stderr, "Failed to close file.\n");
      }
      OPENED_FILES_COUNTER--;
      return 0;
    }

  }

  // Closing the storage file
  if (fclose(storage_file))
  {
    fprintf(stderr, "Failed to close file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
  }
  OPENED_FILES_COUNTER--;

  return 1;
}

// Function to print the matrix to stdout
/*
  USAGE:
    - this function only works on the matrix that is stored in memory, not the
      one in the file

  ARGUMENTS:
    - int ** mat - pointer to the the matrix
    - const int * line_no - pointer to the line number
    - const int * collumn_no - pointer to the collumn number
*/
void printMatrix(int ** mat, const int * line_no, const int * collumn_no)
{
  int i, j;
  printf("The elements of the matrix:\n");
  for (i = 0; i < *line_no; i++)
  {
    printf("\t - ");
    for (j = 0; j < *collumn_no; j++)
    {
      printf("%d ", mat [i][j]);
    }
    printf(";\n");
  }
  putchar('\n');
}

// Function to write the matrix to a file
/*
  ARGUMENTS:
    - int ** mat - pointer to the the matrix
    - const int * line_no - pointer to the line number
    - const int * collumn_no - pointer to the collumn number
    - const char * storage_file_name - string with name of the file handler
                                       for where to put the data

  RETURN VALUE:
    - SUCCESS - 1
    - FAILURE - 0
*/
int writeMatrixToFile(int ** mat, const int * line_no, const int * collumn_no, const char * storage_file_name)
{
  FILE * storage_file; // < pointer/handler for the destination file
  int i;

  // Opening the file
  if ((storage_file = fopen(storage_file_name, "w")) == NULL)
  {
    fprintf(stderr, "Failed to open file.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;

  // Write the line and collumn numbers to the file
  if (fwrite(line_no, sizeof(int), 1, storage_file) != 1 ||
      fwrite(collumn_no, sizeof(int), 1, storage_file) != 1)
  {
    fprintf(stderr, "Could not write to file.\n");
    if (fclose(storage_file))
    {
      fprintf(stderr, "Failed to close file.\n");
    }
    OPENED_FILES_COUNTER--;
    return 0;
  }

  // Write the elements of the matrix to the file, one row at a time
  for (i = 0; i < *line_no; i++)
  {
    if (fwrite(mat [i], sizeof(int), *collumn_no, storage_file) != *collumn_no)
    {
      fprintf(stderr, "Could not write line %d to file.\n", i);
      if (fclose(storage_file))
      {
        fprintf(stderr, "Failed to close file.\n");
      }
      OPENED_FILES_COUNTER--;
      return 0;
    }
  }

  // Close the destination file
  if (fclose(storage_file))
  {
    fprintf(stderr, "Failed to close file.\n");
    return 0;
  }
  OPENED_FILES_COUNTER--;

  return 1;
}

// Function to print the menu with available operations
void generateMenu()
{
  printf("-----MENU-----\n\n");
  printf("1. Write matrix to file;\n");
  printf("2. Read matrix from file;\n");
  printf("3. Exit;\n");
  printf("--------------\n\n");
  printf("What would you like to do? ");
}

int main(int argc, char * argv [])
{
  int ** mat = NULL; // < pointer for the matrix (a bit vague, sorry)
  int line_no, collumn_no; // < data regargind the dimensions of the matrix
  int option; // < variable used to store the input option

  // Number of args must be 4: the name of the program, the dimensions of the matrix,
  // and the name of the file to write to / read from
  if (argc != 4)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  // Checking the validity of the given dimensions of the matrix
  // If they're not valid, asking for new ones until good values are given
  // Check myStdlib.h for information on getInt() and isValidInteger()
  if(!isValidInteger(argv [1]) || *(argv [1]) == '-')
  {
    printf("The line number given as a parameter is not valid.\nPlease input another one: ");
    if (!getInt(&line_no, isValidInteger, stdin))
    {
      fprintf(stderr, "Error when reading the line number.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    line_no = atoi(argv [1]);
  }

  if (!isValidInteger(argv [2]) || *(argv [2]) == '-')
  {
    printf("The collumn number given as a parameter is not valid.\nPlease input another one: ");
    if (!getInt(&collumn_no, isValidInteger, stdin))
    {
      fprintf(stderr, "Error when reading the collumn number.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    collumn_no = atoi(argv [2]);
  }

  // Create the matrix
  if ((mat = createMatrix(&line_no, &collumn_no)) == NULL)
  {
    fprintf(stderr, "Failed to create matrix.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
    exit(EXIT_FAILURE);
  }


  do
  {
    generateMenu();
    if (!getInt(&option, isValidInteger, stdin))
    {
      freeMat(mat, &line_no);
      fprintf(stderr, "Error when reading the user's choice.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
      exit(EXIT_FAILURE);
    }

    switch(option)
    {
      case 1:
      {
        // Write the matrix to a file, after filling the matrix with data from a text file, or stdin
        putchar('\n');
        if (!fillMatrixFromTextFile(mat, &line_no, &collumn_no, stdin))
        {
          freeMat(mat, &line_no);
          fprintf(stderr, "Failed to populate matrix.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
          exit(EXIT_FAILURE);
        }
        putchar('\n');
        if (!writeMatrixToFile(mat, &line_no, &collumn_no, argv [3]))
        {
          freeMat(mat, &line_no);
          fprintf(stderr, "Failed to write matrix to file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
          exit(EXIT_FAILURE);
        }

        break;
      }
      case 2:
      {
        // Read the matrix from a file
        putchar('\n');
        if (!fillMatrixFromBinaryFile(mat, &line_no, &collumn_no, argv [3]))
        {
          freeMat(mat, &line_no);
          fprintf(stderr, "Failed to write matrix to file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
          exit(EXIT_FAILURE);
        }
        putchar('\n');

        printMatrix(mat, &line_no, &collumn_no);

        break;
      }
      case 3:
      {
        // Just exit
        printf("Exiting...");
        break;
      }
      default:
      {
        // Exception, exception, why are you here ? :(
        freeMat(mat, &line_no);
        fprintf(stderr, "Runtime exception caught.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
        exit(EXIT_FAILURE);
      }
    }

  }
  while (option != 3);

  // Free everythin that is to be freed (malloc()-wise) before ending execution
  freeMat(mat, &line_no);
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
  return 0;
}
