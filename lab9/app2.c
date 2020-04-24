#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

create_list_handler(int)

Node_int * invertList(Node_int * list)
{
  Node_int * current = list;
  Node_int * previous = NULL;
  Node_int * next = NULL;

  DEB("Reversing list...\n");
  while (current != NULL)
  {
    next = current->next; // necessary or else null->next will give segfault()
    current->next = previous;
    previous = current;
    current = next;
  }
  DEB("List theoretically reversed.\n");

  return previous;
}

int main()
{
  unsigned list_size = 0;
  Node_int * list = NULL;

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

  list = invertList(list);
  putchar('\n');
  printList_int(list);

  putchar('\n');

  freeList_int(list);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
