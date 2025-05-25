#ifndef PARSER_H
#define PARSER_H

#include "../../ast.h"
#include "../../symbol_table.h"
#include "../../tokens.h"

struct Stack {
    char items[100];
    int top;
};

void nextToken();
void match(TokenType expected);
void push(struct Stack *stack, char item);
char pop(struct Stack *stack);
int isEmpty(struct Stack *stack);
ASTNode* factor();
ASTNode* term();
ASTNode* parseExpression(int minPrecedence);
ASTNode* conditional();
ASTNode* functionDef();
ASTNode* functionCall();
void parseTokens();

#endif // PARSER_H
