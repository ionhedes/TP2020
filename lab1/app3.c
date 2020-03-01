#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void encrypt(int argc, char * argv [])
{
  int word_counter, letter_counter;
  for (word_counter = 3; word_counter < argc; word_counter++)
  {
    for (letter_counter = 0; letter_counter < strlen(argv [word_counter]); letter_counter++)
    {
      argv [word_counter][letter_counter] = toupper(argv [word_counter][letter_counter]);
      argv [word_counter][letter_counter] += atoi(argv [2]);
      argv [word_counter][letter_counter] -= (argv [word_counter][letter_counter] > 'Z') ? 26 : 0;
      argv [word_counter][letter_counter] = tolower(argv [word_counter][letter_counter]);
    }
  }
}

void decrypt(int argc, char * argv [])
{
  int word_counter, letter_counter;
  for (word_counter = 3; word_counter < argc; word_counter++)
  {
    for (letter_counter = 0; letter_counter < strlen(argv [word_counter]); letter_counter++)
    {
      argv [word_counter][letter_counter] = tolower(argv [word_counter][letter_counter]);
      argv [word_counter][letter_counter] -= atoi(argv [2]);
      argv [word_counter][letter_counter] += (argv [word_counter][letter_counter] < 'a') ? 26 : 0;
    }
  }
}

void printArgs(int argc, char * argv[])
{
  int word_counter;
  printf("Encrypted text:\n");
  for (word_counter = 3; word_counter < argc; word_counter++)
  {
    printf("%s ", argv [word_counter]);
  }
  putchar('\n');
}

int main(int argc, char * argv [])
{
  if (!strcmp(argv [1], "enc"))
  {
    encrypt(argc, argv);
    printArgs(argc, argv);
  }
  else if (!strcmp(argv[1], "dec"))
  {
    decrypt(argc, argv);
    printArgs(argc, argv);
  }
  else
  {
    printf("Operatia nu e buna\n");
  }
  return 0;
}
