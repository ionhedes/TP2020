#include <stdio.h>
#include <errno.h>
#include "myStdlib.h"

#define GET(type, var)                                                         \
  {                                                                            \
    printf("Enter the "#type" value for "#var": ");                            \
    if (!get##type(&var, isValid##type, stdin))                                \
    {                                                                          \
      errno = 5;                                                               \
    }                                                                          \
  }

#define VALUE(v)                                                               \
  ((((v) >> 0) & 1) + (((v) >> 1) & 1) + (((v) >> 2) & 1) + (((v) >> 3) & 1) + \
  (((v) >> 4) & 1) + (((v) >> 5) & 1) + (((v) >> 6) & 1) + (((v) >> 7) & 1))   \

#define LINE(num) VALUE(16*num+0),VALUE(16*num+1),VALUE(16*num+2),VALUE(16*num+3),	  \
                  VALUE(16*num+4),VALUE(16*num+5),VALUE(16*num+6),VALUE(16*num+7),	  \
                  VALUE(16*num+8),VALUE(16*num+9),VALUE(16*num+10),VALUE(16*num+11),	\
                  VALUE(16*num+12),VALUE(16*num+13),VALUE(16*num+14),VALUE(16*num+15)

const char table [] =
  {
    LINE(0), LINE(1), LINE(2), LINE(3),
    LINE(4), LINE(5), LINE(6), LINE(7),
    LINE(8), LINE(9), LINE(10), LINE(11),
    LINE(12), LINE(13), LINE(14), LINE(15)
  };

#define NBITI4(v)                                                                     \
  (table [((v) & 0b11111111)] + table [((v) >> 8) & 0b11111111] +                     \
  table [((v) >> 16) & 0b11111111] + table [((v) >> 24) & 0b11111111])                \

int main()
{
  int num;
  GET(Int, num)
  printf("Number of set bits: %d;\n", NBITI4((unsigned)num));
  return 0;
}
