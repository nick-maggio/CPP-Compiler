# CPP-Compiler

A learning project implementing a minimal C++ subset compiler front-end in C++.

The lexer, parser, and semantic analyzer are complete. The compiler tokenizes source code, parses it into an Abstract Syntax Tree (AST), validates it for type correctness and scoping, and prints both the AST and any semantic errors to the console. Code generation is next.

- **Lexer**: tokenizes source code into a typed token stream
- **Parser**: recursive descent parser that builds an AST
- **AST Printer**: pretty-prints the AST to the console for debugging
- **Semantic Analyzer**: symbol table, scoping, and type checking
- COMING SOON / IR generation and code generation

## Status

The lexer, parser, and semantic analyzer are fully implemented. Remaining milestones:

- IR generation
- Backend / code generator (target TBD)
- Support for more C++ features incrementally (function calls, multi-variable declarations, etc.)

## Repository Structure

```
CPP-Compiler/
├── main.cpp                      – Entry point: lexes, parses, prints AST, runs semantic analysis
├── CPPSourceCode.cpp             – Sample source for testing
├── token.h                       – TokenType enum, Token struct, KEYWORDS map, tokenTypeName()
├── Lexer/
│   ├── lexer.h                   – Lexer class declaration
│   └── lexer.cpp                 – Lexer method implementations
├── Parser/
│   ├── ast.h                     – AST node structs
│   ├── parser.h                  – Parser class declaration
│   ├── parser.cpp                – Recursive descent parser implementation
│   └── printer.h                 – AST pretty-printer
└── SemanticAnalysis/
    ├── semantic.h                – Symbol, Scope, ScopeStack, SemanticError, SemanticAnalyzer
    └── semantic.cpp              – Semantic analysis implementation
```

## Build

```bash
g++ -std=c++17 main.cpp Lexer/lexer.cpp Parser/parser.cpp SemanticAnalysis/semantic.cpp -o compiler
```

## Usage

```bash
./compiler CPPSourceCode.cpp
```

Pass any `.cpp` file as the argument. Output prints the full AST followed by the result of semantic analysis.

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

## Semantic Analysis

The semantic analyzer walks the AST and validates meaning after parsing. It catches errors that the parser cannot, since the parser only checks structure.

### What it checks

- **Undeclared identifiers**: use of a variable or function before it is declared
- **Type mismatches**: initializing or assigning a variable with an incompatible type
- **Invalid operators**: applying `%` to a `double`, or arithmetic to a `string`
- **Return type mismatches**: returning a value whose type differs from the function's declared return type
- **Duplicate declarations**: declaring the same variable or function name twice in the same scope
- **Break / continue outside a loop**: using `break` or `continue` where they have no enclosing loop or switch
- **Invalid switch expressions**: switch condition must be `int`
- **Invalid case labels**: case values must be integer constants

### Scoping

Each block (`{}`), function body, `if`/`else` branch, loop body, and `for` init creates its own scope. Variables declared inside a scope are not visible outside it. Inner scopes shadow outer ones.

### Type rules

- `int` and `double` are mutually compatible in arithmetic and assignment
- `int + double` widens to `double`
- `string` only supports `+` (concatenation) and only with another `string`
- Comparison operators always produce `int` (0 or 1)
- `!` accepts any operand and always produces `int`

## TODO

- [ ] Function calls in expressions: `foo(a, b)`
- [ ] Multi-variable declarations: `int a = 1, b = 2`
- [ ] IR generation
- [ ] Backend / code generator

## Contact

Questions/issues: open a GitHub issue in the repo.
