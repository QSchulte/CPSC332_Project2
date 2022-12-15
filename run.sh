#!/bin/bash

#Delete some un-needed files
rm *.o
rm *.lis
rm *.out

# echo "Bash: The script file for Prog1 has begun"

# echo "Bash: Compile Prog1.cpp"
g++ -c -m64 -Wall -fno-pie -no-pie -o Prog1.o Prog1.cpp -std=c++17

#echo "Bash: Link the object files"
g++ -m64 -no-pie -no-pie -o Prog1.out Prog1.o -std=c++17

#echo "Bash: Run the program:"
./Prog1.out

# echo "The script file will terminate"