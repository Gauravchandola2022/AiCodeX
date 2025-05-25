#include "ast_visualizer.h"
#include <stdio.h>
#include <stdlib.h>

// Helper function to print indentation
void printIndent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

// Keep track of visited nodes to prevent infinite recursion
#define MAX_VISITED_NODES 1000
void* visitedNodes[MAX_VISITED_NODES];
int visitedCount = 0;

// Check if a node has been visited
int hasBeenVisited(void* node) {
    for (int i = 0; i < visitedCount; i++) {
        if (visitedNodes[i] == node) {
            return 1;
        }
    }
    return 0;
}

// Add a node to the visited list
void markVisited(void* node) {
    if (visitedCount < MAX_VISITED_NODES) {
        visitedNodes[visitedCount++] = node;
    }
}

// Reset the visited nodes list
void resetVisited() {
    visitedCount = 0;
}

// Function to visualize the AST
void visualizeAST(ASTNode* node, int depth) {
    if (!node) return;
    
    // Check if this node has been visited before
    if (hasBeenVisited(node)) {
        printIndent(depth);
        printf("CIRCULAR REFERENCE DETECTED\n");
        return;
    }
    
    // Mark this node as visited
    markVisited(node);
    
    printIndent(depth);
    
    switch (node->type) {
        case NODE_NUMBER:
            printf("NUMBER: %d\n", node->number);
            break;
            
        case NODE_BINARY_OP:
            printf("BINARY_OP: %c\n", node->binaryOp.op);
            printIndent(depth);
            printf("LEFT:\n");
            visualizeAST(node->binaryOp.left, depth + 1);
            printIndent(depth);
            printf("RIGHT:\n");
            visualizeAST(node->binaryOp.right, depth + 1);
            break;
            
        case NODE_ASSIGN:
            printf("ASSIGN: %s\n", node->assign.name);
            printIndent(depth);
            printf("EXPR:\n");
            visualizeAST(node->assign.expr, depth + 1);
            break;
            
        case NODE_VAR_DECL:
            printf("VAR_DECL: %s\n", node->varDecl.name);
            printIndent(depth);
            printf("VALUE:\n");
            visualizeAST(node->varDecl.value, depth + 1);
            break;
            
        case NODE_IF:
            printf("IF\n");
            printIndent(depth);
            printf("CONDITION:\n");
            visualizeAST(node->ifNode.condition, depth + 1);
            printIndent(depth);
            printf("THEN:\n");
            visualizeAST(node->ifNode.thenStmt, depth + 1);
            if (node->ifNode.elseStmt) {
                printIndent(depth);
                printf("ELSE:\n");
                visualizeAST(node->ifNode.elseStmt, depth + 1);
            }
            break;
            
        case NODE_WHILE:
            printf("WHILE\n");
            printIndent(depth);
            printf("CONDITION:\n");
            visualizeAST(node->whileNode.condition, depth + 1);
            printIndent(depth);
            printf("BODY:\n");
            visualizeAST(node->whileNode.body, depth + 1);
            break;
            
        case NODE_DO_WHILE:
            printf("DO_WHILE\n");
            printIndent(depth);
            printf("BODY:\n");
            visualizeAST(node->doWhileNode.body, depth + 1);
            printIndent(depth);
            printf("CONDITION:\n");
            visualizeAST(node->doWhileNode.condition, depth + 1);
            break;
            
        case NODE_FOR:
            printf("FOR\n");
            printIndent(depth);
            printf("INIT:\n");
            visualizeAST(node->forNode.initialization, depth + 1);
            printIndent(depth);
            printf("CONDITION:\n");
            visualizeAST(node->forNode.condition, depth + 1);
            printIndent(depth);
            printf("INCREMENT:\n");
            visualizeAST(node->forNode.increment, depth + 1);
            printIndent(depth);
            printf("BODY:\n");
            visualizeAST(node->forNode.body, depth + 1);
            break;
            
        case NODE_FUNC_DEF:
            printf("FUNC_DEF: %s\n", node->funcDef.name);
            printIndent(depth);
            printf("PARAMS:\n");
            visualizeAST(node->funcDef.params, depth + 1);
            printIndent(depth);
            printf("BODY:\n");
            visualizeAST(node->funcDef.body, depth + 1);
            break;
            
        case NODE_FUNC_CALL:
            printf("FUNC_CALL: %s\n", node->funcCall.name);
            printIndent(depth);
            printf("ARGS:\n");
            visualizeAST(node->funcCall.args, depth + 1);
            break;
            
        case NODE_LOGICAL_OP:
            printf("LOGICAL_OP: %s\n", node->logicalOp.op);
            printIndent(depth);
            printf("LEFT:\n");
            visualizeAST(node->logicalOp.left, depth + 1);
            printIndent(depth);
            printf("RIGHT:\n");
            visualizeAST(node->logicalOp.right, depth + 1);
            break;
            
        case NODE_RELATIONAL_OP:
            printf("RELATIONAL_OP: %s\n", node->relOp.op);
            printIndent(depth);
            printf("LEFT:\n");
            visualizeAST(node->relOp.left, depth + 1);
            printIndent(depth);
            printf("RIGHT:\n");
            visualizeAST(node->relOp.right, depth + 1);
            break;
            
        case NODE_COMPARISON_OP:
            printf("COMPARISON_OP: %s\n", node->compOp.op);
            printIndent(depth);
            printf("LEFT:\n");
            visualizeAST(node->compOp.left, depth + 1);
            printIndent(depth);
            printf("RIGHT:\n");
            visualizeAST(node->compOp.right, depth + 1);
            break;
            
        case NODE_PRINT:
            printf("PRINT\n");
            printIndent(depth);
            printf("EXPR:\n");
            visualizeAST(node->print.expr, depth + 1);
            break;
            
        default:
            printf("UNKNOWN NODE TYPE: %d\n", node->type);
            break;
    }
    
    // Visualize the next node in the list
    if (node->next) {
        printIndent(depth - 1);
        printf("NEXT:\n");
        visualizeAST(node->next, depth);
    }
}

// Wrapper function to reset visited nodes before visualization
void visualizeASTRoot(ASTNode* node) {
    resetVisited();
    visualizeAST(node, 0);
}

