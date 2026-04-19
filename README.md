# CPP-Compiler

A learning project implementing a minimal C++ subset compiler front-end in C++.

The lexer and parser are complete. The compiler currently tokenizes source code and parses it into an Abstract Syntax Tree (AST), which is printed to the console for inspection. Semantic analysis and code generation are next.

- **Lexer**: tokenizes source code into a typed token stream
- **Parser**: recursive descent parser that builds an AST
- **AST Printer**: pretty-prints the AST to the console for debugging
- COMING SOON / Semantic analysis: symbol table, scoping, type checking
- COMING SOON / Code generation: IR emission and target code

## Status

The lexer and parser are fully implemented. The AST printer displays the parsed tree structure. Remaining milestones:

- Semantic analysis: symbol table, scoping, type checking
- IR generation
- Backend / code generator (target TBD)
- Support for more C++ features incrementally (function calls, multi-variable declarations, etc.)

## Repository Structure
CPP-Compiler/
├── main.cpp               – Entry point: lexes, parses, and prints the AST
├── CPPSourceCode.cpp      – Sample source code for your testing. Point the compiler at any .cpp file
├── token.h                – TokenType enum, Token struct, KEYWORDS map, tokenTypeName()
├── Lexer/
│   ├── lexer.h            – Lexer class declaration
│   └── lexer.cpp          – Lexer method implementations
└── Parser/
├── ast.h              – AST node structs
├── parser.h           – Parser class declaration
├── parser.cpp         – Recursive descent parser implementation
└── printer.h          – AST pretty-printer

## Build

```bash
g++ -std=c++17 main.cpp Lexer/lexer.cpp Parser/parser.cpp -o compiler
```

## Usage

```bash
./compiler CPPSourceCode.cpp
```

Pass any `.cpp` file as the argument. Output prints the full AST of the parsed file.

## Supported Language Features

### Types
`int`, `double`, `string`

### Declarations
- Variable declarations with initializer: `int a = 1;`
- Function declarations with parameters: `int foo(int a, int b) { ... }`

### Expressions
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Assignment: `a = expr`
- Unary: `!`, `-` (prefix), `++`, `--` (prefix and postfix)
- Parenthesized expressions: `(a + b) * c`

### Statements
- `if` / `else if` / `else`
- `while`
- `do` / `while`
- `for`
- `switch` / `case` / `default`
- `break`, `continue`
- `return`
- `cout <<`
- Expression statements: `a = a + 1;`

### Ignored (safely skipped)
- `#include`
- `using namespace`

### Escape sequences in string literals
`\n`, `\t`, `\"`, `\\`

## TODO

- [ ] Semantic analysis: symbol table, scope tracking, type checking
- [ ] Function calls in expressions: `foo(a, b)`
- [ ] Multi-variable declarations: `int a = 1, b = 2`
- [ ] IR generation
- [ ] Backend / code generator

## Contact

Questions/issues: open a GitHub issue in the repo.