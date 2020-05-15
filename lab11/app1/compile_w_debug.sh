#!/bin/bash
gcc -Wall -DDEBUG -c main.c
gcc -Wall -DDEBUG -c database_functions.c
gcc -Wall -DDEBUG -c fileHandler.c
gcc -Wall -DDEBUG -c utils.c
gcc -Wall -o  app1 main.o database_functions.o fileHandler.o utils.o
