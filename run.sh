#!/bin/bash
echo 'Do you want to use GUI or write the expression in CLI'
read input
if [ "$input" == "GUI" ]; then
python3 calc_gui.py
elif [ "$input" == "CLI" ]; then
echo 'expression format example: 6*(4-2)+3'
read expression
echo "$expression" > generated/1_expression.txt
else
echo 'unexpected input, defaulting to GUI'
python3 calc_gui.py
fi

./OOP-parsing
./intermed-codegen
./intermed-to-asm
nasm -f elf64 generated/4_ASM.s -o generated/MATH.o
ld generated/MATH.o -o generated/MATH
./generated/MATH
