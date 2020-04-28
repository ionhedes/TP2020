#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

create_list_handler(int)

Node_int * deleteOccurences(Node_int * list, const int element)
{
  Node_int * current = list;
  Node_int * previous = list;

  while (current)
  {
    if (current->info == element)
    {
      if (current == list)
      {
        list = current->next;
        free(current);
        MALLOC_COUNTER--;
        current = list;
        previous = list;
      }
      else
      {
        previous->next = current->next;
        free(current);
        MALLOC_COUNTER--;
        current = previous->next;
      }
    }
    else
    {
      previous = current;
      current = current->next;
    }
  }
  return list;
}

int belongsToList(Node_int * list, const int element)
{
  Node_int * iter = NULL;
  for (iter = list; iter; iter = iter->next)
  {
    if (iter->info == element)
    {
      return 1;
    }
  }
  return 0;
}

Node_int * reuniteLists(Node_int * list1, Node_int * list2)
{
  int current_element;
  Node_int * iter = NULL;
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

int main()
{
  unsigned list1_size = 0;
  unsigned list2_size = 0;
  Node_int * list1 = NULL;
  Node_int * list2 = NULL;
  Node_int * reunited_list = NULL;

  printf("Enter the desired size of the first list: ");
  if (!get_unsigned(&list1_size, stdin))
  {
    fprintf(stderr, "Input error when reading the first list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list1 = createList_int(&list1_size)) == NULL)
  {
    fprintf(stderr, "The creation of the first list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list1);

  putchar('\n');

  printf("Enter the desired size of the second list: ");
  if (!get_unsigned(&list2_size, stdin))
  {
    fprintf(stderr, "Input error when reading the second list's size.\n");
    freeList_int(list1);
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list2 = createList_int(&list2_size)) == NULL)
  {
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
    freeList_int(list1);
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list2);

  putchar('\n');
  if ((reunited_list = reuniteLists(list1, list2)) == NULL)
  {
    fprintf(stderr, "Failed to reunite lists.\n");
  }
  else
  {
    printf("The elements of the reunited list are:\n");
    printList_int(reunited_list);
  }

  putchar('\n');

  freeList_int(list1);
  freeList_int(list2);
  freeList_int(reunited_list);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
