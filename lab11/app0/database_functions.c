#include "database_interface.h"
#include "utils.h"

Person * database = NULL;

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
