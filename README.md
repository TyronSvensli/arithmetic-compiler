# arithmetic-compiler
input a math expression compile it into a binary that calculates and outputs the answer

it has four main steps:

1. writing the expression
2. parsing the expression to ast following the PEMDAS order of operations
3. generating intermediate code from the ast
4. generating ASM from intermediate code

then the ASM is assembled into a binary that is run


