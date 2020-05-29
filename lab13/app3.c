/**
  Aplicația 13.3: Se cere un 0<n<=100 și apoi n valori reale, fiecare reprezentând
                  volumul unui obiect. În final se cere v, volumul unei cutii.
                  Se cere să se umple cutia cu unele dintre obiecte date, astfel
                  încât volumul ei să fie utilizat în mod optim.
*/

#include <stdio.h>
#include <stdlib.h>

// Necessary variables;
unsigned MALLOC_COUNTER;
double object_no;
double container_volume;
double * object_array = NULL;
int * fitting_objects_array = NULL;

// Prints solution - how were the objects fitted inside the container;
void printFittedObjects(const int number)
{
  int i;
  printf("The objects that were fitted inside the container are:\n");
  for (i = 0; i < number; i++)
  {
    printf("\t - object: #%d -> volume: %lf litres;\n", fitting_objects_array [i] + 1, object_array [fitting_objects_array [i]]);
  }
  putchar('\n');
}

// Checks if an object can be fitted inside the container;
/**
  ARGUMENTS:
    - index - the index of the object to be checked, from the object array;
    - remaining_volume - the remaining volume inside the container;
    - level - the 'deepness' of the backtracking recursive call, also the number
              of objects already stored in the container;

  RETURN VALUE:
    - 1 - the checked object can be fitted in the container;
    - 0 - the checked object cannot be fitted in the container (1 or more reasons);
*/
int isValidObject(const int index, const double remaining_volume, const int level)
{
  int i;

  // Appartenance test - isn't the object inside the container already?;
  for (i = 0; i < level; i++)
  {
    if (fitting_objects_array [i] == index)
    {
      // The object was already inside;
      return 0;
    }
  }

  // Overflow test - is there still enough space for the new object inside the container?
  if (remaining_volume - object_array [index] < 0)
  {
    // There is no sufficient space;
    return 0;
  }

  // G2G;
  return 1;
}

// Backtracking function - computes all the ways we can fit objects inside the container;
void backtrack(double remaining_volume, int level)
{
  int i;
  int ok = 0;
  int * aux_foa = NULL;

  if (remaining_volume < 0)
  {
    return;
  }

  for (i = 0; i < object_no; i++)
  {
    if (isValidObject(i, remaining_volume, level - 1))
    {
      ok = 1;
      if ((aux_foa = (int *)realloc(fitting_objects_array, level * sizeof(int))) == NULL)
      {
        free(object_array);
        MALLOC_COUNTER--;
        if (fitting_objects_array)
        {
          free(fitting_objects_array);
          MALLOC_COUNTER--;
        }
      }
      MALLOC_COUNTER += (fitting_objects_array == NULL ? 1 : 0);
      fitting_objects_array = aux_foa;

      fitting_objects_array [level - 1] = i;
      backtrack(remaining_volume - object_array [i], level + 1);
    }
  }
  if (!ok)
  {
    printFittedObjects(level - 1);
  }
}

// Driver function for the backtracking function;
int main()
{
  int i;

  // Reading volume of container, number of objects, volume of objects;
  printf("Enter the number of objects: ");
  if (scanf("%lf", &object_no) != 1)
  {
    fprintf(stderr, "\t - I/O error, exiting...\n");
    exit(EXIT_FAILURE);
  }
  printf("Enter the volume of the container: ");
  if (scanf("%lf", &container_volume) != 1)
  {
    fprintf(stderr, "\t - I/O error, exiting...\n");
    exit(EXIT_FAILURE);
  }

  if ((object_array = (double *)malloc(object_no * sizeof(double))) == NULL)
  {
    fprintf(stderr, "\t - malloc() error, exiting...\n");
    exit(EXIT_FAILURE);
  }
  MALLOC_COUNTER++;
  for (i = 0; i < object_no; i++)
  {
    printf("\t - enter the volume of the #%d object: ", i + 1);
    if (scanf("%lf", &object_array [i]) != 1)
    {
      fprintf(stderr, "\t\t - I/O error, exiting...\n");
      free(object_array);
      MALLOC_COUNTER--;
      exit(EXIT_FAILURE);
    }
  }
  putchar('\n');

  printf("Printing all the possible ways of filling the container")
  backtrack(container_volume, 1);

  free(object_array);
  free(fitting_objects_array);
  MALLOC_COUNTER -= 2;
  printf("Before the end of execution we have:\n\t - %d unreleased memory blocks;\n", MALLOC_COUNTER);
  printf("Execution ended successfully.\n");
  return 0;
}
