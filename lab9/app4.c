/**
  Aplicația 9.4: Pentru implementarea listei care memorează adresa ultimului
                 element, să se scrie o funcție șterge, care șterge din listă
                 un element dat.
*/

#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"


// Call the list handler creation macro for the int type
// Check listHandler.h for details
create_list_handler(int)

/**
    This program will use a different kind of lists, ones that also store the
  address of their last node. For implementing these, we create a new struct
  based on the stuff we already have inside listHandler.h.
*/
typedef struct
{
  Node_int * head;
  Node_int * tail;
} List_int;

// Function to create a list that also hold the address of its last element
/*
  NOTE: TList = tail + list, I didn't find anything better;

  USAGE:
    - similar to createList_int(), only it works with lists that also hold the
      address of the last element;
    - this function is restricted to integer values and lists;

  ARGUMENTS:
    - const unsigned * list_size - pointer to the size of the desired list;

  RETURN VALUE:
    - success - pointer to the list structure;
    - failure - null-pointer;

  SIDE EFFECTS:
    - no outside values will be modified by this function;

  WARNING:
    - please only use this function in programs that add nodes to a list using
      the add first method, or else the tail of the list will not be properly set
    - if you don't know what this is, you should check the first lines of
      listHandler.h
*/
List_int * createTList_int(const unsigned * list_size)
{
  int i;
  int buf;
  List_int * list = NULL;

  // Making space for the list struct;
  if ((list = (List_int *)malloc(sizeof(List_int))) == NULL)
  {
    DEB("Failed the allocation of memory for the list struct.\n");
    return NULL;
  }
  MALLOC_COUNTER++;

  // Reading info for the elements of the list, one by one;
  printf("Enter the elements of the list:\n");
  for (i = 0; i < *list_size; i++)
  {
    printf("\t - element %d: ", i);
    // For information on get_int, see myStdlib.h;
    if (!get_int(&buf, stdin))
    {
      // Treating I/O errors
      DEB("\n\t - failed to read element %d of the list;", i);
      return NULL;
    }
    if ((list->head = OAF(int, list->head, &buf)) == NULL)
    {
      // Treating node allocation errors
      DEB("\n\t - failed to store element list[%d] = %d into the list;", i, buf);
      return NULL;
    }

    // The firstly added element will end up being the tail of the list;
    if (i == 0) // < This condition is why I have made a warning in the function
                // description
    {
      list->tail = list->head;
    }
  }
  return list;
}

// Function to delete all the occurences of an element from a list
/*
  USAGE:
    - deletes all occurences of an element from a TList;
    - iterating using two pointers, for the currently and previously visited nodes,
      the nodes containing the element to be deleted are freed from memory, and
      links to/from them rebuilt;

  ARGUMENTS:
    - List_int * list - the list to be removed from;
    - const int * del_element - the element to be removed;

  RETURN VALUE:
    - success - pointer to the TList structure;
              - may be null - that just means the list is empty;
    - failure - undefined;

  SIDE EFFECTS:
    - no outside values will be modified by this function;
*/
List_int * deleteOccurences(List_int * list, const int * del_element)
{
  Node_int * current = list->head;
  Node_int * previous = list->head;

  // This auxiliary pointer is very important for the program to not segfault;
  // Without it, while iterating, we could have run into a null->next situation
  Node_int * aux = NULL;

  DEB("Deleting all occurences of %d...\n", *del_element);
  while(current)
  {
    DEB("Currently checking element %d:\n", current->info);
    if (current->info == *del_element)
    {
      DEB("\t - this element is to be deleted;\n");
      aux = current->next;
      if (current == list->head)
      {
        DEB("\t - this element is the head of the list;\n");
        free(current);
        MALLOC_COUNTER--;
        list->head = aux;
        current = list->head;
        previous = list->head;
      }
      else
      {
        previous->next = aux;
        free(current);
        MALLOC_COUNTER--;
        current = aux;
      }
    }
    else
    {
      DEB("\t - this element is not to be deleted;\n");
      previous = current;
      current = current->next;
    }
  }
  return list;
}

// Driver program for the aforementioned function
int main()
{
  // Necessary variables
  List_int * list = NULL;
  unsigned list_size;
  int del_element;

  // Creating & filling the list
  printf("Enter the desired size of the list: ");
  if (!get_unsigned(&list_size, stdin))
  {
    // Treating input errors;
    fprintf(stderr, "Input error when reading the list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list = createTList_int(&list_size)) == NULL)
  {
    // Treating input or allocation errors;
    fprintf(stderr, "The creation of the list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list->head); // < Printing the list to check if all went good;

  putchar('\n');

  // Getting the value to be removed from the list
  printf("Enter the element to be deleted from the list:\n");
  if (!get_int(&del_element, stdin)) // < For information on get_int, see myStdlib.h;
  {
    // Treating input errors;
    fprintf(stderr, "Input error when reading the element to be deleted.\n");
    freeList_int(list->head);
    free(list);
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  // Running the tested function
  // Deleting element del_element from the list...
  if ((list = deleteOccurences(list, &del_element))->head == NULL)
  {
    printf("After deleting, the list became empty.\n");
  }
  else
  {
    putchar('\n');
    printList_int(list->head);  // < Printing the list to check if all went good;
  }
  
  putchar('\n');

  // Free the memory and end execution
  freeList_int(list->head);
  free(list);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
