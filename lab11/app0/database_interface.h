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

extern Person * database;

int addPerson(char * name, const double wage);

int deletePerson(const char * name);

int existsPerson(const char * name);

void printDatabase();

void freeDatabase();

int saveDatabase();

int loadDatabase();
