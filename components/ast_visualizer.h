#ifndef AST_VISUALIZER_H
#define AST_VISUALIZER_H

#include "ast.h"

// Function to visualize the AST
void visualizeAST(ASTNode* node, int depth);

// Wrapper function to reset visited nodes before visualization
void visualizeASTRoot(ASTNode* node);

#endif // AST_VISUALIZER_H
