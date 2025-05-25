@echo off
echo Assembling and linking %1...

REM Assemble the .asm file
nasm -f elf64 %1 -o main.o

REM Link with basic linker
ld -o program main.o

REM Run the program if linking was successful
if %errorlevel% equ 0 (
    echo Running program...
    .\program
) else (
    echo Linking failed.
)
