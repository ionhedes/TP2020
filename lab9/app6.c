#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

create_list_handler(int)

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
    return addFirst_int(sorted_list, element);
  }
  DEB("\t - finding the position for the element to be inserted;\n");
  while(current && current->info > *element) // mandatory for current to be first inside the condition // segfault otherwise
  {
    DEB("\t\t - passed element %d;\n", current->info);
    previous = current;
    current = current->next;
  }
  DEB("\t - the element must be inserted at address %p;\n", current);
  if ((aux = addFirst_int(current, element)) == NULL)
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

Node_int * sortList(Node_int * list)
{
  Node_int * iter = NULL;
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

int main()
{
  unsigned list_size = 0;
  Node_int * list = NULL;
  Node_int * sorted_list = NULL;

  printf("Enter the desired size of the list: ");
  if (!get_unsigned(&list_size, stdin))
  {
    fprintf(stderr, "Input error when reading the list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list = createList_int(&list_size)) == NULL)
  {
    fprintf(stderr, "The creation of the list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list);

  putchar('\n');
  if ((sorted_list = sortList(list)) == NULL)
  {
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

  freeList_int(list);
  freeList_int(sorted_list);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
