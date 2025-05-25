#ifndef PARSER_FUNCTIONS_H
#define PARSER_FUNCTIONS_H

#include "../../tokens.h"
#include "../../ast.h"

ASTNode* statement();
ASTNode* parseCondition(int minPrecedence);
ASTNode* parseExpression(int minPrecedence);
const char* tokenTypeToString(TokenType type);

#endif // PARSER_FUNCTIONS_H
