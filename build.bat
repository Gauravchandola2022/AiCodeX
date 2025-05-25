@echo off
echo Compiling CompilerX...

REM Run flex to generate lex.yy.c
cd utils
flex lexer.l
cd ..

REM Create directories if they don't exist
mkdir obj 2>nul
mkdir obj\components 2>nul
mkdir obj\components\parsers 2>nul
mkdir obj\components\generator 2>nul
mkdir obj\utils 2>nul

REM Compile individual source files
gcc -c components/tokens.c -o obj/components/tokens.o
gcc -c utils/lex.yy.c -o obj/utils/lex.yy.o
gcc -c compiler.c -o obj/compiler.o
gcc -c components/memory.c -o obj/components/memory.o
gcc -c components/symbol_table.c -o obj/components/symbol_table.o
gcc -c components/ast_visualizer.c -o obj/components/ast_visualizer.o
gcc -c components/ast_json_exporter.c -o obj/components/ast_json_exporter.o
gcc -c components/parsers/parser.c -o obj/components/parsers/parser.o
gcc -c components/parsers/expressions.c -o obj/components/parsers/expressions.o
gcc -c components/parsers/statements.c -o obj/components/parsers/statements.o
gcc -c components/parsers/conditionals.c -o obj/components/parsers/conditionals.o
gcc -c components/parsers/functions.c -o obj/components/parsers/functions.o
gcc -c components/parsers/loops.c -o obj/components/parsers/loops.o
gcc -c components/generator/codegen.c -o obj/components/generator/codegen.o
gcc -c semantic.c -o obj/semantic.o

REM Link all object files
gcc obj/compiler.o obj/components/memory.o obj/components/symbol_table.o obj/components/tokens.o obj/components/ast_visualizer.o obj/components/ast_json_exporter.o obj/components/parsers/parser.o obj/components/parsers/expressions.o obj/components/parsers/statements.o obj/components/parsers/conditionals.o obj/components/parsers/functions.o obj/components/parsers/loops.o obj/components/generator/codegen.o obj/semantic.o obj/utils/lex.yy.o -o cmpx.exe

echo Build completed!


