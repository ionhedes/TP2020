#include <stdio.h>
#include <stdlib.h>

unsigned int malloc_counter = 0;

int isEmpty(char * word)
{
  int check = (word == NULL) ? 1 : 0;
  return check;
}


char * getWord()
{
  size_t length = 0;
  char buffer_char;
  char * word = NULL, * aux = NULL;

  printf("Enter word: ");
  while((buffer_char = getc(stdin)) != '\n')
  {
    if ((aux = (char *)realloc(word, (length + 1) * sizeof(char))) == NULL)
    {
      printf("Something went wrong. Exiting...");
      free(word);
      malloc_counter--;
      exit(EXIT_FAILURE);
    }
    word = aux;
    malloc_counter += (!length ? 1 : 0);
    *(word + length) = buffer_char;
    length++;
  }
  if (word)
  {
    *(word + length) = '\0';
  }

  return word;
}

int main()
{
  char * word = NULL;

  word = getWord();
  while (!isEmpty(word))
  {
    printf(" - %s;\n\n", word);
    free(word);
    malloc_counter--;
    word = getWord();
  }

  printf("\nEmpty line reached. Exiting...\n\n");
  //printf("DEBUG: %d dinamically allocated memory blocks unreleased;\n\n", malloc_counter);
  return 0;
}
