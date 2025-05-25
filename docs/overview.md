### CompilerX Documentation

---

### Current Possibilities

#### Lexical Analysis & Parsing

CompilerX currently implements a fully functional Lexical Analyzer and Recursive Descent Parser. The lexical analyzer, built using `flex`, scans the source code and breaks it down into a sequence of meaningful tokens such as identifiers, keywords, literals, and operators. These tokens are passed to the parser, which checks if they follow the grammatical structure defined for the language. CompilerX uses Recursive Descent Parsing, making it easy to read, debug, and extend the grammar. This parsing technique constructs the Abstract Syntax Tree (AST) directly without requiring intermediate parsing tables like LR parsers. The parser handles declarations, expressions, conditional blocks, loops, and function definitions. Error handling is also incorporated to identify syntactical mistakes during compilation. This structure provides a clear and modular separation between lexical and syntactic analysis.

---

#### Memory Leakage & Arena Memory Allocator

One of the significant optimizations in CompilerX is the integration of an Arena Allocator for memory management, reducing memory fragmentation and leaks. Traditionally, allocating and deallocating small memory chunks during compilation leads to fragmentation and complex memory tracking. CompilerX avoids this by allocating memory in large contiguous blocks (arenas). All data structures such as AST nodes, symbol tables, and intermediate representations are allocated inside these arenas. At the end of compilation, instead of freeing every individual allocation, the entire arena is freed at once. This strategy not only improves memory locality, enhancing cache performance, but also simplifies the deallocation process by eliminating the need for complex destructor logic.



---

#### Expression Parsing & Precedence Climbing

CompilerX utilizes Precedence Climbing for efficient and clear parsing of mathematical and logical expressions. Precedence Climbing is an alternative to operator-precedence parsers, providing an intuitive and recursive way to parse expressions based on operator precedence and associativity rules. Unlike naïve recursive descent where operator precedence must be hardcoded into each function, precedence climbing naturally resolves operator priority, ensuring correct construction of ASTs for nested and mixed expressions. CompilerX handles common operators like `+`, `-`, `*`, `/`, `=`, `&&`, `||`, `<`, `>`, and even complex expressions like `1 + 2 * (3 - 4)`.

---

### Future Possibilities

#### Complete Code Execution

CompilerX currently only generates and prints the Assembly code for input CX code. In future versions, the compiler will not only generate but also fully execute the code. This involves integrating a backend system to translate the assembly to machine code, link it, and execute it as a standalone binary. To achieve this, CompilerX could either use an existing assembler and linker (e.g., NASM + LD) or implement its own lightweight assembler. Complete code execution will enable features like program output, error reporting at runtime, and possibly interactive debugging.

---

#### Garbage Collector

Although the arena allocator solves compile-time memory issues, future versions may introduce a garbage collector for runtime memory management, especially if CompilerX evolves into a high-level language with dynamic memory allocation (`new`, `delete`, or `malloc()` like constructs). Implementing a Garbage Collector (GC) would make CompilerX more robust in terms of runtime memory safety. Options include simple mark-and-sweep collectors, generational collectors, or reference-counting mechanisms.

---

#### Expensive Code Sorting (Strength Reduction)

Expression patterns like `1 << x` and `x * 2` are extremely common in optimized code but can be costly if not handled smartly during compilation. CompilerX could introduce an expensive-code sorter and strength reduction optimizer that identifies computationally heavy patterns and replaces them with cheaper equivalents. For example, instead of generating code for `x * 2`, it could generate `x << 1`, which is faster on many architectures. Resources for these techniques include:

- CS 322: Strength Reduction Lecture
- Modern Compiler Implementation in C - Andrew Appel
- Optimizing Compilers for Modern Architectures - Allen & Kennedy

---

#### Just-In-Time Compilation (JIT)

JIT compilation is a powerful technique where code is compiled to machine code during execution rather than before execution. This allows for adaptive optimizations such as hot path detection, runtime constant propagation, and specialization based on actual inputs. For CompilerX, a simple JIT could be implemented by integrating it with dynamic code generators such as LLVM JIT or building a minimalistic custom JIT framework.

---

#### Other Suggested Improvements

1. Bytecode Virtual Machine
2. Intermediate Representation (IR) Optimizations
3. Debugger Integration
4. Modular Compiler Architecture

