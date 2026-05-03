#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Parser/printer.h"
#include "SemanticAnalysis/semantic.h"

static std::string readFile(const std::string& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file: " + path);
  }

  std::ostringstream buf;
  buf << file.rdbuf();
  return buf.str();
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Improper Usage. Please use: './compiler <source-file-path>'\n";
    return 1;
  }

  std::string source;
  try {
    source = readFile(argv[1]);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  Lexer lexer(source);
  std::vector<Token> tokens = lexer.tokenize();

  Parser parser(std::move(tokens));
  auto ast = parser.parse();

  std::cout << "=== AST ===\n";
  printNode(ast.get());

  std::cout << "\n=== Semantic Analysis ===\n";
  SemanticAnalyzer sema;
  bool ok = sema.analyze(ast.get());

  if (ok) {
    std::cout << "No errors found in code.\n";
  }
  else {
    for (const auto& err : sema.errors())
      std::cerr << "[Error]" << err.message << '\n';
    return 1;
  }
  return 0;
}
