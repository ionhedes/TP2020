#ifndef MYSTDLIB_ON
  #include "myStdlib.h"
#endif



// Usual data type string placeholders macros, implemented for automating prints
#define PLACEHOLDER_int "%d"
#define PLACEHOLDER_float "%f"
#define PLACEHOLDER_double "%lf"
#define PLACEHOLDER(type) PLACEHOLDER_##type

#define create_list_handler(type)                                              \
  typedef struct Node_##type Node_##type;                                      \
                                                                               \
  struct Node_##type                                                           \
  {                                                                            \
    type info;                                                                 \
    Node_##type * next;                                                        \
  };                                                                           \
                                                                               \
                                                                               \
  void freeList_##type(Node_##type * list)                                     \
  {                                                                            \
    Node_##type * iter = list;                                                 \
    Node_##type * aux_next = NULL;                                             \
                                                                               \
    DEB("Freeing list...\n");                                                  \
    while (iter != NULL)                                                       \
    {                                                                          \
      aux_next = iter->next;                                                   \
      DEB("\t - element "PLACEHOLDER(type)" ", iter->info);                    \
      free(iter);                                                              \
      DEB("deleted.\n");                                                       \
      iter = aux_next;                                                         \
                                                                               \
      MALLOC_COUNTER--;                                                        \
    }                                                                          \
    DEB("List is freed.\n");                                                   \
  }                                                                            \
                                                                               \
  Node_##type * new_##type(const type * info, Node_##type * next)              \
  {                                                                            \
    Node_##type * node = NULL;                                                 \
    if ((node = (Node_##type *)malloc(sizeof(Node_##type))) == NULL)           \
    {                                                                          \
      DEB("Node memory allocation failed.\n");                                 \
      return NULL;                                                             \
    }                                                                          \
    MALLOC_COUNTER++;                                                          \
                                                                               \
    node->info = *info;                                                        \
    node->next = next;                                                         \
                                                                               \
    return node;                                                               \
  }                                                                            \
                                                                               \
  Node_##type * addFirst_##type(Node_##type * list, const type * info)         \
  {                                                                            \
    list = new_##type(info, list);                                             \
    if (list == NULL)                                                          \
    {                                                                          \
      DEB("Adding node to the beginning of the list failed.\n");               \
      freeList_##type(list);                                                   \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    return list;                                                               \
  }                                                                            \
                                                                               \
  Node_##type * addLast_##type(Node_##type * list, const type * info)          \
  {                                                                            \
    Node_##type * iter = list;                                                 \
                                                                               \
    if (list == NULL)                                                          \
    {                                                                          \
      list = new_##type(info, NULL);                                           \
      if (list == NULL)                                                        \
      {                                                                        \
        DEB("Adding node to the end of the list failed.\n");                   \
        return NULL;                                                           \
      }                                                                        \
      return list;                                                             \
    }                                                                          \
                                                                               \
    while (iter->next != NULL)                                                 \
    {                                                                          \
      iter = iter->next;                                                       \
    }                                                                          \
    iter->next = new_##type(info, NULL);                                       \
    if (iter->next == NULL)                                                    \
    {                                                                          \
      DEB("Adding node to the end of the list failed.\n");                     \
      freeList_##type(list);                                                   \
      return NULL;                                                             \
    }                                                                          \
                                                                               \
    return list;                                                               \
  }                                                                            \
                                                                               \
  Node_##type * createList_##type(const unsigned * list_size)                  \
  {                                                                            \
    int i;                                                                     \
    type buf;                                                                  \
    Node_##type * list = NULL;                                                 \
                                                                               \
    printf("Enter the elements of the list:\n");                               \
    for (i = 0; i < *list_size; i++)                                           \
    {                                                                          \
      printf("\t - element %d: ", i);                                          \
      if (!get_##type(&buf, stdin))                                            \
      {                                                                        \
        DEB("\n\t - failed to read element %d of the list;", i);               \
        return NULL;                                                           \
      }                                                                        \
      if ((list = addFirst_##type(list, &buf)) == NULL)                        \
      {                                                                        \
        DEB("\n\t - failed to store element list[%d] = "                       \
             PLACEHOLDER(type)" into the list;", i, buf);                      \
        return NULL;                                                           \
      }                                                                        \
    }                                                                          \
                                                                               \
    return list;                                                               \
  }                                                                            \
                                                                               \
  void printList_##type(Node_##type * list)                                    \
  {                                                                            \
    Node_##type * iter = NULL;                                                 \
    printf("The elements of the list are:\n\t - ");                            \
    for (iter = list; iter; iter = iter->next)                                 \
    {                                                                          \
      printf(""PLACEHOLDER(type)", ", iter->info);                             \
    }                                                                          \
    putchar('\n');                                                             \
  }                                                                            \
                                                                               \
  unsigned getSizeList_##type(const Node_##type * list)                        \
  {                                                                            \
    unsigned size = 0;                                                         \
    const Node_##type * iter = NULL;                                           \
    for (iter = list; iter != NULL; iter = iter->next)                         \
    {                                                                          \
      size++;                                                                  \
    }                                                                          \
    return size;                                                               \
  }
