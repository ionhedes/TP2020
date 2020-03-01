#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NUMBER_LENGTH 13

unsigned int isBisect(unsigned int year)
{
  if (year % 4 == 0)
  {
    if (year % 100 == 0 && year % 400 != 0)
    {
      return 0;
    }
    return 1;
  }
  return 0;
}

int checkNumber(int length, char * number)
{
  int counter;
  if (length != NUMBER_LENGTH)
  {
    return 0;
  }
  for (counter = 0; counter < length; counter++)
  {
    if (number [counter] < '0' || number [counter] > '9')
    {
      return 0;
    }
  }
  return 1;
}

void printGender(char * number)
{
  if (number [0] == '1' || number [0] == '5')
  {
    printf("male");
  }
  else if (number [0] == '2' || number [0] == '6')
  {
    printf("female");
  }
  else
  {
    printf("The number format is invalid");
    exit(EXIT_FAILURE);
  }
}

void printBirthDate(char *number)
{
  unsigned relative_year, absolute_year, month, day;
  unsigned int day_number_per_month [] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  relative_year = (number [1] - '0') * 10 + (number [2] - '0');
  absolute_year = relative_year + ((number [0] == '5' || number [0] == '6') ? 2000 : 1900);
  if(isBisect(absolute_year))
  {
    day_number_per_month [2] = 29;
  }

  month = (number [3] - '0') * 10 + (number [4] - '0');
  if (month > 12)
  {
    printf("The number format is invalid\n");
    exit(EXIT_FAILURE);
  }

  day = (number [5] - '0') * 10 + (number [6] - '0');
  if (day > day_number_per_month [month])
  {
    printf("The number format is invalid\n");
    exit(EXIT_FAILURE);
  }

  printf("%d.%d.%d", absolute_year, month, day);

}

void parseNumber(char * number)
{
  printf("The person is: ");
  printGender(number);
  printf(", born on: ");
  printBirthDate(number);
  printf(";\n");
}

int main(int argc, char * argv [])
{
  if (argc == 1)
  {
    printf("There is no input personal ID number\n");
  }
  else if (argc > 2)
  {
    printf("Too many arguments given\n");
  }
  else if (!checkNumber(strlen(argv [1]), argv [1]))
  {
    printf("The number doesn't match the format\n");
  }
  else
  {
    parseNumber(argv [1]);
  }
  return 0;
}
