# arithmetic-compiler
input a math expression and compile it into a binary that calculates and outputs the answer

it has four main steps:

1. writing the expression
2. parsing the expression to ast following the PEMDAS order of operations
3. generating intermediate code from the ast
4. generating ASM from intermediate code

then the ASM is assembled into a binary that is run, but that's just using nasm and ld

## How it works:

the first step is done using the run.sh/calc_gui script depending on if you want to write the expression in the CLI or if you want a calculator GUI interface, not too much effort is put into this since the point of this project isn't to provide good UX, rather it's just a predecesor and practise exercise for the hopefully soon-to-come tyrbal programing language compiler. either way when the user has input their expression it will then be written to ./generated/1_expression.txt relative to the run.sh or calc_gui.py script, I'm sure there's a better way to store intermediate expressions, but I found writing it to a file to be easy to work with both when it comes to debugging and ease of programming, I also find it cool to see the expression in all the different stages of compiling so I don't think I'll change this any time in the near future.

the second step is done by the PEMDAS-parsing.c program (PEMDAS meaning: "Parenthesis, Exponent, Multiplication, Division, Addition, Subtraction"), since my job with the tyrbal compiler isn't the initial parsing and AST (Abstract Syntax Tree) generation, so I had AI help me a bit on this one. the way it works however is by reading the ./generated/1_expression.txt file and loading it into a buffer character by character (it has a buffer overflow vulnerability btw since the allocated buffer size is currently 100 bytes and it continues to read and add to the buffer even after the allocated size is succeeded, though this can easily be fixed it is not my priority to do so and there are many more vulnerabilites throughout the compiler that I just won't mention) the expression buffer is then first parsed to see if there are any instructions in front of a parenthesis or not like the "+" in this expression: "1337+(4*4)" if there isn't it will place a multiplication sign since determining what operation to do will be a lot easier this way since parenthesis get removed before the end of the compilation anyways. then the abstract syntax is parsed and generated into a new buffer that is written to a new file ./generated/2_ast_output.txt the abstract syntax is the expression but each induvidual operation is wrapped in parenthesis, the innermost parenthesis being what is done first according to the PEMDAS order of operations.

the third step is the intermed-codegen.c program that decides breaks up the expression up into temporaries, say we have a Pemdas parsed expression like so: ((a+b)*(c-d)) would be broken into temporary single operation expression in their correct order of operations from the innermost-left to the outer-right like so t1=a+b t2=c-d t3=t1*t2 which can easily be translated into ASM operations by treating the temporaries as the contents of registers, which brings us to the last step.

last but not least, my favorite program intermed-to-asm.c reads the intermediate code file that contains temporary variable assignments such as t1 = (a+b) and turns each of those lines into real x86-64 assembly instructions. it starts by writing a small header to define the data and text sections of the assembly program. then it reads each line of the intermediate code, which always has a result variable on the left side and an arithmetic expression inside parentheses on the right. it breaks that line apart to figure out which variables or temporaries are being used and what operator appears between them. it keeps track of which temporary variables are currently stored in which cpu registers from r8 to r15. when both operands are temporaries, it finds their registers and performs the operation directly between them, then marks the resulting temporary as being stored in that same register. when one operand is a temporary and the other is just a number or variable, it loads the non-temporary value into the register that already holds the temporary and performs the operation there. when neither operand is temporary, it looks for a free register, loads the first value into it, performs the operation with the second value, and records that the result temporary now lives in that register. each operation like addition, subtraction, multiplication, or division is translated into the corresponding assembly instruction. after all expressions are processed, the program appends assembly code that converts the final value in r8 into text digits, prints it to the screen, and then exits the program. overall the program’s job is to translate a simplified intermediate representation of arithmetic into actual assembly instructions that perform the computation and show the result.

## Building and Using:

requirements:
- x86-64 processor (the math is compiled into x86-64 ASM)
- Linux based OS   (I used Linux syscalls)
- a C compiler
- nasm & ld
- python (if you want to use the GUI)


#### First of all you have to compile the C files, in this example I'm using GNU's C Compiler:

gcc PEMDAS-parsing.c -o PEMDAS-parsing

gcc intermed-codegen.c -o intermed-codegen

gcc intermed-to-asm.c -o intermed-to-asm


#### Then give executable file permission to the run.sh script:

chmod +x run.sh

#### to run it you simply need to run the run script:
./run.sh








