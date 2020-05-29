/**
  Aplicația 13.1: Modificați exemplul 1, astfel încât să fie permise și deplasările
                  pe diagonală, iar traseul să fie marcat cu litere în succesiunea ‘a’..’z’.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WALL '#'
#define ROAD '.'
#define DIRNUMBER 8
#define SOLUTION_LIMIT 100

unsigned MALLOC_COUNTER;
unsigned OPENED_FILES_COUNTER;

typedef struct
{
  int x;
  int y;
}Point;

char ** labyrinth = NULL; // < The labyrinth matrix;
Point * visited_points = NULL; // < Array for holding paths through the labyrinth, as they are created;
int length; // < Dimension of the square matrix labyrinth;
Point direction [DIRNUMBER] = {
  {0, 1},
  {1, 1},
  {1, 0},
  {1, -1},
  {0, -1},
  {-1, -1},
  {-1, 0},
  {-1, 1}
};

// Frees the memory holding the labyrinth matrix;
void freeLabyrinth(char ** labyrinth_buffer, int * current_length)
{
  int i;

  // Freeing the lines of the matrix;
  for (i = 0; i < *current_length; i++)
  {
    free(labyrinth_buffer [i]);
    MALLOC_COUNTER--;
  }

  // Freeing the matrix structure;
  free(labyrinth_buffer);
  MALLOC_COUNTER--;
}

// Checks if the given position inside the labyrinth is valid for the current path;
int checkPosition(const Point * current_position, const int * level)
{
  // Checking the current position inside the labyrinth;
  // Wall check;
  if (labyrinth[current_position->x][current_position->y] == WALL)
  {
    // The current point is a wall;
    return 0;
  }
  // Invalid index check;
  if (current_position->x < 0 || current_position->x >= length ||
      current_position->y < 0 || current_position->y >= length)
  {
    // The current point's coordinates are out of bounds;
    return 0;
  }

  // Appartenance check;
  int i;
  for (i = 0; i < *level; i++)
  {
    if (visited_points [i].x == current_position->x &&
        visited_points [i].y == current_position->y)
    {
      // The current point is already in our path;
      return 0;
    }
  }

  // G2G;
  return 1;
}

// Prints the labyrinth without add-ons or traces;
void printLabyrinth()
{
  int i;

  printf("The map of the labyrinth:\n");
  for (i = 0; i < length; i++)
  {
    printf("%s\n", labyrinth [i]);
  }
  putchar('\n');
}

// Prints the labyrinth with a solution traced on it;
void printRoad(const int * level)
{
  int i;

  printf("Printing the escape route:\n");

  // Marking the taken route on the labyrinth;
  for (i = 0; i < *level; i++)
  {
    labyrinth [visited_points [i].x][visited_points [i].y] = 'a' + i;
  }

  printLabyrinth();

  // Deleting the marks from the labyrinth, so new routes can be traced later;
  for (i = 0; i < *level; i++)
  {
    labyrinth [visited_points [i].x][visited_points [i].y] = '.';
  }
}

// Backtracking function - computes the path out of the labyrinth;
void backtrack(Point current_position, int level)
{
  // Variable for execution control;
  /** The algorithm takes too long to run, when considering all the possible directions,
      so we limit the number of solutions the algorithm needs to find before stopping;
  */
  static unsigned solution_counter = 0;

  if (solution_counter >= SOLUTION_LIMIT)
  {
    // The maximum number of solutions has been reached;
    return;
  }
  // Checking if we haven't reached the exit yet;
  else if ((current_position.x == length - 1 || current_position.y == length - 1) &&
            labyrinth [current_position.x][current_position.y] == ROAD)
  {
    // The exit has been reached, trace our route and print it;
    printf("Solution #%d;\n", solution_counter++ + 1);
    printRoad(&level);
  }

  // The exit has not yet been reached, iterating further...
  else
  {
    int i;
    Point next_position;
    Point * aux_vp = NULL; // < Auxiliary variable for the array of visited points;

    // Trying all possible directions from the current point;
    for (i = 0; i < DIRNUMBER; i++)
    {
      next_position.x = current_position.x + direction [i].x;
      next_position.y = current_position.y + direction [i].y;

      // Checking the validity of the next position;
      if (checkPosition(&next_position, &level))
      {
        // The position is deemed valid;

        // Resizing the array of visited points to make space for the new position;
        if ((aux_vp = (Point *)realloc(visited_points, (level + 1) * sizeof(Point))) == NULL)
        {
          fprintf(stderr, "Backtracking failed due to memory allocation.\n");
          freeLabyrinth(labyrinth, &length);
          if (visited_points)
          {
            free(visited_points);
            MALLOC_COUNTER--;
          }
          printf("Before the end of execution we have:\n\t - %d unreleased memory blocks;\n\t - %d files still opened;\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
          printf("Execution ended due to an error.\n");
          exit(EXIT_FAILURE);
        }
        MALLOC_COUNTER += (visited_points == NULL ? 1 : 0);
        visited_points = aux_vp;
        visited_points [level] = next_position; // < Inserting the next position inside the array of visited points;

        // Progressing to the next step;
        backtrack(next_position, level + 1);
      }
    }
  }
}

// I/O function for reading the labyrinth matrix from a text file;
char ** read(Point * starting_point)
{
  FILE * file = NULL;
  char ** labyrinth_buffer = NULL; // < The matrix;

  // Opening file;
  if ((file = fopen("labyrinth.txt", "rb")) == NULL)
  {
    fprintf(stderr, "The file was not found;\n");
    return NULL;
  }
  OPENED_FILES_COUNTER++;

  // Reading matrix dimensions and starting point;
  if (fscanf(file, "%d", &length) != 1)
  {
    fclose(file);
    OPENED_FILES_COUNTER--;
    fprintf(stderr, "Could not read from file;\n");
    return NULL;
  }
  if (fscanf(file, "%d %d", &(starting_point->x), &(starting_point->y)) != 2)
  {
    fclose(file);
    OPENED_FILES_COUNTER--;
    fprintf(stderr, "Could not read from file;\n");
    return NULL;
  }
  fgetc(file); // < Consumes the \n left over by fscanf;

  // Allocating memory and reading the  matrix;
  if ((labyrinth_buffer = (char **)malloc(length * sizeof(char *))) == NULL)
  {
    fclose(file);
    OPENED_FILES_COUNTER--;
    fprintf(stderr, "Could not assign memory for the matrix(1).\n");
    return NULL;
  }
  MALLOC_COUNTER++;

  int i, j;
  for (i = 0; i < length; i++, j = i + 1)
  {
    if ((labyrinth_buffer [i] = (char *)malloc((length + 2) * sizeof(char))) == NULL)
    {
      fprintf(stderr, "Could not assign memory for row #%d of the matrix.\n", i);
      freeLabyrinth(labyrinth_buffer, &i);
      fclose(file);
      OPENED_FILES_COUNTER--;
      return NULL;
    }
    MALLOC_COUNTER++;
    if (fgets(labyrinth_buffer [i], length + 2, file) == NULL)
    {
      fprintf(stderr, "Could not read row #%d of the matrix from file.\n", i);
      freeLabyrinth(labyrinth_buffer, &j);
      fclose(file);
      OPENED_FILES_COUNTER--;
      return NULL;
    }
    labyrinth_buffer [i][strcspn(labyrinth_buffer [i], "\n")] = '\0';
  }

  fclose(file);
  OPENED_FILES_COUNTER--;
  return labyrinth_buffer;
}

// Driver program for the labyrinth solving algorithm;
int main()
{
  Point starting_point;
  if ((labyrinth = read(&starting_point)) == NULL)
  {
    fprintf(stderr, "Failed to read the map of the labyrinth.\n");
    printf("Before the end of execution we have %d unreleased memory blocks and %d files still opened.\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  if ((visited_points = (Point *)realloc(visited_points,  sizeof(Point))) == NULL)
  {
    fprintf(stderr, "Failed to initialize path array.\n");
    freeLabyrinth(labyrinth, &length);
    printf("Before the end of execution we have:\n\t - %d unreleased memory blocks;\n\t - %d files still opened;\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
  }
  MALLOC_COUNTER++;
  visited_points [0] = starting_point;

  printf("The backtracking algorithm is limited to 100 solutions.\n\n");
  printf("The labyrinth untraced:\n");
  printLabyrinth(labyrinth, length);

  backtrack(starting_point, 1);

  printf("The labyrinth untraced:\n");
  printLabyrinth(labyrinth, length);

  // Freeing the memory and exiting
  freeLabyrinth(labyrinth, &length);
  free(visited_points);
  MALLOC_COUNTER--;

  printf("Before the end of execution we have:\n\t - %d unreleased memory blocks;\n\t - %d files still opened;\n", MALLOC_COUNTER, OPENED_FILES_COUNTER);
  printf("Execution ended successfully.\n");
  return 0;
}
