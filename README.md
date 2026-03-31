# CPP-Compiler

A learning project implementing a minimal C++ subset compiler front-end in C++.

Currently this is just a C++ tokenizer/lexer. Full compilation capabilities will be added.

- Lexer: tokenizes source code
- COMING SOON / Parser: parses tokens to AST
- COMING SOON / Code generation:  emits IR / target code 

## Status

 Initial lexer implementation exists in `Lexer/lexer.cpp` and sample input in `Lexer/CPPSourceCode.cpp`.

 Remaining milestones:
- Recursive descent parsing to create grammar structures for AST
- AST construction and semantic checks
- symbol table, scoping, type checking
- backend/code generator details TBD
- support for more C++ features incrementally

## Repository structure

- `Lexer/`
  - `CPPSourceCode.cpp` – sample source text for lexer test. Customize this file to whatever CPP source code ought be compiled
  - `lexer.cpp` – tokenizer implementation
- `README.md` – introduction and general information

## Build

### Cross-platform (g++)
1. `cd c:/CPPCompiler/CPP-Compiler/Lexer`
2. `g++ -std=c++17 lexer.cpp -o lexer`   -Note: lexer.cpp can be compiled with versions C++11 and newer
3. `./lexer CPPSourceCode.cpp` (or `lexer.exe` on Windows) any text after ./lexer is interpreted as the path of the file to be lexed

## Usage

1. Edit `Lexer/CPPSourceCode.cpp` with test C++-subset code or use another file.
2. Build as above.
3. Run the executable; lexer output should print token stream or diagnostics.
   - Pass any `.cpp` file as argument: `lexer myfile.cpp`

## LEXER ONLY Features

Recognizes keywords:

 - int, double, string
 - if, else
 - while, do, for
 - switch, case, default
 - break, continue, return
 - cin, cout
 - using, namespace, #include

Recognizes operators:

 - =, ==, !=, !
 - <, >, <=, >=
 - +, -, *, /, %
 - <<, >>
 - ++, --

Recognizes delimiters and punctuation:

 - ;, (), [], {}, :, ,   (No, that last comma is not a typo. The lexer recognizes commas)

Recognizes escape characters:

 - \n, \t, \", \\

## TODO

- [ ] Parser: parse declarations, expressions
- [ ] AST: Feed parsed results into abstract syntax tree
- [ ] Type checker
- [ ] Basic codegen (target IR)

## Contact

Questions/issues: add GitHub issues in repo.
