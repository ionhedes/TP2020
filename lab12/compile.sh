#!/bin/bash
echo "Compiling and linking project files..."
echo "- utils.c;"
gcc -Wall -O3 -c dependencies/utils.c
echo "- app1.c"
gcc -Wall -O3 -c app1.c
echo "- app2.c"
gcc -Wall -O3 -c app2.c
echo "- app3.c"
gcc -Wall -O3 -c app3.c
gcc -Wall -o app1 utils.o app1.o -lm
gcc -Wall -o app2 utils.o app2.o -lm
gcc -Wall -o app3 utils.o app3.o -lm
echo "Done."
