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

List_int * joinLists(const List_int * list1, const List_int * list2)
{
  List_int * joint_list = NULL;
  Node_int * iter1 = list1->head;
  Node_int * iter2 = list2->head;
  Node_int * tail = NULL;

  if ((joint_list = (List_int *)malloc(sizeof(List_int))) == NULL)
  {
    DEB("Failed to allocate memory for the joint list.\n");
    return NULL;
  }

  DEB("Joining the two lists...\n");
  while (iter1 && iter2)
  {
    DEB("\t - current elements:\n");
    DEB("\t\t - list1 : %d;\n", iter1->info);
    DEB("\t\t - list2 : %d;\n", iter2->info);

    if (iter1->info <= iter2->info)
    {
      DEB("\t\t - %d will be added to the list;\n", iter1->info);
      if ((joint_list->head = OAF (int, joint_list->head, &iter1->info)) == NULL)
      {
        DEB("Failed to add an element at the end of the sorted list.\n");
        free(joint_list);
        MALLOC_COUNTER--;
        return NULL;
      }
      iter1 = iter1->next;
    }
    else
    {
      DEB("\t\t - %d will be added to the list;\n", iter2->info);
      if ((joint_list->head = OAF(int, joint_list->head, &iter2->info)) == NULL)
      {
        DEB("Failed to add an element at the end of the sorted list.\n");
        free(joint_list);
        MALLOC_COUNTER--;
        return NULL;
      }
      iter2 = iter2->next;
    }
  }
  DEB("One list was consumed, now adding the remaining elements of the other list.\n");

  if (iter1 && !iter2)
  {
    DEB("There are still elements in the first list.\n");
    while (iter1)
    {
      DEB("\t - current element:\n");
      DEB("\t\t - list1 : %d;\n", iter1->info);
      DEB("\t\t - %d will be added to the list;\n", iter1->info);


      if ((joint_list->head = OAF(int, joint_list->head, &iter1->info)) == NULL)
      {
        DEB("Failed to add an element at the end of the sorted list.\n");
        free(joint_list);
        MALLOC_COUNTER--;
        return NULL;
      }
      tail = iter1;
      iter1 = iter1->next;
    }
  }
  if (!iter1 && iter2)
  {
    DEB("There are still elements in the second list.\n");
    while (iter2)
    {
      DEB("\t - current element:\n");
      DEB("\t\t - list2 : %d;\n", iter2->info);
      DEB("\t\t - %d will be added to the list;\n", iter2->info);

      if ((joint_list->head = OAF(int, joint_list->head, &iter2->info)) == NULL)
      {
        DEB("Failed to add an element at the end of the sorted list.\n");
        free(joint_list);
        MALLOC_COUNTER--;
        return NULL;
      }
      tail = iter2;
      iter2 = iter2->next;
    }
  }
  joint_list->tail = tail;
  DEB("Done!\n");

  return joint_list;
}

int main()
{
  List_int * list1 = NULL;
  List_int * list2 = NULL;
  List_int * joint_list = NULL;
  unsigned list1_size;
  unsigned list2_size;

  printf("Enter the desired size of the first list: ");
  if (!get_unsigned(&list1_size, stdin))
  {
    fprintf(stderr, "Input error when reading the first list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list1 = createSList_int(&list1_size)) == NULL)
  {
    fprintf(stderr, "The creation of the first list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list1->head);

  printf("Enter the desired size of the second list: ");
  if (!get_unsigned(&list2_size, stdin))
  {
    fprintf(stderr, "Input error when reading the second list's size.\nExiting...\n");
    freeList_int(list1->head);
    free(list1);
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list2 = createSList_int(&list2_size)) == NULL)
  {
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
    freeList_int(list2->head);
    free(list1);
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list2->head);

  putchar('\n');
  if ((joint_list = joinLists(list1, list2)) == NULL)
  {
    fprintf(stderr, "Failed to join lists.\n");
  }
  else
  {
    putchar('\n');
    printf("Joint sorted list:\n");
    printList_int(joint_list->head);
  }

  putchar('\n');
  freeList_int(list1->head);
  free(list1);
  freeList_int(list2->head);
  free(list2);
  MALLOC_COUNTER -= 2;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
