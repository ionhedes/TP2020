#include "database_interface.h"
#include "utils.h"
#include <float.h>

Person * database = NULL;
Person * printedList = NULL;

static Person * newPerson(char * name, const double wage, Person * next)
{
  Person * pers = (Person *)malloc(sizeof(Person));
  if (pers == NULL)
  {
    DEB("Memory allocation failed in call of newPerson();\n");
    return NULL;
  }
  MALLOC_COUNTER++;
  pers->wage = wage;
  pers->name = name;
  pers->next = next;
  return pers;
}

int addPerson(char * name, const double wage)
{
  int return_value;
  Person * buffer = NULL;
  Person * previous = NULL;
  Person * current = NULL;

  DEB("Iterating database looking for the position to add the person at...\n");
  for (current = database; current; previous = current, current = current->next)
  {
    DEB("\t - current entry: %s;\n",current->name);
    return_value = strcmp(current->name, name);
    if (return_value > 0)
    {
      DEB("\t\t - found the position, after the current name;\n");
      break;
    }
    else if (return_value == 0)
    {
      DEB("\t - the person is already listed inside the database;\n");
      return 2;
    }
  }
  buffer = newPerson(name, wage, current);
  if (buffer == NULL)
  {
    freeDatabase();
    DEB("\t\t - error when creating a new person;\n");
    return 0;
  }
  if (previous)
  {
    previous->next = buffer;
  }
  else
  {
    DEB("\t - the new person will be inserted as the head entry in the database;\n");
    database = buffer;
  }
  DEB("Person inserted;\n\n");
  return 1;
}

int deletePerson(const char * name)
{
  Person * previous = NULL;
  Person * current = NULL;

  DEB("Iterating database looking for the person to delete...\n");
  for (current = database; current; previous = current, current = current->next)
  {
    DEB("\t - current entry: %s;\n", current->name);
    if (!strcmp(current->name, name))
    {
      DEB("\t - found person to be deleted (%s = %s);\n", current->name, name);
      if (previous)
      {
        previous->next = current->next;
      }
      else
      {
        DEB("\t - the head entry of the database has changed;\n");
        database = current->next;
      }
      free(current->name);
      free(current);
      MALLOC_COUNTER -= 2;
      DEB("Person deleted;\n\n");
      return 1;
    }
  }
  DEB("The person was not found in the database;\n\n");
  return 0;
}

int existsPerson(const char * name)
{
  Person * current = NULL;

  DEB("Iterating the list, looking for the person with name %s...\n", name);
  for (current = database; current; current = current->next)
  {
    DEB("\t - current entry: %s;\n", current->name);
    if (!strcmp(name, current->name))
    {
      DEB("\t\t - found matching entry;\n");
      return 1;
    }
  }
  DEB("An entry with this name was not found;\n");
  return 0;
}

void printDatabase()
{
  int counter = 0;
  Person * current = NULL;

  if (!database)
  {
    printf("The database is empty.\n");
  }
  else
  {
    printf("People inside the database:\n");
    for (current = database; current; current = current->next)
    {
      printf("\t - person %d:\n", ++counter);
      printf("\t\t - name: %s;\n", current->name);
      printf("\t\t - wage: %.2f;\n", current->wage);
    }
  }
  putchar('\n');
}

static void findCurrentLeastPaid(const Person * most_paid, int iteration_number)
{
  Person * current = database;
  Person * least_paid = database;
  double placeholder;

  DEB("Iteration %d of list...\n", iteration_number + 1);
  DEB("\t - finding current minimum...\n");
  for (current = database->next; current; current = current->next)
  {
    if (current->wage <= least_paid->wage)
    {
      least_paid = current;
    }
  }
  DEB("\t - current minimal wage value:\n");
  DEB("\t\t - name: %s;\n", least_paid->name);
  DEB("\t\t - wage: %lf;\n", least_paid->wage);

  printf("\t - person %d:\n\t\t - name: %s;\n\t\t - wage: %.2lf;\n",
         iteration_number + 1, least_paid->name, least_paid->wage);

  if (iteration_number &&
      most_paid->wage == least_paid->wage &&
      !strcmp(most_paid->name, least_paid->name))
  {
    DEB("\t - all the people have been printed;\n");
    DEB("\t - recursive call ended;\n");
  }
  else
  {
    DEB("\t - setting the lowest wage to the biggest possible number, so we don't count it twice;\n");
    placeholder = least_paid->wage;
    least_paid->wage = DBL_MAX;
    findCurrentLeastPaid(most_paid, iteration_number + 1);
    least_paid->wage = placeholder;
    DEB("\t - the lowest wage of iteration %d was restored;\n", iteration_number + 1);
    DEB("Iteration %d ended;\n", iteration_number + 1);
  }
}

void printOrderedByWage()
{
  Person * current = NULL;
  Person * most_paid = database;

  DEB("Finding the best paid person in the database...\n");
  for (current = database->next; current; current = current->next)
  {
    if (current->wage >= most_paid->wage && strcmp(current->name, most_paid->name) > 0)
    {
      most_paid = current;
      DEB("\t - current best paid person:\n");
      DEB("\t\t - name: %s;\n", most_paid->name);
      DEB("\t\t - wage: %lf;\n", most_paid->wage);
    }
  }
  DEB("Global best paid person:\n");
  DEB("\t - name: %s;\n", most_paid->name);
  DEB("\t - wage: %lf;\n", most_paid->wage);
  DEB("\n");

  findCurrentLeastPaid(most_paid, 0);
}

void freeDatabase()
{
  Person * current = NULL;
  Person * next = NULL;
  for (current = database; current; current = next)
  {
    next = current->next;
    free(current->name);
    free(current);
    MALLOC_COUNTER -= 2;
  }
  database = NULL; // < For later reuse;
}
