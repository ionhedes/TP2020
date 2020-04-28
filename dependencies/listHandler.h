/////////////////////////////////////////////////
/// lsitHandler - library created for easing the work with different data type
///                simple linked lists
///              - covers some basic operations like:
///                - creating list;
///                - adding node to beginning/end of list;
///                - printing list;
///                - computing size of list
/////////////////////////////////////////////////

// myStdlib.h should be included for this library to operate
#ifndef MYSTDLIB_ON
  #include "myStdlib.h"
#endif

// Compiler flag macro to define the way lists are read
/**
  WHAT IS THIS?
    - there are two methods of adding nodes to a list, by adding at the beginning
      and at the end of the list, with different time complexities;
    - there may be times when you want to use one or another, based on the computer
      you are running the program on, or the program itself (what is it
      supposed to do);
    - this compiler flag/preprocessor directive help you pick the way lists are
      treated since the compiling/building phase of the program

  OAF?
    - this abbreviation comes from Ordered Adding Function

  USAGE:
    - by default, lists are being created and nodes are being added using the
      add first method, having a constant time complexity;
    - if, for some reason, you want elements to be added at the end of a list,
      add the following flag when compiling: -DADDLAST;
    - OAF() is a wrapper function for the chosen add function
    - this way, the program will always use the corresponding function for the
      specified adding method.

  WARNING:
    - you must first call create_list_handler(type) or provide a definition
      for a Node data type and adding functions before using the OAF() macro
    - you also need to provide your macro with the data type your list contains
*/
#ifdef ADDLAST
  #define OAF(type, ...) addLast_##type(__VA_ARGS__)
#else
  #define OAF(type, ...) addFirst_##type(__VA_ARGS__)
#endif

// Usual data type string placeholders macros, implemented for automating prints
#define PLACEHOLDER_int "%d"
#define PLACEHOLDER_float "%f"
#define PLACEHOLDER_double "%lf"
#define PLACEHOLDER(type) PLACEHOLDER_##type


// Main macro creating a template for simple linked list handling functions
/**
USAGE:
  - call with the appropiate data type to create the necessary data type and
    functions to work with lists
  - these functions only cover lists that only store their beginning address.
    For lists storing both beginning and ending addresses, you can write your
    own stuff based on this

ARGUMENTS:
  - type - one of the currently supported data types:
         - int
         - unsigned
         - float
         - double

FUNCTION DESCRIPTIONS:
  - freeList_##type: - since all the lists are made up of dinamically allocated
                       nodes, we need a function to clean up and release the
                       memory after we are done with it
                     - pass it the list and it will do its magic

  - new_##type: - function to create a new node and fill it with given data
                - you must pass it values for both the info field and the next
                  node field
                 - returns a pointer to the new node on success, and null-pointer
                  on failure

  - addFirst_##type: - function to add a node at the beginning of a list;
                     - you must pass it the address of the head of the list,
                       and a variable poiting to data to be stored in info;
                     - if you pass it a null-pointer for a list, it will create
                       it;
                     - uses the new_##type() function for the creation of the
                       new node;
                     - returns a pointer to the new head of the list on success,
                       null-pointer on failure;

  - addLast_##type: - function to add a node at the end of a list;
                    - you must pass it the address of the head of the list,
                      and a variable poiting to data to be stored in info;
                    - if you pass it a null-pointer for a list, it will create
                      it;
                    - if the given list pointer is not null, the function
                      iterates the function up until the last node, creates a new
                      one and links the list with the new node
                    - uses the new_##type() function for the creation of the
                      new node;
                    - returns a pointer to the new head of the list on success,
                      null-pointer on failure;

  - createList_##type: - function to create a list and return it, 0->100;
                       - makes use of the preceding methods to create the list;
                       - you need to pass it the desired size of the list;
                       - all the information stored in the nodes of the future
                         list is read by this function with calls to get_##type()
                         inside myStdlib.h;
                       - most robust way to create a list, always when possible
                         just use this;
                       - returns a pointer to the list on success, null-pointer
                         on failure;

  - printList_##type: - function to print the elements of a list in a human-
                        friendly format;
                      - you need pass it a list pointer

  - getSizeList_##type: - function to return the size of a list;
                        - you need to pass it a list pointer;
                        - it will return the size of the list;

  WARNING: all these functions deal with memory allocation, so it is good practice
           to check their returned values
*/

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
      if ((list = OAF(type, list, &buf)) == NULL)                              \
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
