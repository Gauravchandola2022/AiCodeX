%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../components/tokens.h"
#include "../components/symbol_table.h" 
extern void addToken(TokenType type, const char* value);
extern Token* head;
extern Token* current;

#define yylex tokenizer

int inVarDeclaration = 0;
%}

DIGIT       [0-9]+
VAR         "num"|"log"|"str"
ASSIGN      "="
OPERATOR    [+\-*/]
COMMENT     "#"[^\\n]*
SEMICOLON   ";"
LBRACE      "{"
RBRACE      "}"
LPAREN      "\("
RPAREN      "\)"
IF          "if"
ELSE        "else"
WHILE       "while"
FUNC        "func"
CALL        "call"
COMMA       ","
WHITESPACE  [ \t\n\r]+
FOR         "for"
DO          "do"
AND         "&&"
OR          "||"
ID          [a-zA-Z_][a-zA-Z0-9_]*
RELOP       "=="|"!="|"<"|">"|"<="|">="
STRING      \"[^\"]*\"
BOOLEAN     "true"|"false"

%%
{VAR}       { 
                if (lookupSymbol(yytext) != -1) {
                    printf("Error: Keyword '%s' is reserved and cannot be redefined\n", yytext);
                    exit(1);
                }
                addToken(VAR, yytext); 
                printf("TOKEN: KEYWORD, VALUE: %s\n", yytext); 
            }  
{STRING}    {
                char value[100];
                strncpy(value, yytext + 1, strlen(yytext) - 2);
                value[strlen(yytext) - 2] = '\0';
                addToken(STRING_LITERAL, value);
                printf("TOKEN: STRING_LITERAL, VALUE: %s\n", value);
            }
{ID}        {
                if (strcmp(yytext, "num") == 0 || 
                    strcmp(yytext, "log") == 0 || 
                    strcmp(yytext, "str") == 0) {
                    addToken(VAR, yytext);
                    printf("TOKEN: KEYWORD, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "if") == 0) {
                    addToken(IF, yytext);
                    printf("TOKEN: IF, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "else") == 0) {
                    addToken(ELSE, yytext);
                    printf("TOKEN: ELSE, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "while") == 0) {
                    addToken(WHILE, yytext);
                    printf("TOKEN: WHILE, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "for") == 0) {
                    addToken(FOR, yytext);
                    printf("TOKEN: FOR, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "do") == 0) {
                    addToken(DO, yytext);
                    printf("TOKEN: DO, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "func") == 0) {
                    addToken(FUNC, yytext);
                    printf("TOKEN: FUNC, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "call") == 0) {
                    addToken(CALL, yytext);
                    printf("TOKEN: CALL, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "print") == 0) {
                    addToken(PRINT, yytext);
                    printf("TOKEN: PRINT, VALUE: %s\n", yytext);
                }
                else if (strcmp(yytext, "true") == 0 || strcmp(yytext, "false") == 0) {
                    addToken(BOOLEAN_LITERAL, yytext);
                    printf("TOKEN: BOOLEAN_LITERAL, VALUE: %s\n", yytext);
                }
                else {
                    addToken(ID, yytext);
                    printf("TOKEN: ID, VALUE: %s\n", yytext);
                }
            }
{DIGIT}         { addToken(NUMBER, yytext); printf("TOKEN: NUMBER, VALUE: %s\n", yytext); }  
{ASSIGN}        { addToken(ASSIGN, yytext); printf("TOKEN: ASSIGN, VALUE: %s\n", yytext); }  
{OPERATOR}      { addToken(OPERATOR, yytext); printf("TOKEN: OPERATOR, VALUE: %s\n", yytext); }  
{SEMICOLON}     { addToken(SEMICOLON, yytext); printf("TOKEN: SEMICOLON, VALUE: %s\n", yytext); }  
{COMMA}         { addToken(COMMA, yytext); printf("TOKEN: COMMA, VALUE: %s\n", yytext); }  

{LBRACE}        { addToken(LBRACE, yytext); printf("TOKEN: LBRACE, VALUE: %s\n", yytext); }
{RBRACE}        { addToken(RBRACE, yytext); printf("TOKEN: RBRACE, VALUE: %s\n", yytext); }
{LPAREN}        { addToken(LPAREN, yytext); printf("TOKEN: LPAREN, VALUE: %s\n", yytext); }
{RPAREN}        { addToken(RPAREN, yytext); printf("TOKEN: RPAREN, VALUE: %s\n", yytext); }

{RELOP}           { addToken(RELOP, yytext); printf("TOKEN: RELOP, VALUE: %s\n", yytext); }  
{AND}           { addToken(AND, yytext); printf("TOKEN: AND, VALUE: %s\n", yytext); }  
{OR}           { addToken(OR, yytext); printf("TOKEN: OR, VALUE: %s\n", yytext); }  
{COMMENT}       { printf("Skipping comment: %s\n", yytext); }  

{WHITESPACE}    { /* Do nothing */ }  
. { 
    printf("ERROR: Unrecognized character '%s'\n", yytext); 
    exit(0); 
}  

%%

int yywrap() { return 1; }
