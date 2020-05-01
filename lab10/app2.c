/**
  Aplicația 10.2: La exemplul 1 să se adauge operația de inserare a unui cuvânt.
                  Pentru aceasta se cere un cuvânt de inserat și un cuvânt
                  succesor. Dacă succesorul există în propoziție, cuvântul de
                  inserat va fi inserat înaintea sa. Dacă succesorul nu există
                  în lista, cuvântul de inserat va fi adăugat la sfârșitul listei.
*/

#include <stdio.h>
#include "myStdlib.h"

// Provides a definition for the word data type;
/*
  FIELDS:
    - char * content - address to the content of the word, the word itself;
    - Word * previous - link to the previous word (for double lists - sentence);
    - Word * next - link to the next word (for double lists - sentence);
*/
typedef struct Word Word;

struct Word
{
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
    - the previous and next links are set to null by default, since the word doesn't belong
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
  if ((word = (Word *)malloc(sizeof(word))) == NULL)
  {
    DEB("Failed to allocate new word.\n");
    return NULL;
  }
  MALLOC_COUNTER++;
  word->content = content;
  DEB("Checking if the value of the word is correctly set:\n");
  DEB("\t - %s = %s;\n", content, word->content);

  DEB("Initializing the links of the word with null-pointers.\n");
  word->previous = NULL;
  word->next = NULL;

  DEB("Done.\n\n");
  return word;
}

// Function to create a sentence from scratch;
/**
  - as stated in the text of the problem, reading words stops when the input
    string reads ".".
  - all the input words are stored in the sentence, with no regard to the fact
    that they have already appeared;

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

    // Creating a word object from the string and adding it to the sentence
    buffer_word = Word_create(buffer_string);
    if (buffer_word == NULL)
    {
      if (sentence)
      {
        Sentence_freeMem(sentence);
        free(sentence);
        MALLOC_COUNTER--;
      }
      return NULL;
    }
    Sentence_add(sentence, buffer_word);
  }
  while (buffer_string && *buffer_string != '.');
  /**
    Interpreting the stop condition of the do{...}while loop:
      - it must stop when a getString() error occurs and buffer_string becomes
        NULL
      - it must stop when the buffer word becomes a point
  */

  DEB("Done.\n\n");
  return sentence;
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
    - note: if this function fails, the sentence if freed by the function before
      the callback;

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

// Function to insert a word into a sentence
/**
  USAGE:
    - by passing it a sentence pointer, this method requests a word and a
      reference word from the user, and then inserts the given word
      before the reference in the sentence;
    - if the reference word cannot be found in the sentence, the word is simply
      added at the end of the sentence;

  ARGUMENTS:
    - Sentence * sentence - the sentence;

  RETURN VALUE:
    - on success - pointer to the sentence;
    - on failure - NULL pointer;
    - note: if this function fails, the sentence will be freed by the function
      before the callback;

  SIDE EFFECTS:
    - this method doesn't modify any outside variables;
*/
Sentence * Word_insert(Sentence * sentence)
{
  char * insert_word_string = NULL; // < Here we will store the word to be inserted;
  char * reference_word_string = NULL; // < Here we will store the reference word;
  Word * insert_word = NULL; // < Object for the insert_word_string string;
  Word * reference_word = NULL; // < Object for the reference_word_string;

  // Read the insert word and reference word using getString() (myStdlib.h)
  printf("Enter the word to be inserted inside the sentence: ");
  if ((insert_word_string = getString(stdin)) == NULL)
  {
    // Treating input errors;
    DEB("Failed to read the word to be inserted.\n");
    if (sentence)
    {
      Sentence_freeMem(sentence);
      free(sentence);
      MALLOC_COUNTER--;
    }
    return NULL;
  }
  printf("Enter the word for the previous word to be inserted before: ");
  if ((reference_word_string = getString(stdin)) == NULL)
  {
    // Treating input errors;
    DEB("Failed to read the word after which the word should be inserted.\n");
    if (sentence)
    {
      Sentence_freeMem(sentence);
      free(sentence);
      MALLOC_COUNTER--;
    }
    free(insert_word_string);
    MALLOC_COUNTER--;
    return NULL;
  }

  // Envelop the insert word in an Word object
  if ((insert_word = Word_create(insert_word_string)) == NULL)
  {
    // Treating malloc() errors;
    DEB("Failed to create a word object for the inserted word.\n");
    if (sentence)
    {
      Sentence_freeMem(sentence);
      free(sentence);
      MALLOC_COUNTER--;
    }
    free(insert_word_string);
    free(reference_word_string);
    MALLOC_COUNTER -= 2;
    return NULL;
  }

  // Getting the location of the reference word from the sentence;
  reference_word = Sentence_seek(sentence, reference_word_string);

  // Case: the reference word doesn't exist inside the sentence;
  if (reference_word == NULL)
  {
    /**
      Both the cases actually do the same thing, but for this one, we set the
      reference to be the point at the end of the sentence, since we know for
      sure that it exists;
    */
    reference_word = Sentence_seek(sentence, ".");
    DEB("The succeeding word was not found, the insert word will be added at the end of the sentence.\n");
  }

  // Case: the reference was found and we have its address
  else
  {
    DEB("Inserting before the designated word.\n");
  }

  // Fixing the links around the inserted word
  if (reference_word->previous)
  {
    reference_word->previous->next = insert_word;
    insert_word->previous = reference_word->previous;
  }
  else
  {
    DEB("The word will be inserted at the head of the sentence.\n");
    sentence->head = insert_word;
  }
  reference_word->previous = insert_word;
  insert_word->next = reference_word;

  // Freeing the variables that will become out of scope;
  free(reference_word_string);
  MALLOC_COUNTER--;

  DEB("Done.\n");
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
  printf("The sentence is:\n");
  for (iter = sentence->head; iter; iter = iter->next)
  {
    printf("%s ", iter->content);
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
  printf("\t 4 - add word;\n");\
  printf("\t 5 - exit;\n");\
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
          printf("Freeing previously stored sentence...\n");
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
            fprintf(stderr, "Error when deleting word.\n");
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

      // Case: insert a word in the sentence;
      case 4:
      {
        if (sentence)
        {
          printf("Adding a word to the sentence...\n");
          if ((sentence = Word_insert(sentence)) == NULL)
          {
            // Treat function errors;
            fprintf(stderr, "Error when inserting word.\n");
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
      case 5:
      {
        go_condition = 0;
        break;
      }
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
