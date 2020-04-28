/**
  Aplicația 9.5: Să se scrie o funcție care primește două liste și returnează 1
                 dacă ele sunt egale, indiferent de ordinea și numărul elementelor,
                 altfel returnează 0.

  Exemple: listele {1, 7, 3, 1, 3} și {7, 1, 3, 7} sunt egale.
           Listele {1,2} și {2} nu sunt egale.
*/

#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

// Call the list handler creation macro for the int type
// Check listHandler.h for details
create_list_handler(int)

// Function to find out if a value can be found in a list
/**
  USAGE:
    - self explanatory;
    - this function is restricted to integer values and lists;

  ARGUMENTS:
    - const Node_int * list - the list where to look for the element;
    - const int element - the value to look for;

  RETURN VALUE:
    - 1 - the element was found;
    - 0 - the element was not found;

  SIDE EFFECTS:
    - no outside values will be modified by this function;
*/
int belongsToList(const int * element, const Node_int * list)
{
  const Node_int * iter = NULL;
  for (iter = list; iter; iter = iter->next)
  {
    if (iter->info == *element)
    {
      return 1;
    }
  }
  return 0;
}

// Function to determine if two lists are equivalent
/**
  USAGE:
    - returns whether or nor two list contain the same values;
    - repeating elements don't affect the result;
    - order of the elements between the lists also doesn't affect the result

  ARGUMENTS:
    - const Node_int * list1 - the first list;
    - const Node_int * list2 - the second list;

  RETURN VALUE:
    - 1 - the lists are equivalent
    - 0 - the lists are not equivalent

  SIDE EFFECTS:
    - no outside values will be modified by this function;
*/
int areEquivalentLists(const Node_int * list1, const Node_int * list2)
{
  const Node_int * iter = NULL;

  DEB("Iterating the lists to find out if they are equivalent...\n");

  DEB("Looking for list-1 exclusives:\n");
  for (iter = list1; iter; iter = iter->next)
  {
    DEB("\t - currently checking element %d;\n", iter->info);
    if (!belongsToList(&iter->info, list2))
    {
      DEB("\t\t - the element was not found inside the other list;\n");
      DEB("Done.\n");
      return 0;
    }
    DEB("\t\t - the element was found inside the other list;\n");
  }

  DEB("Looking for list-2 exclusives:\n");
  for (iter = list2; iter; iter = iter->next)
  {
    DEB("\t - currently checking element %d;\n", iter->info);
    if (!belongsToList(&iter->info, list1))
    {
      DEB("\t\t - the element was not found inside the other list;\n");
      DEB("Done.\n");
      return 0;
    }
    DEB("\t\t - the element was found inside the other list;\n");
  }

  DEB("Done.\n");
  return 1;
}

// Driver program for the previous function
int main()
{
  // Necessary variables for the two lists
  unsigned list1_size = 0;
  unsigned list2_size = 0;
  Node_int * list1 = NULL;
  Node_int * list2 = NULL;

  // Creating & filling the first list
  printf("Enter the desired size of the first list: ");
  if (!get_unsigned(&list1_size, stdin))
  {
    // Treating input errors
    fprintf(stderr, "Input error when reading the first list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list1 = createList_int(&list1_size)) == NULL)
  {
    // Treating input or allocation errors
    fprintf(stderr, "The creation of the first list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list1); // < Printing the list to check if all went good

  putchar('\n');

  // Creating & filling the second list
  printf("Enter the desired size of the second list: ");
  if (!get_unsigned(&list2_size, stdin))
  {
    // Treating input errors
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
    freeList_int(list1);
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list2 = createList_int(&list2_size)) == NULL)
  {
    // Treating input or allocation errors;
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
    freeList_int(list1);
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list2); // < Printing the list to check if all went good

  putchar('\n');

  // Running the tested function
  // Checking if the lists are equivalent...
  if (areEquivalentLists(list1, list2))
  {
    putchar('\n');
    printf("The lists are equivalent.\n");
  }
  else
  {
    putchar('\n');
    printf("The lists have some different values.\n");
  }

  putchar('\n');

  // Free the memory and end execution
  freeList_int(list1);
  freeList_int(list2);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
