#include <stdio.h>
#include "myStdlib.h"

typedef struct
{
  char * name;
  float price;
}Product;

void freeProductArray(Product ** prod_array, const unsigned * array_size)
{
  int i;
  for (i = 0; i < *array_size; i++)
  {
    if (prod_array [i]->name)
    {
      free(prod_array [i]->name);
      MALLOC_COUNTER--;
    }
    free(prod_array [i]);
    MALLOC_COUNTER--;
  }
  free(prod_array);
}

Product ** resizeProductArray(Product ** prod_array, unsigned * array_size, const unsigned size_change)
{
  Product ** aux = NULL;

  if ((aux = (Product **)realloc(prod_array, (*array_size + size_change) * sizeof(Product *))) == NULL)
  {
    return NULL;
  }
  (*array_size) ++;
  return aux;
}

/**char * getString(FILE * stream)
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
      MALLOC_COUNTER--;
      return NULL;
    }
    string = aux;

    //MALLOC_COUNTER should only be increased once, on the first iteration
    MALLOC_COUNTER += (!length ? 1 : 0);
    *(string + length) = buffer_char;
    length++;
  }

  // No use setting up the \0 character if we don't have a valid string
  if (string)
  {
    *(string + length) = '\0';
  }
  return string;
}*/

void initProduct(Product * prod)
{
  prod->name = NULL;
  prod->price = 0;
}

/**int isValidFloat(char * number)
{
  char * iterator = number;
  char * point_location = strchr(number, '.');

  if (*iterator == '-')
  {
    iterator++;
  }
  for (; iterator < point_location ; iterator++)
  {
    if (!isdigit(*iterator))
    {
      return 0;
    }
  }

  if (point_location != NULL)
  {
    for (iterator = point_location + 1; *iterator != '\0'; iterator++)
    {
      if (!isdigit(*iterator))
      {
        return 0;
      }
    }
  }
  return 1;
}*/

Product * addProductToArray()
{
  Product * prod = NULL;
  char * aux_price = NULL;

  if ((prod = (Product *)malloc(sizeof(Product))) == NULL)
  {
    return NULL;
  }
  MALLOC_COUNTER++;
  initProduct(prod);

  printf("\t - name of the product: ");
  if ((prod->name = getString(stdin)) == NULL)
  {
    free(prod);
    MALLOC_COUNTER--;
    return NULL;
  }

  printf("\t - price of the product: ");
  if((aux_price = getString(stdin)) == NULL)
  {
    free(prod);
    free(prod->name);
    MALLOC_COUNTER -= 2;
    return NULL;
  }

  if (!isValidFloat(aux_price))
  {
    do
    {
      free(aux_price);
      MALLOC_COUNTER--;
      printf("\t  ! Enter a valid price !\n");
      printf("\t - price of the product: ");
      if((aux_price = getString(stdin)) == NULL)
      {
        free(prod);
        free(prod->name);
        MALLOC_COUNTER -= 2;
        return NULL;
      }
    }
    while(!isValidFloat(aux_price));
  }

  prod->price = (float)atof(aux_price);
  free(aux_price);
  MALLOC_COUNTER--;


  return prod;
}

void printProductArray(Product ** prod_array, const unsigned * array_size)
{
  int i;
  putchar('\n');
  for (i = 0; i < *array_size; i++)
  {
    printf("Product #%d:\n", i);
    printf("\t - product name: %s;\n", prod_array [i]->name);
    printf("\t - product price: %f;\n", prod_array [i]->price);
  }
  putchar('\n');
}

void generateMenu()
{
  printf("-----MENU-----\n\n");
  printf("1. Add product;\n");
  printf("2. Print product list;\n");
  printf("3. Save to file and exit;\n");
  printf("--------------\n\n");
  printf("What would you like to do? ");
}

int writeProductsToDatabase(Product ** prod_array, const unsigned * array_size, char * file_name)
{
  FILE * store_file;
  int i;
  int name_len;

  if ((store_file = fopen(file_name, "w")) == NULL)
  {
    fprintf(stderr, "Failed to open file for the database to be written in.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;

  for (i = 0; i < *array_size; i++)
  {
    name_len = strlen(prod_array [i]->name);
    if (fwrite(&name_len, sizeof(int), 1, store_file) != 1 ||
        fwrite(prod_array [i]->name, sizeof(char), strlen(prod_array [i]->name), store_file) != strlen(prod_array [i]->name) ||
        fwrite(&prod_array [i]->price, sizeof(float), 1, store_file) != 1)
    {
      fprintf(stderr, "Error when reading to file.\n");
      return 0;
    }
  }

  if (fclose(store_file))
  {
    fprintf(stderr, "Error while closing file.\n");
    return 0;
  }
  OPENED_FILES_COUNTER--;

  return 1;

}

int main(int argc, char * argv [])
{
  Product ** prod_array = NULL, ** aux_prod_array = NULL;
  unsigned short option;
  unsigned array_size = 0;

  if (argc != 2)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    return -1;
  }

  do
  {
    generateMenu();
    if (scanf("%hu", &option) != 1)
    {
      freeProductArray(prod_array, &array_size);
      fprintf(stderr, "Error when reading the user's choice.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
      exit(EXIT_FAILURE);
    }
    switch (option)
    {
      case 1:
      {
        printf("Adding product:\n");
        getchar();
        if ((aux_prod_array = resizeProductArray(prod_array, &array_size, 1)) == NULL)
        {
          freeProductArray(prod_array, &array_size);
          fprintf(stderr, "Error when incrementing array\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
          exit(EXIT_FAILURE);
        }
        prod_array = aux_prod_array;

        if ((prod_array [array_size - 1] = addProductToArray()) == NULL)
        {
          freeProductArray(prod_array, &array_size);
          fprintf(stderr, "Error when adding product in array\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
        }
        break;
      }
      case 2:
      {
        printf("Products list:\n");
        printProductArray(prod_array, &array_size);
        break;
      }
      case 3:
      {
        // write to disk

        if (writeProductsToDatabase(prod_array, &array_size, argv [1]) == 0)
        {
          freeProductArray(prod_array, &array_size);
          exit(EXIT_FAILURE);
        }
        break;
      }
      default:
      {
        freeProductArray(prod_array, &array_size);
        fprintf(stderr, "Runtime exception caught.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
        exit(EXIT_FAILURE);
      }
    }
  }
  while (option != 3);


  freeProductArray(prod_array, &array_size);
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);

  return 0;
}
