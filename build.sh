#!/bin/bash
echo "Compiling all approaches..."

# Create the bin folder if it doesn't exist
mkdir -p bin

# Compile each C file and put the executable in the bin folder
gcc -O3 -pthread src/ap1.c -o bin/run1
gcc -O3 -pthread src/ap2.c -o bin/run2
gcc -O3 -pthread src/ap3.c -o bin/run3
gcc -O3 -pthread src/ap3_modify.c -o bin/run3_mod
gcc -O3 -pthread src/thread_scalling.c -o bin/ts

echo "Compilation complete! Executables are in the bin/ folder."