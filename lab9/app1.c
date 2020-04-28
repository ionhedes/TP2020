/**
  Aplicația 9.1: Să se scrie o funcție care primește două liste și returnează 1
                 dacă ele sunt identice, altfel 0.
*/

#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

// Call the list handler creation macro for the int type
// Check listHandler.h for details
create_list_handler(int)

// Function to check if two lists are identical
/**
  USAGE:
    - equality function for lists;

  ARGUMENTS:
    - const Node_int * list1 - the first list;
    - const Node_int * list2 - the second list;

  RETURN VALUE:
    - 1 - the lists are identical;
    - 0 - the lists are diferent;

  SIDE EFFECTS:
    - no outside values will be modified by this function
*/
int areIdenticalLists(const Node_int * list1, const Node_int * list2)
{
  const Node_int * iter1 = NULL;
  const Node_int * iter2 = NULL;

  if (getSizeList_int(list1) != getSizeList_int(list2))
  {
    DEB("The two lists have different sizes, so they are different.\n");
    return 0;
  }

  DEB("Iterating to check for different values:\n");
  for (iter1 = list1, iter2 = list2; (iter1 != NULL) && (iter2 != NULL); iter1 = iter1->next, iter2 = iter2->next)
  {
    DEB("\t - current values:\n");
    DEB("\t\t - list1: %d;\n", iter1->info);
    DEB("\t\t - list2: %d;\n", iter2->info);
    if (iter1->info != iter2->info)
    {
      DEB("\t\t - the elements are different;\n");
      DEB("Done.\n");
      return 0;
    }
  }
  DEB("Done!\n");
  return 1;
}

// Driver program for the aforementioned function
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
    fprintf(stderr, "Input error when reading the second list's size.\nExiting...\n");
    freeList_int(list1); // < Releasing the first list before aborting exe.
    exit(EXIT_FAILURE);
  }
  if ((list2 = createList_int(&list2_size)) == NULL)
  {
    // Treating input or allocation errors
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
    freeList_int(list1); // < Releasing the first list before aborting exe.
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list2); // < Printing the list to check if all went good

  putchar('\n');

  // Running the tested function
  if (areIdenticalLists(list1, list2))
  {
    printf("The lists are identical.\n");
  }
  else
  {
    printf("The lists are different.\n");
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
