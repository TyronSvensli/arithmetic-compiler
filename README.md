# arithmetic-compiler
input a math expression compile it into a binary that calculates and outputs the answer

it has four main steps:

1. writing the expression
2. parsing the expression to ast following the PEMDAS order of operations
3. generating intermediate code from the ast
4. generating ASM from intermediate code

then the ASM is assembled into a binary that is run, but that's just using nasm and ld

the first step is done using the run.sh/calc_gui script depending on if you want to write the expression in the CLI or if you want a calculator GUI interface, not too much effort is put into this since the point of this project isn't to provide good UX, rather it's just a predecesor and practise exercise for the hopefully soon-to-come tyrbal programing language compiler. either way when the user has input their expression it will then be written to ./generated/1_expression.txt relative to the run.sh or calc_gui.py script, I'm sure there's a better way to store intermediate expressions, but I found writing it to a file to be easy to work with both when it comes to debugging and ease of programming, I also find it cool to see the expression in all the different stages of compiling so I don't think I'll change this any time in the near future.

the second step is done by the PEMDAS-parsing.c program (previously called OOP-parsing.c OOP in this context standing for "Order of OPerations", but then I remembered that it mostly stands for "Object Oriented Programing" so I renamed it to PEMDAS: "Parenthesis, Exponent, Multiplication, Division, Addition, Subtraction", which in my opinion sounds a lot more lame, but at least it's not as confusing), since my job with the tyrbal compiler isn't the initial parsing and AST (Abstract Syntax Tree) generation, I didn't put a whole bunch of effort into this either and had chatgpt do most of the job.





