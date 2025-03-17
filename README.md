# arithmetic-compiler
input a math expression and compile it into a binary that calculates and outputs the answer

it has four main steps:

1. writing the expression
2. parsing the expression to ast following the PEMDAS order of operations
3. generating intermediate code from the ast
4. generating ASM from intermediate code

then the ASM is assembled into a binary that is run, but that's just using nasm and ld

##How it works and some trivia:

the first step is done using the run.sh/calc_gui script depending on if you want to write the expression in the CLI or if you want a calculator GUI interface, not too much effort is put into this since the point of this project isn't to provide good UX, rather it's just a predecesor and practise exercise for the hopefully soon-to-come tyrbal programing language compiler. either way when the user has input their expression it will then be written to ./generated/1_expression.txt relative to the run.sh or calc_gui.py script, I'm sure there's a better way to store intermediate expressions, but I found writing it to a file to be easy to work with both when it comes to debugging and ease of programming, I also find it cool to see the expression in all the different stages of compiling so I don't think I'll change this any time in the near future.

the second step is done by the PEMDAS-parsing.c program (previously called OOP-parsing.c OOP in this context standing for "Order of OPerations", but then I remembered that it mostly stands for "Object Oriented Programing" so I renamed it to PEMDAS: "Parenthesis, Exponent, Multiplication, Division, Addition, Subtraction", which in my opinion sounds a lot more lame, but at least it's not as confusing), since my job with the tyrbal compiler isn't the initial parsing and AST (Abstract Syntax Tree) generation, I didn't put a whole bunch of effort into this either and had chatgpt do most of the job. the way it works however is by reading the ./generated/1_expression.txt file and loading it into a buffer character by character (it has a buffer overflow vulnerability btw since the allocated buffer size is currently 100 bytes and it continues to read and add to the buffer even after the allocated size is succeeded, though this can easily be fixed it is not my priority to do so and there are many more vulnerabilites throughout the compiler that I just won't mention) the expression buffer is then first parsed to see if there are any instructions in front of a parenthesis or not like the "+" in this expression: "1337+(4*4)" if there isn't it will place a multiplication sign since determining what operation to do will be a lot easier this way since parenthesis get removed before the end of the compilation anyways. then the abstract syntax is parsed and generated into a new buffer that is written to a new file ./generated/2_ast_output.txt the abstract syntax is the expression but each induvidual operation is wrapped in parenthesis, the innermost parenthesis being what is done first according to the PEMDAS order of operations.

the third step is the intermed-codegen.c program that does, what it does is 





