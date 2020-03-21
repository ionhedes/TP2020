#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

unsigned malloc_counter, opened_files_counter;

void freeMat(int ** mat, const int * line_no)
{
  int i;
  for (i = 0; i < *line_no; i++)
  {
    free(mat [i]);
    malloc_counter--;
  }
  free(mat);
  malloc_counter--;
}

char * getString(FILE * stream)
{
  char * string = NULL, * aux = NULL;
  char buffer_char;
  unsigned length = 0;

  // Strings are considered to span until \n
  while((buffer_char = getc(stream)) != '\n')
  {
    if ((aux = (char *)realloc(string, (length + 1) * sizeof(char))) == NULL)
    {
      fprintf(stderr, "getString() error...\n");
      free(string);
      malloc_counter--;
      return NULL;
    }
    string = aux;

    //malloc_counter should only be increased once, on the first iteration
    malloc_counter += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }

  // No use setting up the \0 character if we don't have a valid string
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;
}

int getInt(int * location, int (* validationFunction)(char * number), FILE * stream)
{
  char * aux_element = NULL;

  if ((aux_element = getString(stream)) == NULL)
  {
    return 0;
  }
  if (!validationFunction(aux_element))
  {
    do
    {
      free(aux_element);
      malloc_counter--;
      printf("\t ! Enter a valid integer: ");
      if ((aux_element = getString(stream)) == NULL)
      {
        return 0;
      }
    }
    while (!validationFunction(aux_element));
  }
  *location = atoi(aux_element);
  free(aux_element);
  malloc_counter--;
  return 1;
}

int isValidInteger(char * number)
{
  char * iterator = number;
  if (*iterator == '-')
  {
    iterator++;
  }
  for (; *iterator != '\0'; iterator++)
  {
    if (!isdigit(*iterator))
    {
      return 0;
    }
  }
  return 1;
}

int ** createMatrix(const int * line_no, const int * column_no)
{
  int i;
  int ** mat = NULL;
  if ((mat = (int **)malloc(*line_no * sizeof(int *))) == NULL)
  {
    return NULL;
  }
  malloc_counter++;

  for (i = 0; i < *line_no; i++)
  {
    if ((mat [i] = (int *)malloc(*column_no * sizeof(int))) == NULL)
    {
      freeMat(mat, &i);
      return NULL;
    }
    malloc_counter++;
  }
  return mat;
}

int fillMatrixTextFile(int ** mat, const int * line_no, const int * column_no, FILE * stream)
{
  //char * aux_element = NULL;
  int i, j;
  printf("Populating the matrix:\n");
  for (i = 0; i < *line_no; i++)
  {
    for (j = 0; j < *column_no; j++)
    {
      printf("\t - enter element mat [%d][%d]: ", i, j);
      if (!getInt(&mat [i][j], isValidInteger, stream))
      {
        return 0;
      }
    }
  }
  return 1;
}

int fillMatrixBinaryFile(int ** mat, const int * line_no, const int * column_no, const char * storage_file_name)
{
  int i;
  FILE * storage_file;

  if ((storage_file = fopen(storage_file_name, "r")) == NULL)
  {
    fprintf(stderr, "Failed to open file.\n");
    return 0;
  }
  opened_files_counter++;

  if (fseek(storage_file, 8, SEEK_CUR) == -1)
  {
    if (fclose(storage_file))
    {
      fprintf(stderr, "Failed to close file.\n");
    }
    opened_files_counter--;
    return 0;
  }

  for (i = 0; i < *line_no; i++)
  {
    if (fread(mat [i], sizeof(int), *column_no, storage_file) != *column_no || ferror(storage_file))
    {
      fprintf(stderr, "Could not read line %d.\n", i);
      if (fclose(storage_file))
      {
        fprintf(stderr, "Failed to close file.\n");
      }
      opened_files_counter--;
      return 0;
    }

  }

  if (fclose(storage_file))
  {
    fprintf(stderr, "Failed to close file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
  }
  opened_files_counter--;

  return 1;
}

void printMatrix(int ** mat, const int * line_no, const int * column_no)
{
  int i, j;
  printf("The elements of the matrix:\n");
  for (i = 0; i < *line_no; i++)
  {
    printf("\t - ");
    for (j = 0; j < *column_no; j++)
    {
      printf("%d ", mat [i][j]);
    }
    printf(";\n");
  }
  putchar('\n');
}

int writeMatrixToFile(int ** mat, const int * line_no, const int * column_no, const char * storage_file_name)
{
  FILE * storage_file;
  int i;

  if ((storage_file = fopen(storage_file_name, "w")) == NULL)
  {
    fprintf(stderr, "Failed to open file.\n");
    return 0;
  }
  opened_files_counter++;

  if (fwrite(line_no, sizeof(int), 1, storage_file) != 1 ||
      fwrite(column_no, sizeof(int), 1, storage_file) != 1)
  {
    fprintf(stderr, "Could not write to file.\n");
    if (fclose(storage_file))
    {
      fprintf(stderr, "Failed to close file.\n");
    }
    opened_files_counter--;
    return 0;
  }

  for (i = 0; i < *line_no; i++)
  {
    if (fwrite(mat [i], sizeof(int), *column_no, storage_file) != *column_no)
    {
      fprintf(stderr, "Could not write line %d to file.\n", i);
      if (fclose(storage_file))
      {
        fprintf(stderr, "Failed to close file.\n");
      }
      opened_files_counter--;
      return 0;
    }
  }

  if (fclose(storage_file))
  {
    fprintf(stderr, "Failed to close file.\n");
    return 0;
  }
  opened_files_counter--;

  return 1;
}

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
  int ** mat = NULL;
  int line_no, column_no;
  int option;

  if (argc != 4)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    exit(EXIT_FAILURE);
  }

  if(!isValidInteger(argv [1]) || *(argv [1]) == '-')
  {
    printf("The line number given as a parameter is not valid.\nPlease input another one: ");
    if (!getInt(&line_no, isValidInteger, stdin))
    {
      fprintf(stderr, "Error when reading the line number.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    line_no = atoi(argv [1]);
  }

  if (!isValidInteger(argv [2]) || *(argv [2]) == '-')
  {
    printf("The column number given as a parameter is not valid.\nPlease input another one: ");
    if (!getInt(&column_no, isValidInteger, stdin))
    {
      fprintf(stderr, "Error when reading the column number.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    column_no = atoi(argv [2]);
  }

  if ((mat = createMatrix(&line_no, &column_no)) == NULL)
  {
    fprintf(stderr, "Failed to create matrix.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
    exit(EXIT_FAILURE);
  }


  do
  {
    generateMenu();
    if (!getInt(&option, isValidInteger, stdin))
    {
      freeMat(mat, &line_no);
      fprintf(stderr, "Error when reading the user's choice.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
      exit(EXIT_FAILURE);
    }

    switch(option)
    {
      case 1:
      {
        // write to file
        putchar('\n');
        if (!fillMatrixTextFile(mat, &line_no, &column_no, stdin))
        {
          freeMat(mat, &line_no);
          fprintf(stderr, "Failed to populate matrix.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
          exit(EXIT_FAILURE);
        }
        putchar('\n');
        if (!writeMatrixToFile(mat, &line_no, &column_no, argv [3]))
        {
          freeMat(mat, &line_no);
          fprintf(stderr, "Failed to write matrix to file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
          exit(EXIT_FAILURE);
        }

        break;
      }
      case 2:
      {
        // read from file
        putchar('\n');
        if (!fillMatrixBinaryFile(mat, &line_no, &column_no, argv [3]))
        {
          freeMat(mat, &line_no);
          fprintf(stderr, "Failed to write matrix to file.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
          exit(EXIT_FAILURE);
        }
        putchar('\n');

        printMatrix(mat, &line_no, &column_no);

        break;
      }
      case 3:
      {
        // exit
        break;
      }
      default:
      {
        freeMat(mat, &line_no);
        fprintf(stderr, "Runtime exception caught.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
        exit(EXIT_FAILURE);
      }
    }

  }
  while (option != 3);

  freeMat(mat, &line_no);
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", malloc_counter, opened_files_counter);
  return 0;
}
