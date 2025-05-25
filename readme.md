# AiCodeX

AiCodeX is a compiler that compiles cx code into Assembly code using flex and bison in C. The purpose of this compiler is to learn how to use flex and bison in C and how to write a compiler for your own language in C. It is also a good example of how to use flex and bison in C to write a compiler.

## Installation

To install AiCodeX, you need to have flex and bison installed on your computer. You can download flex and bison from [here](https://www.gnu.org/software/flex/bison.html).

## Building CompilerX

### Using the Build Script

The easiest way to build CompilerX is to use the provided build script:

```
# On Windows
$ build.bat

# On Linux/macOS
$ make
```

### Manual Build Process

If you prefer to build manually, follow these steps:

1. Generate the lexer C file:
```
$ cd utils
$ flex lexer.l
$ cd ..
```

2. Compile all source files:
```
$ gcc -c compiler.c -o obj/compiler.o
$ gcc -c components/memory.c -o obj/components/memory.o
$ gcc -c components/symbol_table.c -o obj/components/symbol_table.o
$ gcc -c components/tokens.c -o obj/components/tokens.o
$ gcc -c components/ast_visualizer.c -o obj/components/ast_visualizer.o
$ gcc -c components/ast_json_exporter.c -o obj/components/ast_json_exporter.o
$ gcc -c components/parsers/parser.c -o obj/components/parsers/parser.o
$ gcc -c components/parsers/expressions.c -o obj/components/parsers/expressions.o
$ gcc -c components/parsers/statements.c -o obj/components/parsers/statements.o
$ gcc -c components/parsers/conditionals.c -o obj/components/parsers/conditionals.o
$ gcc -c components/parsers/functions.c -o obj/components/parsers/functions.o
$ gcc -c components/parsers/loops.c -o obj/components/parsers/loops.o
$ gcc -c semantic.c -o obj/semantic.o
$ gcc -c utils/lex.yy.c -o obj/utils/lex.yy.o
```

3. Link all object files:
```
$ gcc obj/compiler.o obj/components/memory.o obj/components/symbol_table.o obj/components/tokens.o obj/components/ast_visualizer.o obj/components/ast_json_exporter.o obj/components/parsers/parser.o obj/components/parsers/expressions.o obj/components/parsers/statements.o obj/components/parsers/conditionals.o obj/components/parsers/functions.o obj/components/parsers/loops.o obj/semantic.o obj/utils/lex.yy.o -o cmpx.exe
```

4. Copy the executable to the bin directory (optional):
```
$ mkdir -p bin
$ cp cmpx.exe bin/
```

## Using AiCodeX

### Compiling a CX Program

To compile a CX program, use the following command:

```
$ cmpx example.cx
```

This will:
1. Tokenize the source code.
2. Parse the tokens into an Abstract Syntax Tree (AST).
3. Perform semantic analysis.
4. Generate assembly code.
5. Output visualization of the AST structure.
6. Export the AST to JSON for further analysis.

### Compiler Output

AiCodeX generates several outputs:

1. **Console Output**: Shows the tokenization process, AST visualization, and compilation status
2. **AST JSON File**: `ast_all_statements.json` contains the complete AST in JSON format
3. **Assembly Code**: Generated assembly code ready for execution

### Visualizing the AST

The AST is visualized in two ways:
1. Text representation in the console
2. JSON file that can be visualized using external tools

### Example Execution Flow

When you run `cmpx example.cx`, the following happens:

1. The lexer breaks down the source code into tokens
2. The parser constructs an AST from these tokens
3. The semantic analyzer checks for errors
4. The code generator produces assembly code
5. The AST is visualized and exported to JSON

For example, with this input:
```
num x = 10;
num y = 5;
print x + y;
```

The compiler will:
- Identify tokens: `num`, `x`, `=`, `10`, `;`, etc.
- Build an AST with variable declarations and a print statement
- Check that variables are declared before use
- Generate assembly code to allocate memory, perform addition, and print
- Visualize the AST structure in the console
- Export the AST to a JSON file

## Examples

You can find some examples of cx code below.
```
# A simple program that prints the sum of two numbers in cx
int x = 10;
int y = 5;

print x;
print y;

x = x + y;

print x;

int z = x * 2;

print z;
```

## How Our Compiler Works


```
.
└── CompilerX/
    ├── bin/
    │   └── cmpx.exe # Executable compiler file for $PATH variable
    ├── components/
    │   ├── ast.h
    │   ├── tokens.c
    │   ├── tokens.h
    │   └── ...Other header files for declarations
    ├── logs/ # To handle error logs
    ├── scripts/ # Various auto scripts
    ├── utils/
    │   ├── codePreprocessor.l # Lexical file for comment and spaces remove.
    │   ├── lex.yy.c # Lexical c file to perform lexical analysis.
    │   └── lexer.l # Lexical file for lexical analysis
    ├── .gitignore
    ├── changes.md # Maintain at every PR
    ├── cmpx.exe # Only for testing duration
    ├── compiler.c # Main file to compile compiler
    ├── parser.c # For analysis will be splitted into modules soon.
    └── semantic.c # For analysis adn AST to Assembly code generation.

```
### **Roles of Each Component:**  

#### 1️⃣ **Lexer (Lexical Analyzer)**  
- **Role:** Scans the source code and breaks it into tokens.  
- **Tokens:** `Identifiers`, `Keywords`, `Operators`, `Numbers`.  
- **Example:**  
  ```plaintext
  int x = 5;
  ```
  becomes:  
  ```
  <KEYWORD: int> <ID: x> <ASSIGN: => <NUMBER: 5>
  ```
- **Lex Usage:**  
   - Define patterns (e.g., `int`, `float`, `if`, `while`).  
   - Handle whitespace and comments.  
   - Output tokens for the parser.

---

#### 2️⃣ **Parser (Syntax Analyzer)**  
- **Role:** Analyzes token sequences based on grammar rules.  
- **Uses:** Recursive Descent Parsing or `yacc/bison` with Lex.  
- **Checks:** Correct structure like `if-else` blocks or expressions.  
- **Example:**  
   - Grammar for assignment:  
     ```
     statement -> ID ASSIGN expression
     expression -> term (OPERATOR term)*
     term -> ID | NUMBER
     ```
- **Error Handling:** Detects `Syntax Error` if grammar is incorrect.

---

#### 3️⃣ **Semantic Analyzer**  
- **Role:** Ensures meaningful code.  
- **Tasks:**  
  - Type Checking.  
  - Symbol Table management.  
  - Scope validation.  
- **Example:**  
   - Invalid code:  
     ```c
     int x = "hello"; // Type mismatch
     ```

---

#### 4️⃣ **Compiler (`compiler.c`)**  
- **Role:** Coordinates everything.  
- **Responsibilities:**  
  - Calls `lexer.c` to tokenize.  
  - Passes tokens to `parser.c` to validate structure.  
  - Performs semantic checks.  
  - Generates intermediate/assembly code.  
- **Output:** Machine-executable code or instructions.

---

### Reference
- [Parsing expressions by precedence climbing](https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing)
- [CS 340: Lecture 6: Precedence Climbing, Abstract Syntax Trees](https://ycpcs.github.io/cs340-fall2018/lectures/lecture06.html)
- Recursive Descent Parser in C
- [Untangling Lifetimes: The Arena Allocator](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator)
--- 

### **Precedence and Arena Usage in Memory Management**  

In our project, **precedence and arena-based memory management** are used to optimize performance, reduce fragmentation, and efficiently allocate and deallocate memory.  

#### **Why Use Precedence?**  
Precedence is used to determine the priority of memory allocation strategies based on factors such as access speed, alignment, and resource constraints. It ensures that:  
- **Frequently used memory blocks** are allocated efficiently.  
- **Memory deallocation follows an ordered approach** to prevent leaks.  
- **Cache-friendly memory allocation** enhances execution speed.  

#### **Why Use Arena-Based Memory Management?**  
Arena-based memory management helps in:  
- **Fast allocation and deallocation** by managing memory in pre-allocated chunks.  
- **Minimizing fragmentation** since memory is reused efficiently.  
- **Simplified memory handling**, as all memory is freed at once when the arena is destroyed, reducing overhead.  

This approach is particularly beneficial in scenarios where frequent memory allocation and deallocation occur, such as **real-time applications, compilers, and high-performance computing tasks**.  

---

## Author
- [Gaurav Chandola](https://github.com/Gauravchandola2022)
- [Sachin Bhawala](https://github.com/SachinBhawala)

## License

AiCodeX is released under the [MIT License](https://opensource.org/licenses/MIT).

## Future work
- Convert large If-Else blocks to Switch-Case blocks for better performance.
- Bytecode Generation (Platform Independency).
- faster Execution.
- statement issues, for open code it will work but for any function it will not, basically `}` is not being detectable and any return statement. (should be fixed soon).
- Create a function pool with parameters and return types, and use it to make a function table.
- Adding AI to the Compiler.
