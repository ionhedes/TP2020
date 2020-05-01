/**
  Aplicația 10.1: Să se modifice exemplul 1 astfel încât el să numere de câte
                  ori apare fiecare cuvânt în propoziție. Pentru aceasta
                  cuvintele vor fi adăugate doar cu litere mici și fiecare
                  cuvânt va avea asociat un contor. Dacă un cuvânt nou nu există
                  în propoziție, el va fi adăugat. Altfel, dacă el există deja,
                  doar se va incrementa contorul cuvântului existent.
                  La afișare, pentru fiecare cuvânt se va afișa și contorul său.
*/

#include <stdio.h>
#include "myStdlib.h"

// Provides a definition for the word data type;
/*
  FIELDS:
    - int occurence_counter - the number of occurences of the word inside a sentence;
    - char * content - address to the content of the word, the word itself;
    - Word * previous - link to the previous word (for double lists - sentence);
    - Word * next - link to the next word (for double lists - sentence);
*/
typedef struct Word Word;

struct Word
{
  int occurence_counter;
  char * content;
  Word * previous;
  Word * next;
};

// Macros defined for implemeting the sentence data type
/**
  USAGE:
    - these macros are used by the abstract data type defined in
      doubleListHandler.h;
    - they implement the Sentence data type based on doubly linked lists;
*/
#define DLIST_NAME Sentence
#define DLIST_ATOM Word
#define ADDITIONAL_ATOM_FIELD content
#include "doubleListHandler.h"

// Function to create a new Word node;
/**
  USAGE:
    - pass it the word in string form, and it will encapsulate it in a dinamically
      assigned Word struct;
    - the previous and next links are set to null, and the occurence
      counter of the word is set to 0 by default, since the word doesn't belong
      to a sentence yet
    - the dinamically assigned char * content will be used inside the Word struct,
      so it shouldn't be released until the whole struct is released;

  ARGUMENTS:
    - char * content - the word in string form, dinamically assigned;

  RETURN VALUE:
    - on success - pointer to the Word struct;
    - on failure - NULL pointer;

  SIDE EFFECTS:
    - there are no outside variables modified by the use of this function;
*/
Word * Word_create(char * content)
{
  Word * word = NULL;

  DEB("Allocating necessary memory for the word structure and its content.\n");
  if ((word = (Word *)malloc(sizeof(Word))) == NULL)
  {
    DEB("Failed to allocate new word.\n");
    return NULL;
  }
  MALLOC_COUNTER++;
  word->occurence_counter = 0;
  word->content = content;
  DEB("Checking if the value of the word is correctly set:\n");
  DEB("\t - %s = %s;\n", content, word->content);

  DEB("Initializing the links of the word with null-pointers.\n");
  word->previous = NULL;
  word->next = NULL;

  DEB("Done.\n\n");
  return word;
}

// Function to return the node with the first occurence of a word in a sentence;
/**
  USAGE:
    - find if a word belongs to a sentence;
    - additionally, if it belongs, find its position inside it;
    - iterates the Sentence, and compares the value of char * element to the
      values of the words, until it finds the first occurence, or until it reaches
      the end of the sentence;

  ARGUMNETS:
    - Sentence * sentence - the sentence list;
    - char * element - the word to be found, in string form;

  RETURN VALUE:
    - the word was found - the address of the Word node;
    - the word was not found - NULL pointer

  SIDE EFFECTS:
    - there are no outside variables modified by the use of this function;
*/
Word * Sentence_seek(Sentence * sentence, char * element)
{
  Word * iter = NULL;

  DEB("Iterating list, looking for element \"%s\"...\n", element);
  for (iter = sentence->head; iter; iter = iter->next)
  {
    if (!strcmp(iter->content, element))
    {
      DEB("Word with this content found at address %p, value %s;\n", iter, iter->content);
      return iter;
    }
  }
  DEB("The word was not found inside the sentence.\n");
  DEB("Done.\n\n");
  return NULL;
}

// Function to create a sentence from scratch;
/**
  - as stated in the text of the problem, reading words stops when the input
    string reads ".".
  - if a word is already part of a sentence, it is not added, but the occurence
    counter of the word object from the sentence is incremented;

  USAGE:
    - use it to create a new sentence, with no prerequisites;
    - all the data concerning the new sentence will be defined inside this function;
    - this method makes use of all the previously defined methods, here and in
      doubleListHandler.h;
    - string input is handled by getString() -> see myStdlib.h for details;

  ARGUMENTS:
    - none, everything needed will be requested once the function is in execution;

  RETURN VALUE:
    - on success - a sentence pointer;
    - on failure - NULL pointer;

  SIDE EFFECTS:
    - multiple memory blocks will be created using malloc(). When testing, they
      were all freed by this function, if they weren't part of the sentence, or
      when freeing the sentence later, so there was no memory block uncovered;
    - if, for some reason, when in debug mode, at the end of execution or when
      a forced exit occurs, MALLOC_COUNTER reads anything else than zero and you
      think it may be because of this function, please create an issue on github
      with the debug and error data;
*/
Sentence * Sentence_create()
{
  Sentence * sentence = NULL;
  Word * buffer_word = NULL;
  char * buffer_string = NULL;

  DEB("Allocating necessary memory for the sentence.\n");
  if ((sentence = (Sentence *)malloc(sizeof(Sentence))) == NULL)
  {
    DEB("Failed to allocate the new sentence.\n");
    return NULL;
  }
  MALLOC_COUNTER++;

  DEB("Initializing the pointers to head/tail of the sentence with null-pointers.\n");
  Sentence_init(sentence);

  // Loop used for reading all the words. Keep in mind reading ends at ".";
  do
  {
    printf("Enter a word: ");
    buffer_string = getString(stdin);
    if (buffer_string == NULL)
    {
      DEB("Failed to read a word from keyboard input.\n");
      if (sentence)
      {
        Sentence_freeMem(sentence);
        free(sentence);
        MALLOC_COUNTER--;
      }
      return NULL;
    }

    // Finding out if the word is already part of the sentence;
    buffer_word = Sentence_seek(sentence, buffer_string);

    // Case: word was not found, adding it;
    if (buffer_word == NULL)
    {
      DEB("The word was not found inside the sentence, inserting it...;\n");
      buffer_word = Word_create(buffer_string);
      if (buffer_word == NULL)
      {
        if (sentence)
        {
          Sentence_freeMem(sentence);
          free(sentence);
          MALLOC_COUNTER--;
        }
        free(buffer_string);
        MALLOC_COUNTER--;
        return NULL;
      }
      Sentence_add(sentence, buffer_word);
    }
    // Case: word was not found, only incrementing word counter;
    else
    {
      DEB("The word was found inside the sentence, increasing its occurence_counter...\n\n");
      free(buffer_string); // < In this case, the buffer must be freed, since it
                           //   won't be used in a word, so it has no role
      MALLOC_COUNTER--;
    }
    buffer_word->occurence_counter++;
  }
  while (buffer_string && *buffer_word->content != '.');
  /**
    Interpreting the stop condition of the do{...}while loop:
      - it must stop when a getString() error occurs and buffer_string becomes
        NULL
      - it must stop when the buffer word becomes a point
  */

  DEB("Done.\n\n");
  return sentence;
}

// Function to delete a word from a sentence;
/**
  USAGE:
    - removes a word from the sentence, frees the memory used by it and
      rebuilds the links between the other words in the sentence;
    - this makes use of the Sentence_delete() method;

  ARGUMENTS:
    - Sentence * sentence - the sentence;
    - anything else that is needed will be defined inside the function;

  RETURN VALUE:
    - on success - pointer to the sentence;
    - on failure - NULL pointer;
    - note: if this function fails, the sentence will be freed by the function
      before the callback;

  SIDE EFFECTS:
    - no outside variables are modified by this function;
*/
Sentence * Word_delete(Sentence * sentence)
{
  char * del_string = NULL;
  Word * del_word = NULL;

  printf("Enter the word to be deleted: ");
  if ((del_string = getString(stdin)) == NULL)
  {
    fprintf(stderr, "Failed to read word to be deleted.\n");
    if (sentence)
    {
      Sentence_freeMem(sentence);
      free(sentence);
      MALLOC_COUNTER--;
    }
    return NULL;
    printf("Execution ended.\n");
    DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }

  if ((del_word = Sentence_seek(sentence, del_string)) == NULL)
  {
    DEB("The word %s was not found inside the sentence.\n", del_string);
  }
  else
  {
    DEB("Deleting element %s from the sentence...\n", del_string);
    Sentence_delete(sentence, del_word);
  }

  free(del_string);
  MALLOC_COUNTER--;

  return sentence;
}

// Function to print a sentence;
/**
  USAGE:
    - prints the words in a sentence and the number of times they appear in it;

  ARGUMENTS:
    - Sentence * sentence - the sentence;

  RETURN VALUE:
    - this method does not have any return value;

  SIDE EFFECTS:
    - this method doesn't modify any outside variables;
*/
void Sentence_print(Sentence * sentence)
{
  Word * iter = NULL;
  printf("The words in the sentence are:\n");
  for (iter = sentence->head; iter; iter = iter->next)
  {
    printf("\t - %s, appearing %d times;\n", iter->content, iter->occurence_counter);
  }
  putchar('\n');
}

// Function to generate the menu requested by the problem text;
void generateMenu()
{
  printf("----MENU----\n");
  printf("what would you like to do?\n");
  printf("\t 1 - new sentence;\n");
  printf("\t 2 - print sentece;\n");
  printf("\t 3 - delete word;\n");
  printf("\t 4 - exit;\n");\
  putchar('\n');
  printf("What would you like to do: ");
}

// Driver function for the sentence manipulation
int main()
{
  // Necessary variables;
  unsigned go_condition = 1; // < Keep on executing program switch, default on;
  unsigned option; // < Operation selector -> see menu for details;
  Sentence * sentence = NULL; // < Pointer for the sentence;

  while (go_condition)
  {
    // Print the menu and ask the user for its option;
    generateMenu();
    if (!get_unsigned(&option, stdin))
    {
      // Treat input errors;
      fprintf(stderr, "Failed to read option value.\n");
      if (sentence)
      {
        Sentence_freeMem(sentence);
        free(sentence);
        MALLOC_COUNTER--;
      }
      printf("Execution ended.\n");
      DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
      printf("Exiting...\n");
      exit(EXIT_FAILURE);
    }

    // Branch off on the option on the user;
    // Always check for the definition state of the sentence pointer before calling methods;
    switch (option)
    {
      // Case: create a new sentence;
      case 1:
      {
        if (sentence)
        {
          printf("Freeing previosly stored sentence...\n");
          Sentence_freeMem(sentence);
          free(sentence);
          MALLOC_COUNTER--;
        }
        printf("Creating new sentence...\n");
        sentence = Sentence_create();
        break;
      }

      // Case: print the current stored sentence;
      case 2:
      {
        if (sentence)
        {
          printf("Printing sentence...\n");
          Sentence_print(sentence);
        }
        else
        {
          printf("The sentence is empty.\n");
        }
        putchar('\n');
        break;
      }

      // Case: delete a word from the sentence;
      case 3:
      {
        if (sentence)
        {
          printf("Deleting a word from the sentence...\n");
          if ((sentence = Word_delete(sentence)) == NULL)
          {
            // Treat function errors;
            fprintf(stderr, "Error when reading word to be deleted.\n");
            printf("Execution ended.\n");
            DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
            printf("Exiting...\n");
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          printf("The sentence is empty.\n");
        }
        putchar('\n');
        break;
      }

      // Case: exit the program;
      case 4:
      {
        go_condition = 0;
        break;
      }

      // Default case: reached in case of severe error, will be treated with
      // immediate exit;
      default:
      {
        fprintf(stderr, "Default case reached. Error. Freeing memory and aborting.\n");
        if (sentence)
        {
          Sentence_freeMem(sentence);
          free(sentence);
          MALLOC_COUNTER--;
        }
        printf("Execution ended.\n");
        DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
        printf("Exiting...\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  // Free memory and end execution;
  // Freeing the sentence, if it exists;
  if (sentence)
  {
    Sentence_freeMem(sentence);
    free(sentence);
    MALLOC_COUNTER--;
  }
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
