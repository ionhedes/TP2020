/**
  Aplicația 9.2: Să se scrie o funcție care primește o listă și returnează lista
                 respectivă cu elementele inversate. Funcția va acționa doar
                 asupra listei originare, fără a folosi vectori sau alocare de
                 noi elemente.
*/

#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

// Call the list handler creation macro for the int type
// Check listHandler.h for details
create_list_handler(int)

// Function to invert a list
/**
  USAGE:
    - pass it a list to be inverted;
    - iterating the list using three pointers, for the previous, current and
      future nodes, the links between nodes are reversed;

  ARGUMENTS:
    - Node_int * list - the list to be reversed;

  RETURN VALUE:
    - success - pointer to the head of the list
    - failure - undefined

  SIDE EFFECTS:
    - the initial order of the list is lost;
*/
Node_int * invertList(Node_int * list)
{
  Node_int * current = list;
  Node_int * previous = NULL;
  Node_int * next = NULL;

  DEB("Reversing list...\n");
  while (current != NULL)
  {
    next = current->next; // < It is necessary to only assign the next pointer here.
                          //   Else, we may try to access null->next,
                          //   which will give segfault;
    current->next = previous; // < Reversing the link;
    previous = current;
    current = next;
  }
  DEB("Done.\n");

  return previous; // < The last assigned previous node is equal to the new head;
}

// Driver program for the aforementioned function
int main()
{
  // Necessary variables
  unsigned list_size = 0;
  Node_int * list = NULL;

  // Creating & filling the list
  printf("Enter the desired size of the list: ");
  if (!get_unsigned(&list_size, stdin))
  {
    // Treating input errors;
    fprintf(stderr, "Input error when reading the list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list = createList_int(&list_size)) == NULL)
  {
    // Treating input or allocation errors;
    fprintf(stderr, "The creation of the list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list); // < Printing the list to check if all went good;

  list = invertList(list); // < Reversing the list using the function;
  putchar('\n');
  printList_int(list); // < Printing the list reversed to check if all went good;

  putchar('\n');

  // Free the memory and end execution
  freeList_int(list);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
