/**
  Aplicația 12.1: Se cere un n<100, iar apoi coordonatele reale (x,y) a n puncte
                  din plan. Fiecare punct reprezintă poziția unde va trebui dată
                  o gaură într-o placă. Bormașina se află inițial la locația (0,0).
                  Să se afișeze ordinea de dat găuri, astfel încât întotdeauna
                  bormașina să fie mutată la punctul cel mai apropiat de ea.
*/

#include "dependencies/utils.h"
#include <math.h>
#include <float.h> // < used for the DBL_MAX macro - maximum double value;

// 2D point structure definition;
typedef struct
{
  double x;
  double y;
}
Point;

// Function to fill the array with values for its points;
// Further comments for this method are out of scope;
int fillArray(Point * point_array, const unsigned * point_number)
{
  int iter;

  DEB("Inserting points in array...\n");
  for (iter = 0; iter < *point_number; iter++)
  {
    printf("Enter values for point #%d:\n", iter + 1);
    printf("\t - enter an x: ");
    if (!get_double(&(point_array [iter].x), stdin))
    {
      DEB("\t\t - failed to read a double value from the standard input.\n");
      return 0;
    }
    printf("\t - enter an y: ");
    if (!get_double(&(point_array [iter].y), stdin))
    {
      DEB("\t\t - failed to read a double value from the standard input.\n");
      return 0;
    }
  }
  DEB("Done;\n");
  return 1;
}

// Use to create an array of points
/**
  USAGE:
    - use to create an array of points from scratch;
    - reads an unsigned value, and then creates an array of that number of points;
    - reads data for the points using fillArray();

  ARGUMENTS:
    - unsigned * point_number - pointer to the number of points;

  RETURN VALUE:
    - on success - pointer to the array of points;
    - on failure - null pointer;

  SIDE EFFECTS:
    - the variable point_number points to is set in the caller function;
*/
Point * initArray(unsigned * point_number)
{
  Point * point_array = NULL;

  DEB("Initializing point array...\n");
  printf("Enter the number of desired points: ");
  if (!get_unsigned(point_number, stdin))
  {
    DEB("\t - failed to read number of points from the standard input;\n");
    return NULL;
  }
  if ((point_array = (Point *)malloc(*point_number * sizeof(Point))) == NULL)
  {
    DEB("\t - failed to assign memory for the point array;\n");
    return NULL;
  }
  MALLOC_COUNTER++;
  if (!fillArray(point_array, point_number))
  {
    DEB("\t - failed to fill array;\n");
    free(point_array);
    MALLOC_COUNTER--;
    return NULL;
  }
  DEB("Done;\n");
  return point_array;
}

// Function to print the point array;
// Further comments for this method are out of scope;
void printArray(const Point * point_array, const unsigned * point_number)
{
  int iter;
  printf("The array of points is:\n");
  for (iter = 0; iter < *point_number; iter++)
  {
    printf("\t - point #%d:\n", iter + 1);
    printf("\t\t - (x: %lf; y: %lf);\n", point_array [iter].x, point_array [iter].y);
  }
}

// Function to compute the distance between two points
/**
  USAGE:
    - use it to compute the distance between two points;
    - uses the well-known mathematical formula;

  ARGUMENTS:
    - const Point * point1 - the first point;
    - const Point * point2 - the second point;

  RETURN VALUE:
    - the computed distance between the points;

  SIDE EFFECTS:
    - no outside variables will be affected by this function;
*/
double distance(const Point * point1, const Point * point2)
{
  double result;
  result = sqrt(pow((point2->x - point1->x), 2) + pow((point2->y - point1->y), 2));
  return result;
}

// Function to determine if a certain point was already visited
/**
  USAGE:
    - use by findClosestPoint() to determine if a point
      was already visited;
    - given an array of the addresses of already visited points, this function
      tells you if the current point was already visited;

  ARGUMENTS:
    - const Point * current_point - the address of the point to be checked;
    - Point ** visited_points - array holding the addresses of the points already
      visited;

  RETURN VALUE:
    - 0 - not yet visited;
    - 1 - already visited;

  SIDE EFFECTS:
    - no outside variables will be affected by this function;
*/
int wasPointVisited(const Point * current_point, Point ** visited_points)
{
  int iter;
  DEB("Checking if point was already visited...\n");
  DEB("Looking for address %p...\n", current_point);
  for (iter = 0; *(visited_points + iter); iter++)
  {
    DEB("\t - current entry: %p;\n", *(visited_points + iter));
    if (*(visited_points + iter) == current_point)
    {
      DEB("The point was already visited;\n");
      return 1;
    }
  }
  DEB("The point was not yet visited;\n");
  return 0;
}

// Function to find the closest valid point to move to;
/**
  - a point is deemed valid if it was not yet visited;

  USAGE:
    - used by findPointPath to find the next step when creating a path through
      the given points;

  ARGUMENTS:
    - const Point * current_point - the last point in the path;
    - Point * point_array - the array of available points to go to;
    - const unsigned * point_number - pointer to the number of points in point_array;
    - Point ** visited_points - array holding the points already along the path;

  RETURN VALUE:
    - a Point * holding the address of the suitable next step in the path;
    - this should not throw any error;

  SIDE EFFECTS:
    - no outside variables will be affected by this function;
*/
Point * findClosestPoint(const Point * current_point, Point * point_array, const unsigned * point_number,
                         Point ** visited_points)
{
  int iter;
  double current_distance;
  double smallest_distance = DBL_MAX;
  Point * closest_point = NULL;

  DEB("Finding closest point to (x: %lf; y: %lf), not yet visited...\n", current_point->x, current_point->y);
  for (iter = 0; iter < *point_number; iter++)
  {
    DEB("\t - current entry: (x: %lf; y: %lf);\n", (point_array + iter)->x, (point_array + iter)->y);
    current_distance = distance(current_point, point_array + iter);
    if (current_distance < smallest_distance &&
        current_point != (point_array + iter) &&
        !wasPointVisited(point_array + iter, visited_points))
    {
      DEB("\t - found new closest point:\n");
      DEB("\t\t - (x: %lf; y: %lf);\n", (point_array + iter)->x, (point_array + iter)->y);
      DEB("\t\t - distance: %lf;\n", current_distance);
      closest_point = point_array + iter;
      smallest_distance = current_distance;
    }
  }
  return closest_point;
}

// Function to create a path through set points;
/**

  - this function follows the Greedy paradigm - for every step, computing the
    local optimal value, in our case, finding the closest point to the current
    one;

  USAGE:
    - given a set of points and a starting point, computes a route, taking
      the shortest distance option at every step;

  ARGUMENTS:
    - Point * current_point - the current point in the path; initially the
                              starting point of the path;
    - Point * point_array - the array of available points to go to;
    - const unsigned * point_number - pointer to the number of points in point_array;

  RETURN VALUE:
    - on success, path found - 1;
    - on error - 0;

  SIDE EFFECTS:
    - calloc() is used when creating the array of visited points; the memory is
      also released at the end of the function, so this should not be a problem;
*/
int findPointPath(Point * current_point, Point * point_array, const unsigned * point_number)
{
  unsigned iteration_number = 0;
  Point * closest_point = NULL;
  Point ** visited_points = NULL;

  DEB("Creating array for visited points...\n");
  if ((visited_points = (Point **)calloc(*point_number,  sizeof(Point *))) == NULL)
  {
    DEB("Failed to assign space for the array of visited points;\n");
    return 0;
  }
  MALLOC_COUNTER++;

  printf("Smallest distance per step path is:\n");

  // Looping until every point is covered by our path;
  while(iteration_number < *point_number)
  {
    DEB("Starting iteration number %d...\n", iteration_number + 1);

    // Finding the local optimum - closest point to current point;
    closest_point = findClosestPoint(current_point, point_array, point_number, visited_points);

    // Adding the point to the 'already visited' list;
    visited_points [iteration_number] = current_point;

    // Printing the step in the path;
    printf("\t - (x: %lf; y: %lf);\n", closest_point->x, closest_point->y);
    current_point = closest_point;
    iteration_number++;
  }

  DEB("Array of visited points is out of scope, freeing...\n");
  free(visited_points);
  MALLOC_COUNTER--;
  DEB("Done;\n");
  putchar('\n');
  return 1;
}

// Driver program for the pathfinding functions;
int main()
{
  unsigned point_number;
  Point starting_point = {0, 0};
  Point * point_array = NULL;

  // Creating the array of points;
  if ((point_array = initArray(&point_number)) == NULL)
  {
    err("Failed to create array of points.\n");
  }

  // Computing path;
  if (!findPointPath(&starting_point, point_array, &point_number))
  {
    free(point_array);
    MALLOC_COUNTER--;
    err("Failed to determine path.\n");
  }

  // Freeing memory and exiting;
  free(point_array);
  MALLOC_COUNTER--;
  printf("Execution ended.\n");
  DEB("Unreleased memory blocks: %d;\n", MALLOC_COUNTER);
  printf("Exiting...\n");
  return 0;
}
