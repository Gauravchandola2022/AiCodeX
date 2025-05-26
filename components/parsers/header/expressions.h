#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "../../ast.h"

ASTNode* factor();
ASTNode* primary();
ASTNode* term();
ASTNode* multiplicative();
ASTNode* additive();
ASTNode* expression();
ASTNode* parseExpression(int minPrecedence);
ASTNode* parseCondition(int minPrecedence);
ASTNode* comparison();
ASTNode* logical();
int getPrecedence(char op); 
int isRightAssociative(char op);
int isLogicalOp(const char *op);
int isRelationalOp(const char *op);
int isComparisonOperator(const char *value);
int evaluateExpression(ASTNode *expr);
ASTNode* functionCall(const char* name);

#endif

