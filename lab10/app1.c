#include <stdio.h>
#include "myStdlib.h"

typedef struct Word Word;

struct Word
{
  int occurence_counter;
  char * content;
  Word * previous;
  Word * next;
};

#define DLIST_NAME Sentence
#define DLIST_ATOM Word
#define ADDITIONAL_ATOM_FIELD content
#include "doubleListHandler.h"

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

  do
  {
    printf("Enter a word: ");
    buffer_string = getString(stdin);
    if (buffer_string == NULL)
    {
      DEB("Failed to read a word from keyboard input.\n");
      Sentence_freeMem(sentence);
      free(sentence);
      MALLOC_COUNTER--;
      return NULL;
    }
    buffer_word = Sentence_seek(sentence, buffer_string);

    if (buffer_word == NULL)
    {
      DEB("The word was not found inside the sentence, inserting it...;\n");
      buffer_word = Word_create(buffer_string);
      if (buffer_word == NULL)
      {
        Sentence_freeMem(sentence);
        free(sentence);
        free(buffer_string);
        MALLOC_COUNTER -= 2;
        return NULL;
      }
      Sentence_add(sentence, buffer_word);
    }
    else
    {
      DEB("The word was found inside the sentence, increasing its occurence_counter...\n\n");
      free(buffer_string);
      MALLOC_COUNTER--;
    }
    buffer_word->occurence_counter++;
  }
  while (buffer_string && *buffer_word->content != '.');

  DEB("Done.\n\n");
  return sentence;
}

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

int main()
{
  unsigned go_condition = 1;
  unsigned option;
  Sentence * sentence = NULL;

  while (go_condition)
  {
    generateMenu();
    if (!get_unsigned(&option, stdin))
    {
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
    switch (option)
    {
      case 1:
      {
        printf("Creating new sentence...\n");
        sentence = Sentence_create();
        break;
      }
      case 2:
      {
        printf("Printing sentence...\n");
        Sentence_print(sentence);
        putchar('\n');
        break;
      }
      case 3:
      {
        printf("Deleting a word from the sentence...\n");
        if ((sentence = Word_delete(sentence)) == NULL)
        {
          fprintf(stderr, "Error when reading word to be deleted.\n");
          printf("Execution ended.\n");
          DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
          printf("Exiting...\n");
          exit(EXIT_FAILURE);
        }
        putchar('\n');
        break;
      }
      case 4:
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
