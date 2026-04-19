#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Parser/printer.h"

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

  printNode(ast.get());
  return 0;
}
