#!/bin/bash

# Enable core dumps
ulimit -c unlimited

# Compile with debug symbols
./build.sh

# Run the program that causes the segfault
echo "Running program to generate core dump..."
./cmpx tests/simple.cx

# Check if core dump was generated
if [ -f "core" ] || [ -f "core.$(pidof cmpx)" ]; then
    echo "Core dump generated. Analyzing with GDB..."
    
    # Find the core file
    CORE_FILE=$(find . -name "core*" -type f -newer cmpx | head -1)
    
    # Use GDB to analyze the core dump
    gdb ./cmpx $CORE_FILE -ex "bt full" -ex "quit"
else
    echo "No core dump found. Running with GDB directly..."
    gdb -ex "run tests/simple.cx" -ex "bt full" -ex "quit" ./cmpx
fi