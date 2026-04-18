#include <iostream>
#include <stdexcept>

#include "parser.h"

Token& Parser::peek() { return tokens[pos];}
Token  Parser::advance() { return tokens[pos++];}

bool Parser::check(TokenType t) {
  return peek().type == t;
}

Token Parser::expect(TokenType t, const std::string& msg) {
  if (!check(t)) throw std::runtime_error(msg);
  return advance();
}

bool Parser::match(TokenType t) {
  if (!check(t)) return false;
  advance(); return true;
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
  Token t = peek();

  if (t.type == TokenType::Number) {
    advance();
    auto node = std::make_unique<NumberLit>();
    node->value = std::stod(t.text);
    return node;
  }

  if (t.type == TokenType::Identifier) {
    advance();
    auto node = std::make_unique<Identifier>();
    node->value = t.text;
    return node;
  }
  
  if (t.type == TokenType::LParen) {
    advance(); // consume '('
    auto expr = parseExpression();
    expect(TokenType::RParen, "Expected ')'");
    return expr;
  }

  throw std::runtime_error("Unexpected token: " + t.text);
}

std::unique_ptr<ASTNode> Parser::parseMult() {
  auto left = parsePrimary();

  while(check(TokenType::Multiply) || check(TokenType::Divide) || check(TokenType::Modulo)) { 
    Token op = advance();  // returns operation token and moves forward :: return tokens[pos++]
    auto right = parsePrimary(); // parse the right-hand side operand
    auto node = std::make_unique<BinaryExpr>();  //construct a new BinaryExpr node
    node->op = op.text; // set the operation of the binary expression
    node->left = std::move(left); // set the left operand of the binary expression
    node->right = std::move(right); // set the right operand of the binary expression
    left = std::move(node); // create a new AST starting with node being the left child
  }

  return left;
}

std::unique_ptr<ASTNode> Parser::parseAdditive() {
  auto left = parseMult();                           // Allows multiplication priority over addition

  while(check(TokenType::Plus) || check(TokenType::Minus))
  {
    Token op = advance();
    auto right = parseMult();                        // Allows multiplication priority over addition
    auto node = std::make_unique<BinaryExpr>();
    node->op = op.text;
    node->right = std::move(right);
    node->left = std::move(left);
    left = std::move(node);
  }

  return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
  auto left = parseAdditive();

  while (check(TokenType::LT) || check(TokenType::GT) || check(TokenType::LTE) 
        || check(TokenType::GTE) || check(TokenType::ComprEquals) || check(TokenType::NotEquals))
  {
    Token op = advance();
    auto right = parseAdditive();
    auto node = std::make_unique<BinaryExpr>();
    node->op = op.text;
    node->left = std::move(left);
    node->right = std::move(right);
    left = std::move(node);
  }

  return left;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
  return parseComparison();
}

//TODO
/*

-define parseStatement()
-define parseBlock()
-define parse()
-add parser constructor


*/