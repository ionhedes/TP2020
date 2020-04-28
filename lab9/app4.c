#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

create_list_handler(int)

typedef struct
{
  Node_int * head;
  Node_int * tail;
} List_int;

List_int * createSList_int(const unsigned * list_size)
{
  int i;
  int buf;
  List_int * list = NULL;

  if ((list = (List_int *)malloc(sizeof(List_int))) == NULL)
  {
    DEB("Failed the allocation of memory for the list struct.\n");
    return NULL;
  }
  MALLOC_COUNTER++;

  printf("Enter the elements of the list:\n");
  for (i = 0; i < *list_size; i++)
  {
    printf("\t - element %d: ", i);
    if (!get_int(&buf, stdin))
    {
      DEB("\n\t - failed to read element %d of the list;", i);
      return NULL;
    }
    if ((list->head = OAF(int, list->head, &buf)) == NULL)
    {
      DEB("\n\t - failed to store element list[%d] = %d into the list;", i, buf);
      return NULL;
    }
    if (i == 0)
    {
      list->tail = list->head;

    }
  }
  return list;
}

List_int * deleteOccurences(List_int * list, const int * del_element)
{
  Node_int * current = list->head;
  Node_int * previous = list->head;
  Node_int * aux = NULL; // this auxiliary pointer is very important for the program to not segfault

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

int main()
{
  List_int * list = NULL;
  unsigned list_size;
  int del_element;

  printf("Enter the desired size of the list: ");
  if (!get_unsigned(&list_size, stdin))
  {
    fprintf(stderr, "Input error when reading the list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list = createSList_int(&list_size)) == NULL)
  {
    fprintf(stderr, "The creation of the list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list->head);

  putchar('\n');
  printf("Enter the element to be deleted from the list:\n");
  if (!get_int(&del_element, stdin))
  {
    fprintf(stderr, "Input error when reading the element to be deleted.\n");
    freeList_int(list->head);
    free(list);
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list = deleteOccurences(list, &del_element))->head == NULL)
  {
    printf("After deleting, the list became empty.\n");
  }
  else
  {
    putchar('\n');
    printList_int(list->head);
  }
  putchar('\n');

  freeList_int(list->head);
  free(list);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
