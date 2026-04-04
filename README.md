# CPP-Compiler

A learning project implementing a minimal C++ subset compiler front-end in C++.

Currently this is a C++ tokenizer/lexer split across multiple files. Full compilation capabilities will be added.

- Lexer: tokenizes source code
- COMING SOON / Parser: parses tokens to AST
- COMING SOON / Code generation: emits IR / target code

## Status

Initial lexer implementation is split across `token.h`, `lexer.h`, and `lexer.cpp`. Sample input lives in `CPPSourceCode.cpp`.

Remaining milestones:
- Recursive descent parsing to create grammar structures for AST
- AST construction and semantic checks
- Symbol table, scoping, type checking
- Backend/code generator details TBD
- Support for more C++ features incrementally

## Repository structure

- `token.h` – TokenType enum, Token struct, KEYWORDS map, tokenTypeName()
- `lexer.h` – Lexer class declaration
- `lexer.cpp` – Lexer method implementations
- `main.cpp` – Entry point, reads file and prints token stream
- `CPPSourceCode.cpp` – Sample source text for lexer testing. Customize this file to whatever C++ source code ought to be compiled
- `README.md` – Introduction and general information

## Build

### Cross-platform (g++)
1. `cd c:/CPPCompiler/CPP-Compiler`
2. `g++ -std=c++17 main.cpp lexer.cpp -o output`
3. `./output CPPSourceCode.cpp` (or `output.exe` on Windows) — any path after `./lexer` is interpreted as the file to be lexed

## Usage

1. Edit `CPPSourceCode.cpp` with test C++-subset code or point at another file.
2. Build as above.
3. Run the executable; output prints the token stream or diagnostics.
   - Pass any `.cpp` file as argument: `./lexer myfile.cpp`

## Features

### Recognized keywords
- `int`, `double`, `string`
- `if`, `else`
- `while`, `do`, `for`
- `switch`, `case`, `default`
- `break`, `continue`, `return`
- `cin`, `cout`
- `using`, `namespace`, `#include`

### Recognized operators
- `=`, `==`, `!=`, `!`
- `<`, `>`, `<=`, `>=`
- `+`, `-`, `*`, `/`, `%`
- `<<`, `>>`
- `++`, `--`

### Recognized delimiters and punctuation
- `;`, `(`, `)`, `{`, `}`, `:`, `,`

### Recognized escape sequences
- `\n`, `\t`, `\"`, `\\`

## TODO

- [ ] `ast.h`: AST node structs
- [ ] `parser.h` / `parser.cpp`: recursive descent parser
- [ ] Type checker
- [ ] Basic codegen (target IR)

## Contact

Questions/issues: open a GitHub issue in the repo.