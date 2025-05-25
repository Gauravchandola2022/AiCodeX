#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "components/tokens.h"
#include "components/ast.h"

void tokenize(FILE *file);
ASTNode* statement();
void nextToken();
ASTNode* parseExpression(int minPrecedence);
ASTNode* conditional();
ASTNode* loop();
ASTNode* doWhileLoop();
ASTNode* forLoop();
ASTNode* functionDef();

#endif // PARSER_H
