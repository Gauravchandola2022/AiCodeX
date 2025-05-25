@echo off
echo Compiling CompilerX...

REM
mkdir obj 2>nul
mkdir obj\components 2>nul
mkdir obj\components\parsers 2>nul

REM
gcc -c compiler.c -o obj/compiler.o
gcc -c components/memory.c -o obj/components/memory.o
gcc -c components/symbol_table.c -o obj/components/symbol_table.o
gcc -c components/tokens.c -o obj/components/tokens.o
gcc -c components/parsers/parser.c -o obj/components/parsers/parser.o
gcc -c components/parsers/expressions.c -o obj/components/parsers/expressions.o
gcc -c components/parsers/statements.c -o obj/components/parsers/statements.o
gcc -c components/parsers/conditionals.c -o obj/components/parsers/conditionals.o
gcc -c components/parsers/functions.c -o obj/components/parsers/functions.o
gcc -c components/parsers/loops.c -o obj/components/parsers/loops.o
gcc -c semantic.c -o obj/semantic.o

REM
gcc obj/compiler.o obj/components/memory.o obj/components/symbol_table.o obj/components/tokens.o obj/components/parsers/parser.o obj/components/parsers/expressions.o obj/components/parsers/statements.o obj/components/parsers/conditionals.o obj/components/parsers/functions.o obj/components/parsers/loops.o obj/semantic.o -o cmpx.exe

echo Build completed!



