#!/bin/bash
echo "Compiling and linking project files..."
gcc -Wall -O3 -c main.c
gcc -Wall -O3 -c database_functions.c
gcc -Wall -O3 -c fileHandler.c
gcc -Wall -O3 -c ../dependencies/utils.c
gcc -Wall -o  app main.o database_functions.o fileHandler.o utils.o
echo "Deleting out of scope .o files..."
rm *.o
echo "Done."
