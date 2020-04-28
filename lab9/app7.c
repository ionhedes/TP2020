/**
  Aplicația 9.7: Pentru implementarea listei care memorează ultimul element,
                 să se scrie o funcție care primește două liste sortate și
                 returnează lista sortată care conține toate elementele lor.
                 Pentru lista rezultată se va folosi doar operația de adăugare
                 la sfârșit de listă.

  Exemplu: {1, 2, 7, 8} și {2, 9} -> {1, 2, 2, 7, 8, 9}
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

// Function to join two sorted list into a resulting sorted list
/**
  USAGE:
    - interclassing algorithm implemented for usage on integer simple linked
      lists;
    - this function creates a new TList from the initial lists while keeping the
      elements ordered;

  HOW IT WORKS:
      - both of the lists are iterated at the same time, until one of them ends;
      - for each step, the list with the currently smaller element gets iterated,
        and the element stored in the resulting list;
      - the rest of the remaining string is 'copied' in the resulting list;
      - this has a linear time complexity;

  ARGUMENTS:
    - const List_int * list1 - the first list;
    - const List_int * list2 - the second list;

  RETURN VALUE:
    - success - pointer to the joint list;
    - failure - null pointer;

  SIDE EFFECTS:
    - no outside values will be modified by this function;

  WARNING:
    - please only use this function in programs that add nodes to a list using
      the add last method, or else the tail of the list will not be properly set
    - if you don't know what this is, you should check the first lines of
      listHandler.h
*/
List_int * joinLists(const List_int * list1, const List_int * list2)
{
  List_int * joint_list = NULL;
  Node_int * iter1 = list1->head;
  Node_int * iter2 = list2->head;
  Node_int * tail = NULL; // < Variable used to track the tail of the list

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

// Driver program for the list-joining function
int main()
{
  // Necessary variables for the two lists
  List_int * list1 = NULL;
  List_int * list2 = NULL;
  List_int * joint_list = NULL;
  unsigned list1_size;
  unsigned list2_size;

  // Creating & filling the first list
  printf("Enter the desired size of the first list: ");
  if (!get_unsigned(&list1_size, stdin))
  {
    // Treating input errors
    fprintf(stderr, "Input error when reading the first list's size.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list1 = createTList_int(&list1_size)) == NULL)
  {
    // Treating input or allocation errors
    fprintf(stderr, "The creation of the first list failed.\nExiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list1->head); // < Printing the list to check if all went good

  // Creating & filling the second list
  printf("Enter the desired size of the second list: ");
  if (!get_unsigned(&list2_size, stdin))
  {
    // Treating input errors
    fprintf(stderr, "Input error when reading the second list's size.\nExiting...\n");
    freeList_int(list1->head);
    free(list1); // < Releasing the first list before aborting exe.
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((list2 = createTList_int(&list2_size)) == NULL)
  {
    // Treating input or allocation errors;
    fprintf(stderr, "The creation of the second list failed.\nExiting...\n");
    freeList_int(list2->head);
    free(list1); // < Releasing the first list before aborting exe.
    MALLOC_COUNTER--;
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  putchar('\n');
  printList_int(list2->head); // < Printing the list to check if all went good

  putchar('\n');

  // Running the tested function
  // Joining the two functions...
  if ((joint_list = joinLists(list1, list2)) == NULL)
  {
    // Treating function errors
    fprintf(stderr, "Failed to join lists.\n");
  }
  else
  {
    putchar('\n');
    printf("Joint sorted list:\n");
    printList_int(joint_list->head);
  }

  putchar('\n');

  // Free the memory and end execution
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
