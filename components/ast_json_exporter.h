#ifndef AST_JSON_EXPORTER_H
#define AST_JSON_EXPORTER_H

#include "ast.h"

// Function to export AST to JSON
void exportASTToJSON(ASTNode* node, const char* filename);

// Function to export multiple AST nodes to a single JSON file
void exportASTsToSingleJSON(ASTNode** nodes, int count, const char* filename);

// Function to reset the JSON exporter state between compilations
void resetJSONExporter();

#endif // AST_JSON_EXPORTER_H
