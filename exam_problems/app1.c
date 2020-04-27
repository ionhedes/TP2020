#include <stdio.h>
#include <stdlib.h>

unsigned MALLOC_COUNTER;

typedef struct
{
  double x;
  double y;
}
Point;
typedef struct
{
  Point head;
  Point tail;
}
Segment;

Segment * readPoints(FILE * stream, unsigned * array_size)
{
  Segment * array = NULL;
  Segment * aux = NULL;
  Segment buffer;
  while(!feof(stream))
  {
    printf("----\n");
    if (fscanf(stream, "%lf %lf %lf %lf", &buffer.head.x, &buffer.head.y, &buffer.tail.x, &buffer.tail.y) != 4 && ferror(stream))
    {
      free(array);
      return NULL;
    }
    if ((aux = (Segment *)realloc(array, (*array_size + 1) * sizeof(Segment))) == NULL)
    {
      free(array);
    }
    MALLOC_COUNTER += (*array_size == 0 ? 1 : 0);
    array = aux;
    array [*array_size].head.x = buffer.head.x;
    array [*array_size].head.y = buffer.head.y;
    array [*array_size].tail.x = buffer.tail.x;
    array [*array_size].tail.y = buffer.tail.y;
    (*array_size)++;
  }
  (*array_size)--; // text file always has one extra line, weird

  return array;
}

int isValid(Segment item, Segment * current_road, unsigned level)
{
  int i;
  if (level == 0)
  {
    return 1;
  }
  else if (current_road [level - 1].tail.x != item.head.x ||
      current_road [level - 1].tail.y != item.head.y)
      {
        return 0;
      }
  for (i = 0; i < level; i++)
  {
    if (current_road [i].head.x == item.head.x &&
        current_road [i].head.y == item.head.y &&
        current_road [i].tail.x == item.tail.x &&
        current_road [i].tail.y == item.tail.y)
        {
          return 0;
        }
  }
  return 1;

}

void backtrack(Segment * array, unsigned * array_size, unsigned level, Segment * current_road)
{
  printf("current level: %d;\n", level);
  printf("array size: %d\n", *array_size);
  if (level == *array_size)
  {
    printf("Found road!\n");
    int j;
    for (j = 0; j < *array_size; j++)
    {
      printf("\t - head: %.2f %.2f, tail: %.2f %.2f;\n", current_road [j].head.x, current_road [j].head.y, current_road [j].tail.x, current_road [j].tail.y);
    }
  }
  else
  {
    int i;
    for (i = 0; i < *array_size; i++)
    {
      if (isValid(array [i], current_road, level))
      {
        current_road [level] = array [i];
        int j;
        for (j = 0; j <= level; j++)
        {
          printf("\t - head: %.2f %.2f, tail: %.2f %.2f;\n", current_road [j].head.x, current_road [j].head.y, current_road [j].tail.x, current_road [j].tail.y);
        }
        putchar('\n');
        backtrack(array, array_size, level + 1, current_road);
      }
    }
  }
}

int findSegmentRoad(Segment * array, unsigned * array_size)
{
  Segment * road = NULL;

  if ((road = (Segment *)malloc(*array_size * sizeof(Segment))) == NULL)
  {
    return 0;
  }
  backtrack(array, array_size, 0, road);
  return 1;
}

int main()
{
  unsigned array_size = 0;
  Segment * array = NULL;
  FILE * data_file;

  if ((data_file = fopen("data_file.txt", "r")) == NULL)
  {
    fprintf(stderr, "Opening file failed.\n");
    exit(EXIT_FAILURE);
  }

  if ((array = readPoints(data_file, &array_size)) == NULL)
  {
    fprintf(stderr, "Reading the points failed.\n");
    exit(EXIT_FAILURE);
  }
  fclose(data_file);

  if (!findSegmentRoad(array, &array_size))
  {
    fprintf(stderr, "Error while running the road finder algorithm.\n");
    free(array);
    MALLOC_COUNTER--;
    exit(EXIT_FAILURE);
  }


  free(array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  printf("Exiting...\n");
  return 0;
}
