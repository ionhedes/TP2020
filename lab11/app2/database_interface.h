// Interface for declaring database functions

#pragma once

#define MAX_LENGTH 31

typedef struct Person Person;

struct Person
{
  char * name;
  double wage;
  Person * next;
};

// Declaration for the database;
extern Person * database;

// Adds a person to the database;
/**
  USAGE:
    - only use ths function to add to the database;
    - creates a new entry in the database with the specified name, gender and wage;
    - the insertion is made so that the alphabetical order is preserved;

  ARGUMENTS:
    - char * name - the name of the person;
    - const char gender - the gender of the person;
    - const double wage - the wage of the person;

  RETURN VALUE:
    - on success - 1;
    - on failure, person is already inserted - 2;
    - on failure, error - 0;

  SIDE EFFECTS:
    - no outside variables will be modified by this method;
    - warning, if the function fails, the person will not be added to the database,
      and you will have to free char * name yourself, since freeDatabase() will
      not cover it;
*/
int addPerson(char * name, const double wage);

// Deletes a person from the database;
/**
  USAGE:
    - only use this function to delete from the database;
    - use it to delete a person from the database, based on its name;

  ARGUMENTS:
    - char * name - the name of the person to be deleted;

  RETURN VALUE:
    - on success - 1;
    - on failure, the person was not found - 0;
    - this method should not throw errors;

  SIDE EFFECTS:
    - no outside variables will be modified by this method;
    - you need to free the recipient char * name after calling this function;
*/
int deletePerson(const char * name);

// Checks if a person exists inside ;
/**
  USAGE:
    - use it to check if a person is inside the database;
    - the search is done based on the name;

  ARGUMENTS:
    - const char * name - the name of the person to be searched;

  RETURN VALUE:
    - on success, person found - 1;
    - on failure, person not found - 0;

  SIDE EFFECTS:
    - no outside variables will be modified by this method;
*/
int existsPerson(const char * name);

// Prints the database's entries;
void printDatabase();

// Prints the database's entries, ordered by wage;
/**
  USAGE:
    - call it to print the entries in the database, ordered ascendingly by wage;

  ARGUMENTS:
    - none, the database structure is globally visible;

  RETURN VALUE:
    - none, all the outputs of this function are printed to stdin;

  SIDE EFFECTS:
    - the entries inside the database are not sorted during the execution of this
      function, so getting the right entry order right when printing takes more
      time;
    - during the call of this function, all the entries in the database will
      shortly have their wage value altered. The initial values are restored
      before returning to the caller;
*/
void printOrderedByWage();

// Frees the dinamically assigned memroy blocks used by the database
// DROP TABLE *;
void freeDatabase();

// Saves the database to a text file on the disk and deletes it from memory;
int saveDatabase();

// Loads the database from a text file on the disk into memory;
int loadDatabase();
