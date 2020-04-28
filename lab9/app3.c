/**
  Aplicația 9.3: Să se scrie o funcție care primește ca parametri două liste și
                 returnează o listă care reprezintă reuniunea elementelor lor,
                 fiecare element apărând o singură dată, chiar dacă în listele
                 originare el este duplicat.
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
int belongsToList(const Node_int * list, const int element)
{
  const Node_int * iter = NULL;
  for (iter = list; iter; iter = iter->next)
  {
    if (iter->info == element)
    {
      return 1;
    }
  }
  return 0;
}

// Function to create a single list from two different ones
/**
  USAGE:
    - builds the reunion of the two lists, with no repeating elements;
    - the first list is iterated, adding all elements that don't belong
      to the second list to the reunion;
    - the second list is iterated, adding all elements that don't already belong
      to the reunion to it

  ARGUMENTS:
    - const Node_int * list1 - the first list;
    - const Node_int * list2 - the second list;

  RETURN VALUE:
    - success - pointer to the reunion list;
    - failure (mostly because of malloc()) - null pointer;

  SIDE EFFECTS:
    - the initial order of the list is lost;
*/
Node_int * reuniteLists(const Node_int * list1, const Node_int * list2)
{
  int current_element;
  const Node_int * iter = NULL;
  Node_int * reunited_list = NULL;

  DEB("Reuniting the two lists...\n");

  DEB("Iterating the first list...\n");
  iter = list1;
  while (iter)
  {
    current_element = iter->info;
    DEB("\t - currently treating element %d;\n", current_element);

    if (!belongsToList(reunited_list, current_element) && !belongsToList(list2, current_element))
    {
      if ((reunited_list = OAF(int, reunited_list, &current_element)) == NULL)
      {
        DEB("Failed to add element from the first list to the reunited list.\n");
        return NULL;
      }
    }
    iter = iter->next;
  }

  iter = list2;
  DEB("Iterating the second list...\n");
  while (iter)
  {
    current_element = iter->info;
    DEB("\t - currently treating element %d;\n", current_element);

    if (!belongsToList(reunited_list, current_element))
    {
      if ((reunited_list = OAF(int, reunited_list, &current_element)) == NULL)
      {
        DEB("Failed to add element from the second list to the reunited list.\n");
        return NULL;
      }
    }
    iter = iter->next;
  }

  return reunited_list;
}

// Driver program for the reunion function
int main()
{
  // Necessary variables for the two lists
  unsigned list1_size = 0;
  unsigned list2_size = 0;
  Node_int * list1 = NULL;
  Node_int * list2 = NULL;
  Node_int * reunited_list = NULL;

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
    fprintf(stderr, "Input error when reading the second list's size.\n");
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
  printList_int(list2);  // < Printing the list to check if all went good

  putchar('\n');

  // Running the tested function
  // Reuniting the lists..
  if ((reunited_list = reuniteLists(list1, list2)) == NULL)
  {
    // Treating (more or less, less than more) function errors
    fprintf(stderr, "Failed to reunite lists.\n");
  }
  else
  {
    // Presumably, the function worked
    // Printing the returned value of the function, to check if it really worked
    printf("The elements of the reunited list are:\n");
    printList_int(reunited_list);
  }

  putchar('\n');

  // Free the memory and end execution
  freeList_int(list1);
  freeList_int(list2);
  freeList_int(reunited_list);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
