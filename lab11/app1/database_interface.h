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

int addPerson(char * name, double wage);

int deletePerson(char * name);

void printDatabase();

void printOrderedByWage();

void freeDatabase();

int saveDatabase();

int loadDatabase();
