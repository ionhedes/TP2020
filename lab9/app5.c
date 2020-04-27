#include <stdio.h>
#include "myStdlib.h"
#include "listHandler.h"

create_list_handler(int)

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

int main()
{
  unsigned list1_size = 0;
  unsigned list2_size = 0;
  Node_int * list1 = NULL;
  Node_int * list2 = NULL;

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
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
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

  freeList_int(list1);
  freeList_int(list2);
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
