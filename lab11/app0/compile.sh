#!/bin/bash
gcc -Wall -O3 -c main.c
gcc -Wall -O3 -c database_functions.c
gcc -Wall -O3 -c fileHandler.c
gcc -Wall -O3 -c utils.c
gcc -Wall -o  app1 main.o database_functions.o fileHandler.o utils.o
