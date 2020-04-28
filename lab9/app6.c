/**
  Aplicația 9.6: Să se scrie o funcție care primește ca parametri o listă
                 (posibil vidă) de elemente sortate și un element.
                 Funcția va insera în listă noul element, astfel încât lista să
                 rămână sortată. Folosind aceasta funcție, să se scrie o funcție
                 de sortare a unei liste, care primește ca parametru o listă
                 nesortată și returnează una sortată. Programul nu va folosi
                 niciun vector.
*/

#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

// Call the list handler creation macro for the int type
// Check listHandler.h for details
create_list_handler(int)

// Function to insert a value inside a sorted list
/**
  USAGE:
    - insert a value inside a sorted list, while keeping it sorted;
    - the function iterates the list up until the point where the element must
      be inserted;
    - then, it creates a new node and connects it there;
    - in case the given input list is empty, a singleton with the element to be
      added is created;

  ARGUMENTS:
    - Node_int * sorted_list - the sorted input list;
    - const int * element - the value of the element to be inserted

  RETURN VALUE:
    - pointer to the list - success;
    - NULL - failure when creating the new node;

  SIDE EFFECTS:
    - no outside values will be modified by this function;

  WARNING:
    - please only use this function in programs that add nodes to a list using
      the add first method, or else the tail of the list will not be properly set
    - if you don't know what this is, you should check the first lines of
      listHandler.h
*/
Node_int * insertInSortedList(Node_int * sorted_list, const int * element)
{
  Node_int * current = sorted_list;
  Node_int * previous = sorted_list;
  Node_int * aux = NULL;

  DEB("Inserting the element inside the sorted list, while keeping it sorted.\n");
  if (sorted_list == NULL)
  {
    DEB("\t - the given sorted list is empty;\n");
    DEB("\t - returning a singleton with our element as sole value;\n");
    return OAF(int, sorted_list, element);
  }
  DEB("\t - finding the position for the element to be inserted;\n");

  // In the while condition underneath, it is mandatory to have the iterator check
  // first. Else, you might check for the info field of a null pointer => segfault;
  while(current && current->info > *element)
  {
    DEB("\t\t - passed element %d;\n", current->info);
    previous = current;
    current = current->next;
  }
  DEB("\t - the element must be inserted at address %p;\n", current);
  if ((aux = OAF(int, current, element)) == NULL)
  {
    DEB("\t - adding the element failed due to an exception in memory allocation.\n");
    freeList_int(sorted_list);
    return NULL;
  }
  if (current == sorted_list)
  {
    DEB("\t - the element will be inserted at the head of the list;\n");
    sorted_list = aux;
  }
  else
  {
    previous->next = aux;
  }
  DEB("Done.\n");

  return sorted_list;
}

// Function to sort a list using the previous function
/**
  USAGE:
    - returns a new list, created with the elements of the previous one,
      but sorted;
    - uses the insertInSortedList() function:
        - it iterates the initial list, and calls this function for each element,
          the result after each value being kept in a buffer list, which will
          also serve as the return value of the sorting function
    - if you want it to release the unsorted list, just add a freeList_int()
      call before the end of the function, passing the initial list as a parameter;

  ARGUMENTS:
    - Node_int * list - the list to be sorted;

  RETURN VALUE:
    - success - a pointer to the new sorted list;
    - failure - null pointer;

  SIDE EFFECTS:
    - no outside values will be modified by this function;
*/
Node_int * sortList(const Node_int * list)
{
  const Node_int * iter = NULL;
  Node_int * sorted_list = NULL;
  Node_int * aux = NULL;

  DEB("Sorting the given list...\n");
  for (iter = list; iter; iter = iter->next)
  {
    if ((aux = insertInSortedList(sorted_list, &iter->info)) == NULL)
    {
      freeList_int(sorted_list);
      return NULL;
    }
    sorted_list = aux;
  }
  DEB("Done.\n");
  return sorted_list;
}

// Driver program for the sorting function
int main()
{
  // Necessary variables
  unsigned list_size = 0;
  Node_int * list = NULL;
  Node_int * sorted_list = NULL;

  // Creating & filling the list
  printf("Enter the desired size of the list: ");
  if (!get_unsigned(&list_size, stdin))
  {
    // Treating input errors
    fprintf(stderr, "Input error when reading the list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list = createList_int(&list_size)) == NULL)
  {
    // Treating input or allocation errors
    fprintf(stderr, "The creation of the list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list); // < Printing the list to check if all went good

  putchar('\n');

  // Running the tested function
  // Sorting the list...
  if ((sorted_list = sortList(list)) == NULL)
  {
    // Treating function errors
    fprintf(stderr, "Failed to sort the initial list.\n");
    freeList_int(list);
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  putchar('\n');
  printf("Printing the elements of the sorted list:\n");
  printList_int(sorted_list);

  putchar('\n');

  // Free the memory and end execution
  freeList_int(list);
  freeList_int(sorted_list);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
