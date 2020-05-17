#!/bin/bash
echo "Compiling and linking project files..."
echo "Using DEBUG mode..."
gcc -Wall -DDEBUG -c main.c
gcc -Wall -DDEBUG -c database_functions.c
gcc -Wall -DDEBUG -c fileHandler.c
gcc -Wall -DDEBUG -c ../dependencies/utils.c
gcc -Wall -o  app main.o database_functions.o fileHandler.o utils.o
echo "Deleting out of scope .o files..."
rm *.o
echo "Done."
