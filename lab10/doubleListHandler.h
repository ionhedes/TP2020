/////////////////////////////////////////////////
/// doubleListHanlder - copy of the listad.h library, defined in the lab, with
///                     slight modifications;
///                   - gives an abstract definition of methods to use with
///                     doubly linked lists;
///                   - the code is not mine, so I will only document the
///                     differences between the lab version and this one;
///                   - I have tried adapting my style of using macros and coding
///                     style to the one of file, aside from the slide changes
///                     incurred by the alterations of this file from the base
///                     version;
/////////////////////////////////////////////////

/*
  IN-LAB NAMES:
    - DLIST_NAME - LISTAD_NUME
    - DLIST_ATOM - LISTAD_ELEMENT - I thought that in our case, a word being the
                                    atom of the sentence, this name better suited
                                    the macro name
*/
#if !defined(DLIST_NAME) || !defined(DLIST_ATOM)
  #error "Warning, you didn't provide the necessary data to implement this abstract data type"
#endif

typedef struct
{
  DLIST_ATOM * head;
  DLIST_ATOM * tail;
}
DLIST_NAME;

#ifndef DLIST_FUNC
  #define DLIST_FUNC_AUX(name, operation) name##_##operation
  #define DLIST_FUNC(name, operation) DLIST_FUNC_AUX(name, operation)
#endif

// Macro to make the memory freeing of atom fields freer
/**
  - the abstract data type doesn't hold information on the atom fields which are
    dynamically assigned, making the freeing operation defined in this file in-
    complete;
  - I have created this, so using a macro defined before this whole header was
    included we can make sure all the memory occupied by the structure held by
    the abstract data type can be freed at once;
  - If you have a dinamically assigned field in the atom data struct, define
    the ADDITIONAL_ATOM_FIELD with its name,
      e.g.: #define ADDITIONAL_ATOM_FIELD content for the Word struct in app1.c
*/
#ifdef ADDITIONAL_ATOM_FIELD
  #define FREE_ADD_FIELD(struct, field) free(struct->field); MALLOC_COUNTER--;
#else
  #define FREE_ADD_FIELD(struct, field)
#endif

void DLIST_FUNC(DLIST_NAME, init)(DLIST_NAME * list)
{
  list->head = NULL;
  list->tail = NULL;
}

void DLIST_FUNC(DLIST_NAME, add)(DLIST_NAME * list, DLIST_ATOM * element)
{
  element->previous = list->tail;
  if (list->tail)
  {
    list->tail->next = element;
  }
  else
  {
    list->head = element;
  }
  list->tail = element;
  element->next = NULL;
}

// Abstract method to insert an atom inside a list, at a given position
/**
  USAGE:
    - by passing it a doubly linked list, an atom to be inserted and a reference
      atom inside the list, this method inserts the given atom before the
      reference one in the list;
    - if the reference atom is NULL, the given atom is inserted at the end of the
      list;

  ARGUMENTS:
    - DLIST_NAME * list - the list;
    - DLIST_ATOM * insert_element - the element to be inserted;
    - DLIST_ATOM * succeeding_element - the reference element;

  RETURN VALUE:
    - this method does not have any return value;

  SIDE EFFECTS:
    - this method doesn't modify any outside variables;

  NOTE:
    - this function is also the answer to app3 from the lab;
*/
void DLIST_FUNC(DLIST_NAME, insert)(DLIST_NAME * list, DLIST_ATOM * insert_element, DLIST_ATOM * succeeding_element)
{
  if (succeeding_element)
  {
    if (succeeding_element->previous)
    {
      succeeding_element->previous->next = insert_element;
      insert_element->previous = succeeding_element->previous;
    }
    else
    {
      list->head = insert_element;
    }

    succeeding_element->previous = insert_element;
    insert_element->next = succeeding_element;
  }
  else
  {
    DLIST_FUNC(DLIST_NAME, add)(list, insert_element);
  }
}

void DLIST_FUNC(DLIST_NAME, delete)(DLIST_NAME * list, DLIST_ATOM * element)
{
  if (element->previous)
  {
    element->previous->next = element->next;
  }
  else
  {
    list->head = element->next;
  }

  if (element->next)
  {
    element->next->previous = element->previous;
  }
  else
  {
    list->tail = element->previous;
  }
  if (!element->previous && !element->next)
  {
    DLIST_FUNC(DLIST_NAME, init)(list);
  }
  FREE_ADD_FIELD(element, ADDITIONAL_ATOM_FIELD);
  free(element);
  MALLOC_COUNTER--;
}

void DLIST_FUNC(DLIST_NAME, freeMem)(DLIST_NAME * list)
{
  DLIST_ATOM * current = NULL;
  DLIST_ATOM * next = NULL;

  for (current = list->head; current; current = next)
  {
    next = current->next;
    FREE_ADD_FIELD(current, ADDITIONAL_ATOM_FIELD);
    free(current);
    MALLOC_COUNTER--;
  }

  DLIST_FUNC(DLIST_NAME, init)(list);
}

#undef DLIST_NAME
#undef DLIST_ATOM
