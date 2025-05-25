#include "ast_json_exporter.h"
#include <stdio.h>
#include <stdlib.h>

// Keep track of visited nodes to prevent infinite recursion
#define MAX_VISITED_NODES 1000
void* jsonVisitedNodes[MAX_VISITED_NODES];
int jsonVisitedCount = 0;

// Node ID mapping
typedef struct {
    void* node;
    int id;
} NodeIDMapping;

#define MAX_NODE_MAPPINGS 1000
NodeIDMapping nodeIDMappings[MAX_NODE_MAPPINGS];
int nodeIDCount = 0;

// Check if a node has been visited
int jsonHasBeenVisited(void* node) {
    for (int i = 0; i < jsonVisitedCount; i++) {
        if (jsonVisitedNodes[i] == node) {
            return 1;
        }
    }
    return 0;
}

// Add a node to the visited list
void jsonMarkVisited(void* node) {
    if (jsonVisitedCount < MAX_VISITED_NODES) {
        jsonVisitedNodes[jsonVisitedCount++] = node;
    }
}

// Reset the visited nodes list
void jsonResetVisited() {
    jsonVisitedCount = 0;
}

// Get node ID or assign a new one
int getNodeID(void* node) {
    for (int i = 0; i < nodeIDCount; i++) {
        if (nodeIDMappings[i].node == node) {
            return nodeIDMappings[i].id;
        }
    }
    
    // Node not found, assign a new ID
    if (nodeIDCount < MAX_NODE_MAPPINGS) {
        nodeIDMappings[nodeIDCount].node = node;
        nodeIDMappings[nodeIDCount].id = nodeIDCount;
        return nodeIDCount++;
    }
    
    // Too many nodes, return -1 as error
    return -1;
}

// Reset the node ID mappings
void resetNodeIDs() {
    nodeIDCount = 0;
}

// Reset the JSON exporter state
void resetJSONExporter() {
    jsonResetVisited();
    resetNodeIDs();
}

// Helper function to write AST node to JSON
void writeNodeToJSON(ASTNode* node, FILE* file, int isLast) {
    if (!node) {
        fprintf(file, "null");
        return;
    }

    int nodeID = getNodeID(node);

    // Check if this node has been visited before
    if (jsonHasBeenVisited(node)) {
        fprintf(file, "{ \"type\": \"REFERENCE\", \"id\": %d }", nodeID);
        return;
    }

    // Mark this node as visited
    jsonMarkVisited(node);

    fprintf(file, "{\n");
    fprintf(file, "  \"id\": %d,\n", nodeID);

    switch (node->type) {
        case NODE_NUMBER:
            fprintf(file, "  \"type\": \"NUMBER\",\n");
            fprintf(file, "  \"value\": %d", node->number);
            break;

        case NODE_BINARY_OP:
            fprintf(file, "  \"type\": \"BINARY_OP\",\n");
            fprintf(file, "  \"operator\": \"%c\",\n", node->binaryOp.op);
            fprintf(file, "  \"left\": ");
            writeNodeToJSON(node->binaryOp.left, file, 0);
            // Removed explicit comma here to avoid double commas
            // fprintf(file, ",\n");
            fprintf(file, "\n");
            fprintf(file, "  \"right\": ");
            writeNodeToJSON(node->binaryOp.right, file, 1);
            break;

        case NODE_ASSIGN:
            fprintf(file, "  \"type\": \"ASSIGN\",\n");
            fprintf(file, "  \"name\": \"%s\",\n", node->assign.name);
            fprintf(file, "  \"expr\": ");
            writeNodeToJSON(node->assign.expr, file, 1);
            break;

        case NODE_VAR_DECL:
            fprintf(file, "  \"type\": \"VAR_DECL\",\n");
            fprintf(file, "  \"name\": \"%s\",\n", node->varDecl.name);
            fprintf(file, "  \"value\": ");
            writeNodeToJSON(node->varDecl.value, file, 1);
            break;

        case NODE_IF:
            fprintf(file, "  \"type\": \"IF\",\n");
            fprintf(file, "  \"condition\": ");
            writeNodeToJSON(node->ifNode.condition, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"then\": ");
            writeNodeToJSON(node->ifNode.thenStmt, file, node->ifNode.elseStmt == NULL);
            if (node->ifNode.elseStmt) {
                fprintf(file, ",\n");
                fprintf(file, "  \"else\": ");
                writeNodeToJSON(node->ifNode.elseStmt, file, 1);
            }
            break;

        case NODE_WHILE:
            fprintf(file, "  \"type\": \"WHILE\",\n");
            fprintf(file, "  \"condition\": ");
            writeNodeToJSON(node->whileNode.condition, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"body\": ");
            writeNodeToJSON(node->whileNode.body, file, 1);
            break;

        case NODE_DO_WHILE:
            fprintf(file, "  \"type\": \"DO_WHILE\",\n");
            fprintf(file, "  \"body\": ");
            writeNodeToJSON(node->doWhileNode.body, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"condition\": ");
            writeNodeToJSON(node->doWhileNode.condition, file, 1);
            break;

        case NODE_FOR:
            fprintf(file, "  \"type\": \"FOR\",\n");
            fprintf(file, "  \"init\": ");
            writeNodeToJSON(node->forNode.initialization, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"condition\": ");
            writeNodeToJSON(node->forNode.condition, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"increment\": ");
            writeNodeToJSON(node->forNode.increment, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"body\": ");
            writeNodeToJSON(node->forNode.body, file, 1);
            break;

        case NODE_FUNC_DEF:
            fprintf(file, "  \"type\": \"FUNC_DEF\",\n");
            fprintf(file, "  \"name\": \"%s\",\n", node->funcDef.name);
            fprintf(file, "  \"params\": ");
            writeNodeToJSON(node->funcDef.params, file, 0);
            fprintf(file, ",\n");
            fprintf(file, "  \"body\": ");
            writeNodeToJSON(node->funcDef.body, file, 1);
            break;

        case NODE_FUNC_CALL:
            fprintf(file, "  \"type\": \"FUNC_CALL\",\n");
            fprintf(file, "  \"name\": \"%s\",\n", node->funcCall.name);
            fprintf(file, "  \"args\": ");
            writeNodeToJSON(node->funcCall.args, file, 1);
            break;

        case NODE_LOGICAL_OP:
            fprintf(file, "  \"type\": \"LOGICAL_OP\",\n");
            fprintf(file, "  \"operator\": \"%s\",\n", node->logicalOp.op);
            fprintf(file, "  \"left\": ");
            writeNodeToJSON(node->logicalOp.left, file, 0);
            // Removed explicit comma here to avoid double commas
            // fprintf(file, ",\n");
            fprintf(file, "\n");
            fprintf(file, "  \"right\": ");
            writeNodeToJSON(node->logicalOp.right, file, 1);
            break;

        case NODE_RELATIONAL_OP:
            fprintf(file, "  \"type\": \"RELATIONAL_OP\",\n");
            fprintf(file, "  \"operator\": \"%s\",\n", node->relOp.op);
            fprintf(file, "  \"left\": ");
            writeNodeToJSON(node->relOp.left, file, 0);
            // Removed explicit comma here to avoid double commas
            // fprintf(file, ",\n");
            fprintf(file, "\n");
            fprintf(file, "  \"right\": ");
            writeNodeToJSON(node->relOp.right, file, 1);
            break;

        case NODE_COMPARISON_OP:
            fprintf(file, "  \"type\": \"COMPARISON_OP\",\n");
            fprintf(file, "  \"operator\": \"%s\",\n", node->compOp.op);
            fprintf(file, "  \"left\": ");
            writeNodeToJSON(node->compOp.left, file, 0);
            // Removed explicit comma here to avoid double commas
            // fprintf(file, ",\n");
            fprintf(file, "\n");
            fprintf(file, "  \"right\": ");
            writeNodeToJSON(node->compOp.right, file, 1);
            break;

        case NODE_PRINT:
            fprintf(file, "  \"type\": \"PRINT\",\n");
            fprintf(file, "  \"expr\": ");
            writeNodeToJSON(node->print.expr, file, 1);
            break;

        case NODE_STRING_LITERAL:
            fprintf(file, "  \"type\": \"STRING_LITERAL\",\n");
            fprintf(file, "  \"value\": \"%s\"", node->stringLiteral.value);
            break;

        case NODE_BOOLEAN_LITERAL:
            fprintf(file, "  \"type\": \"BOOLEAN_LITERAL\",\n");
            fprintf(file, "  \"value\": \"%s\"", node->booleanLiteral.value);
            break;

        case NODE_VAR_REF:
            fprintf(file, "  \"type\": \"VAR_REF\",\n");
            fprintf(file, "  \"name\": \"%s\"", node->varRef.name);
            break;

        case NODE_RETURN:
            fprintf(file, "  \"type\": \"RETURN\",\n");
            fprintf(file, "  \"expr\": ");
            writeNodeToJSON(node->assign.expr, file, 1);
            break;

        case NODE_UNKNOWN:
            fprintf(file, "  \"type\": \"UNKNOWN\"");
            break;
        default:
            fprintf(file, "  \"type\": \"UNKNOWN\",\n");
            fprintf(file, "  \"typeValue\": %d", node->type);
            break;
    }

    // Add next node if it exists
    if (node->next) {
        fprintf(file, ",\n");
        fprintf(file, "  \"next\": ");
        // Pass isLast as 1 only if current node is last, else 0
        writeNodeToJSON(node->next, file, isLast ? 1 : 0);
    }

    fprintf(file, "\n}");
    if (!isLast) {
        fprintf(file, ",");
    }
}

// Function to export AST to JSON
void exportASTToJSON(ASTNode* node, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return;
    }
    
    fprintf(file, "{\n");
    fprintf(file, "  \"ast\": ");
    
    jsonResetVisited();
    writeNodeToJSON(node, file, 1);
    
    fprintf(file, "\n}\n");
    
    fclose(file);
    printf("AST exported to %s\n", filename);
}

// Function to export multiple AST nodes to a single JSON file
void exportASTsToSingleJSON(ASTNode** nodes, int count, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return;
    }
    
    fprintf(file, "{\n");
    fprintf(file, "  \"statements\": [\n");
    
    resetJSONExporter();
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "    {\n");
        fprintf(file, "      \"id\": %d,\n", i);
        fprintf(file, "      \"ast\": ");
        
        jsonResetVisited();
        writeNodeToJSON(nodes[i], file, 1);
        
        fprintf(file, "\n    }");
        if (i < count - 1) {
            fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");

    fclose(file);
    printf("All ASTs exported to %s\n", filename);
}

