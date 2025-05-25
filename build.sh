#!/bin/bash

echo "Compiling CompilerX..."

# Run flex to generate lex.yy.c
cd utils
flex lexer.l
cd ..

# Create directories if they don't exist
mkdir -p obj
mkdir -p obj/components
mkdir -p obj/components/parsers
mkdir -p obj/components/generator
mkdir -p obj/utils

# Add compiler flags for debugging and warnings
CFLAGS="-g -Wall -Wextra -Werror=implicit-function-declaration"

# Compile individual source files with improved flags
gcc $CFLAGS -c components/tokens.c -o obj/components/tokens.o
gcc $CFLAGS -c utils/lex.yy.c -o obj/utils/lex.yy.o
gcc $CFLAGS -c compiler.c -o obj/compiler.o
gcc $CFLAGS -c components/memory.c -o obj/components/memory.o
gcc $CFLAGS -c components/symbol_table.c -o obj/components/symbol_table.o
gcc $CFLAGS -c components/ast_visualizer.c -o obj/components/ast_visualizer.o
gcc $CFLAGS -c components/ast_json_exporter.c -o obj/components/ast_json_exporter.o
gcc $CFLAGS -c components/parsers/parser.c -o obj/components/parsers/parser.o
gcc $CFLAGS -c components/parsers/expressions.c -o obj/components/parsers/expressions.o
gcc $CFLAGS -c components/parsers/statements.c -o obj/components/parsers/statements.o
gcc $CFLAGS -c components/parsers/conditionals.c -o obj/components/parsers/conditionals.o
gcc $CFLAGS -c components/parsers/functions.c -o obj/components/parsers/functions.o
gcc $CFLAGS -c components/parsers/loops.c -o obj/components/parsers/loops.o
gcc $CFLAGS -c components/generator/codegen.c -o obj/components/generator/codegen.o
gcc $CFLAGS -c semantic.c -o obj/semantic.o

# Link all object files with debugging information
gcc $CFLAGS obj/compiler.o obj/components/memory.o obj/components/symbol_table.o obj/components/tokens.o obj/components/ast_visualizer.o obj/components/ast_json_exporter.o obj/components/parsers/parser.o obj/components/parsers/expressions.o obj/components/parsers/statements.o obj/components/parsers/conditionals.o obj/components/parsers/functions.o obj/components/parsers/loops.o obj/components/generator/codegen.o obj/semantic.o obj/utils/lex.yy.o -o cmpx

echo "Build completed!"

# Optional: Create bin directory and copy executable
mkdir -p bin
cp cmpx bin/
echo "Executable copied to bin/cmpx"

# Make the executable executable
chmod +x cmpx
chmod +x bin/cmpx

# Run a simple test to check for segmentation faults
echo "Running a simple test..."
if [ -f "tests/simple.cx" ]; then
    ./cmpx tests/simple.cx
    if [ $? -ne 0 ]; then
        echo "Test failed! The compiler might have issues."
    else
        echo "Test passed!"
    fi
else
    echo "No test file found at tests/simple.cx. Skipping test."
fi

