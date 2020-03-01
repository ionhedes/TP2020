#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv [])
{
  int counter;
  double sum;

  for (counter = 1; counter < argc; counter++)
  {
    sum += atof(argv [counter]);
  }
  printf("The sum is %.3f;\n\n", sum);

  return 0;
}
