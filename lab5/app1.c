/**
  Aplicația 5.1: Modificați exemplul anterior, astfel încât să se scrie
                 la început de fișier numărul de produse din baza de date.
*/

#include <stdio.h>
#include "myStdlib.h" // < 'proprietary' :P input functions here

typedef struct
{
  char * name; // < the name of a product - will be dinamically allocated
  float price; // < the price of a product
}Product;

// Function used to free the memory blocks used for the product array
/*
  USAGE:
    - use before ending execution, or when an error occured to free the memory
      used by the product array
  ARGUMENTS:
    - Product ** prod_array - the array of products
    - const unsigned * array_size - the number of products currently in the array
*/
void freeProductArray(Product ** prod_array, const unsigned * array_size)
{
  int i;
  for (i = 0; i < *array_size; i++)
  {
    // Only free the name string if the pointer is not NULL
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

// Function to resize the product array
/*
  USAGE:
    - when trying to add or remove a product
  ARGUMENTS:
    - Product ** prod_array - the array of products
    - const unsigned * array_size - the number of products currently in the array
    - const unsigned size_change - the rate of change in the array_size
                                 - 1 when adding an item to the array
                                 - -1 when removing an item from the array
*/
Product ** resizeProductArray(Product ** prod_array, unsigned * array_size, const unsigned size_change)
{
  Product ** aux = NULL; // < Auxilliary pointer for using realloc() on

  if ((aux = (Product **)realloc(prod_array, (*array_size + size_change) * sizeof(Product *))) == NULL)
  {
    return NULL;
  }
  // Only increasing array size if the realloc() is succesful
  // Otherwise, it won't handshake with freeProductArray(), making it segfault
  (*array_size) ++;
  return aux;
}

// Function to initialize the fields of a freshly created product
// (no data inside yet)
/*
  USAGE:
    - some kind of constructor
    - right after a succesful malloc(sizeof(Product))
  ARGUMENTS:
    - Product * prod - pointer to the product structure
*/
void initProduct(Product * prod)
{
  prod->name = NULL; // < Setting this to NULL to make sure we don't try to free
                     // this pointer afterwards, befor a prior malloc()
                     // The C documentation states that freeing a NULL pointer
                     // has no effect, as opposed to the undefined behaviour of
                     // freeing something not created by malloc()
  prod->price = 0;
}


// Function to malloc() a structure and return its pointer
/*
  USAGE:
    - when adding a product to the array
  RETURN VALUE:
    - SUCCESS - pointer to the new structure
    - FAILURE - NULL, in case of malloc() error on the product or the string
*/
Product * addProductToArray()
{
  Product * prod = NULL; // < The pointer which will be returned
  char * aux_price = NULL; // < Will be used for reading the price
                           // See myStdlib.h for how it works

  if ((prod = (Product *)malloc(sizeof(Product))) == NULL)
  {
    return NULL;
  }
  MALLOC_COUNTER++;

  //After succesfully allocating space for the object, call its constructor
  initProduct(prod);

  // Filling the object (product) with data
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

  // Did not create a function getFloat() inside myStdlib.h yet, so the behaviour
  // is simulated by this loop
  // See getInt() from myStdlib.h for how this works, it is virtually the same thing,
  // but with float instead of int
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

// Function to print the array of products
/*
  USAGE:
    - literally prints them
  ARGUMENTS:
    - Product ** prod_array - the array of products
    - const unsigned * array_size - the number of products currently in the array
*/
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

// Function to print the menu with available operations
void generateMenu()
{
  printf("-----MENU-----\n\n");
  printf("1. Add product;\n");
  printf("2. Print product list;\n");
  printf("3. Save to file and exit;\n");
  printf("--------------\n\n");
  printf("What would you like to do? ");
}

// Function to write the database of products to a file
/*
  USAGE:
    - before terminating execution, write the array of products to a file
    - the file must be binary

  ARGUMENTS:
    - Product ** prod_array - the array of products
    - const unsigned * array_size - the number of products currently in the array
    - char * file_name - the file name where we save the data, as taken from argv []

  RETURN VALUE:
    - SUCCESS - 1
    - FAILURE - 0
*/
int writeProductsToDatabase(Product ** prod_array, const unsigned * array_size, char * file_name)
{
  FILE * store_file = NULL; // < pointer to use in fopen() as the file handler
  int i;
  int name_len; // < before the name of the product, we need to store the name length
                // for each product

  // Opening the file for writing
  if ((store_file = fopen(file_name, "w")) == NULL)
  {
    fprintf(stderr, "Failed to open file for the database to be written in.\n");
    return 0;
  }
  OPENED_FILES_COUNTER++;

  for (i = 0; i < *array_size; i++)
  {
    name_len = strlen(prod_array [i]->name);

    // Check man fwrite() for return values, errors and why I did this
    if (fwrite(&name_len, sizeof(int), 1, store_file) != 1 ||
        fwrite(prod_array [i]->name, sizeof(char), strlen(prod_array [i]->name), store_file) != strlen(prod_array [i]->name) ||
        fwrite(&prod_array [i]->price, sizeof(float), 1, store_file) != 1)
    {
      fprintf(stderr, "Error when writing to file.\n");
      return 0;
    }
  }

  // Closing the file
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
  Product ** prod_array = NULL, ** aux_prod_array = NULL; // < pointers for
                                                          // the product array
  int option; // < variable used to store the input option
  unsigned array_size = 0; // < number of products

  // Number of args must be 2: the name of the program and the name of the file
  // to write to
  if (argc != 2)
  {
    fprintf(stderr, "Invalid number of parameters.\nExiting...\n");
    return -1;
  }

  do
  {
    // Print the menu and wait for the input option
    generateMenu();
    // Check myStdlib.h for details on getInt()
    if (!getInt(&option, isValidInteger, stdin))
    {
      freeProductArray(prod_array, &array_size);
      fprintf(stderr, "Error when reading the user's choice.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
      exit(EXIT_FAILURE);
    }

    // Act accordingly to the input option
    switch (option)
    {
      case 1:
      {
        printf("Adding product:\n");
        /** For adding: - resize the current array
                        - put a new structure in the array
        */
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
        // Just calling the printing function
        printProductArray(prod_array, &array_size);
        break;
      }
      case 3:
      {
        // Just calling the write function
        if (writeProductsToDatabase(prod_array, &array_size, argv [1]) == 0)
        {
          freeProductArray(prod_array, &array_size);
          exit(EXIT_FAILURE);
        }
        break;
      }
      default:
      {
        // Exception, exception, why are you here ? :(
        freeProductArray(prod_array, &array_size);
        fprintf(stderr, "Runtime exception caught.\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
        exit(EXIT_FAILURE);
      }
    }
  }
  while (option != 3);

  // Free everythin that is to be freed (malloc()-wise) before ending execution
  freeProductArray(prod_array, &array_size);
  printf("Execution ended successfully;\nUnreleased memory blocks: %d;\nUnclosed files: %d\nExiting...\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);

  return 0;
}
