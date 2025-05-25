%{
#include <stdio.h>
FILE *fp;
extern FILE *yyin;
%}
%%
"/*"[^*]*"*/"
"#".* { fprintf(fp, "\n"); }
[ \t]+ { fprintf(fp, "%s", yytext); }
.|\n { fprintf(fp, "%s", yytext); }
%%
int main() {
 yyin = fopen("input.c", "r");
 if (!yyin) {
 perror("Error opening input.c");
 return 1;
 }
 fp = fopen("out.c", "w");
 if (!fp) {
 perror("Error opening out.c");
 return 1;
 }
yylex();
 fclose(yyin);
 fclose(fp);
 return 0;
}
int yywrap() { return 1; }