#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Token types
typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    double value; // Only used for numbers
} Token;

// AST Node Types
typedef enum {
    AST_NUMBER,
    AST_BINARY_OP
} ASTNodeType;

// AST Node Structure
typedef struct ASTNode {
    ASTNodeType type;
    union {
        double value; // for numbers
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
            TokenType op;
        } binaryOp; // for binary operations
    };
} ASTNode;

// Global variables for tokens and input
const char *input;
Token currentToken;

// Function prototypes
Token getNextToken();
void match(TokenType expected);
ASTNode* parseExpr();
ASTNode* parseTerm();
ASTNode* parseFactor();
void printASTToFile(ASTNode *node, FILE *file);

// Lexer: Get the next token from input
Token getNextToken() {
    while (*input == ' ') input++; // Skip whitespace

    if (isdigit(*input) || *input == '.') {
        // Parse a number
        char *endPtr;
        double value = strtod(input, &endPtr);
        input = endPtr;
        return (Token){TOKEN_NUMBER, value};
    }

    char ch = *input++;
    switch (ch) {
        case '+': return (Token){TOKEN_PLUS, 0};
        case '-': return (Token){TOKEN_MINUS, 0};
        case '*': return (Token){TOKEN_MUL, 0};
        case '/': return (Token){TOKEN_DIV, 0};
        case '(': return (Token){TOKEN_LPAREN, 0};
        case ')': return (Token){TOKEN_RPAREN, 0};
        case '\0': return (Token){TOKEN_EOF, 0};
        default:
            fprintf(stderr, "Unexpected character: %c\n", ch);
            exit(1);
    }
}

// Match the current token and consume it
void match(TokenType expected) {
    if (currentToken.type == expected) {
        currentToken = getNextToken();
    } else {
        fprintf(stderr, "Syntax error: Expected %d, got %d\n", expected, currentToken.type);
        exit(1);
    }
}

// Parse an expression (handles '+' and '-')
ASTNode* parseExpr() {
    ASTNode *node = parseTerm(); // Parse the first term

    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType op = currentToken.type;
        match(op);

        ASTNode *newNode = malloc(sizeof(ASTNode));
        newNode->type = AST_BINARY_OP;
        newNode->binaryOp.op = op;
        newNode->binaryOp.left = node;
        newNode->binaryOp.right = parseTerm();
        node = newNode;
    }
    return node;
}

// Parse a term (handles '*' and '/')
ASTNode* parseTerm() {
    ASTNode *node = parseFactor(); // Parse the first factor

    while (currentToken.type == TOKEN_MUL || currentToken.type == TOKEN_DIV) {
        TokenType op = currentToken.type;
        match(op);

        ASTNode *newNode = malloc(sizeof(ASTNode));
        newNode->type = AST_BINARY_OP;
        newNode->binaryOp.op = op;
        newNode->binaryOp.left = node;
        newNode->binaryOp.right = parseFactor();
        node = newNode;
    }
    return node;
}

// Parse a factor (handles numbers and parentheses)
ASTNode* parseFactor() {
    if (currentToken.type == TOKEN_LPAREN) {
        match(TOKEN_LPAREN);
        ASTNode *node = parseExpr();
        match(TOKEN_RPAREN);
        return node;
    } else if (currentToken.type == TOKEN_NUMBER) {
        ASTNode *node = malloc(sizeof(ASTNode));
        node->type = AST_NUMBER;
        node->value = currentToken.value;
        match(TOKEN_NUMBER);
        return node;
    } else {
        fprintf(stderr, "Syntax error: Unexpected token\n");
        exit(1);
    }
}

// Print the AST to a file (formatted to avoid trailing .000000)
void printASTToFile(ASTNode *node, FILE *file) {
    if (node->type == AST_NUMBER) {
        if (node->value == (int)node->value) {
            fprintf(file, "%d", (int)node->value);
        } else {
            fprintf(file, "%g", node->value);
        }
    } else if (node->type == AST_BINARY_OP) {
        fprintf(file, "(");
        printASTToFile(node->binaryOp.left, file);
        fprintf(file, " %c ", node->binaryOp.op == TOKEN_PLUS ? '+' : 
                              node->binaryOp.op == TOKEN_MINUS ? '-' :
                              node->binaryOp.op == TOKEN_MUL ? '*' : '/');
        printASTToFile(node->binaryOp.right, file);
        fprintf(file, ")");
    }
}

char* insert_missing_multiplication(const char* expr) {
    int len = strlen(expr);
    char* result = malloc(len * 2 + 1);  // Allocate enough space for new characters
    if (!result) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    int j = 0;
    for (int i = 0; i < len; i++) {
        result[j++] = expr[i];

        // If a digit is followed by '(', insert '*'
        if (isdigit(expr[i]) && expr[i + 1] == '(') {
            result[j++] = '*';
        }
    }
    result[j] = '\0';  // Null-terminate the string
    return result;
}

// Main function to parse and output the AST
int main() {
    FILE *r_file;
    r_file = fopen("generated/1_expression.txt", "r");

    char expression[100];
    int idx = 0;
    char ch;
    while ((ch = fgetc(r_file)) != EOF) {
        if (ch != '\n') {
            expression[idx++] = ch;
        }
    }
    expression[idx] = '\0'; // Ensure null termination
    fclose(r_file);
    
    input = insert_missing_multiplication(expression);
    currentToken = getNextToken(); // Initialize the first token

    ASTNode *ast = parseExpr();

    FILE *w_file = fopen("generated/2_ast_output.txt", "w");
    printASTToFile(ast, w_file);
    fclose(w_file);
    
    return 0;
}
