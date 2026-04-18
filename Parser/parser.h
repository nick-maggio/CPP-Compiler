#include <iostream>

#include "ast.h"
#include "../token.h"

class Parser{
public:
  explicit Parser(std::vector<Token> tokens);
  std::unique_ptr<ASTNode> parse();

private:
  std::vector<Token> tokens;
  size_t pos;

  // Navigation and matching
  Token& peek();
  Token  advance();
  bool   check(TokenType t);
  Token  expect(TokenType t, const std::string& msg);
  bool   match(TokenType t);
  
  // Expression parsing
  std::unique_ptr<ASTNode> parseExpression();
  std::unique_ptr<ASTNode> parseComparison();
  std::unique_ptr<ASTNode> parseAdditive();
  std::unique_ptr<ASTNode> parseMult();
  std::unique_ptr<ASTNode> parsePrimary();

  // Statement parsing
  std::unique_ptr<ASTNode> parseStatement();
  std::unique_ptr<ASTNode> parseBlock();

};