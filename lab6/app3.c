#include <stdio.h>

#define VALUE(v)                                                               \
  (((v) >= 'a' && (v) <= 'z' ? 1 : 0) |                                        \
  ((v) >= 'A' && (v) <= 'Z' ? 2 : 0) |                                         \
  ((v) >= '0' && (v) <= '9' ? 4 : 0))

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

#define ISLOWER(c) (table [(c)] & 1)
#define ISUPPER(c) (table [(c)] >> 1 & 1)
#define ISDIGIT(c) (table [(c)] >> 2 & 1)

int main()
{
  unsigned char c;
  printf("Enter a character: ");
  c = getchar();
  putchar('\n');
  printf("Character data:\n");
  printf("\t - value: %c\n", c);
  printf("\t - is lowercase: %d\n", ISLOWER(c));
  printf("\t - is uppercase: %d\n", ISUPPER(c));
  printf("\t - is digit: %d\n", ISDIGIT(c));

  return 0;
}
