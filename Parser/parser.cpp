#include <iostream>
#include <stdexcept>

#include "parser.h"

Token& Parser::peek() {
  return tokens[pos];
}

Token Parser::advance() {
  return tokens[pos++];
}

bool Parser::check(TokenType t) {
  return peek().type == t;
}

Token Parser::expect(TokenType t, const std::string& msg) {
  if (!check(t)) {
    throw std::runtime_error(msg + " got '" + peek().text + "' on line " + std::to_string(peek().line) + ")");
  }
  return advance();
}

bool Parser::match(TokenType t) {
  if (!check(t)) {
    return false;
  }
  advance();
  return true;
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

    // Parse postfix ++ and -- immediately after identifiers.
    if (check(TokenType::Increment) || check(TokenType::Decrement)) {
      Token op = advance();
      auto unary = std::make_unique<UnaryExpr>();
      unary->operation = op.text + std::string("_postfix");
      unary->operand = std::move(node);
      return unary;
    }

    return node;
  }

  if (t.type == TokenType::LParen) {
    advance();
    auto expr = parseExpression();
    expect(TokenType::RParen, "Expected ')'");
    return expr;
  }

  if (t.type == TokenType::StringLiteral) {
    advance();
    auto node = std::make_unique<StringLit>();
    node->value = t.text;
    return node;
  }

  throw std::runtime_error("Unexpected token: " + t.text + " on line: " + std::to_string(t.line));
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
  while (check(TokenType::Increment) || check(TokenType::Decrement) ||
         check(TokenType::Negate) || check(TokenType::Minus)) {
    Token operation = advance();
    auto operand = parseUnary();
    auto node = std::make_unique<UnaryExpr>();
    node->operation = operation.text + std::string("_prefix");
    node->operand = std::move(operand);
    return node;
  }

  return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parseMult() {
  auto left = parseUnary();

  while (check(TokenType::Multiply) || check(TokenType::Divide) || check(TokenType::Modulo)) {
    Token op = advance();
    auto right = parseUnary();
    auto node = std::make_unique<BinaryExpr>();
    node->op = op.text;
    node->left = std::move(left);
    node->right = std::move(right);
    left = std::move(node);
  }

  return left;
}

std::unique_ptr<ASTNode> Parser::parseAdditive() {
  auto left = parseMult();

  while (check(TokenType::Plus) || check(TokenType::Minus)) {
    Token op = advance();
    auto right = parseMult();
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

  while (check(TokenType::LT) || check(TokenType::GT) || check(TokenType::LTE) ||
         check(TokenType::GTE) || check(TokenType::ComprEquals) || check(TokenType::NotEquals)) {
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
  auto left = parseComparison();

  if (check(TokenType::Equals)) {
    advance();
    auto value = parseExpression();

    auto* ident = dynamic_cast<Identifier*>(left.get());
    if (!ident) {
      throw std::runtime_error("Left side of assignment needs variable name.");
    }

    auto node = std::make_unique<AssignExpr>();
    node->name = ident->value;
    node->value = std::move(value);
    return node;
  }
  return left;
}

std::unique_ptr<ASTNode> Parser::parseFunctionDecl(Token typeToken, Token nameToken) {
  expect(TokenType::LParen, "Expected '('");

  auto node = std::make_unique<FunctionDecl>();
  node->returnType = typeToken.text;
  node->name = nameToken.text;

  // Parse a simple comma-separated parameter list.
  while (!check(TokenType::RParen) && !check(TokenType::EndOfFile)) {
    Token paramType = advance();
    Token paramName = expect(TokenType::Identifier, "Expected parameter name");

    auto param = std::make_unique<ParamDecl>();
    param->type = paramType.text;
    param->name = paramName.text;
    node->params.push_back(std::move(param));

    if (!check(TokenType::RParen)) {
      expect(TokenType::Comma, "Expected ',' between parameters");
    }
  }

  expect(TokenType::RParen, "Expected ')'");
  node->body = parseBlock();
  return node;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
  Token t = peek();

  if (t.type == TokenType::LBrace) {
    return parseBlock();
  }

  // Ignore preprocessor includes at the statement level.
  if (t.type == TokenType::KeywordInclude) {
    size_t currentLine = peek().line;
    while (!check(TokenType::EndOfFile) && peek().line == currentLine) {
      advance();
    }
    return parseStatement();
  }

  // Ignore using namespace directives.
  if (t.type == TokenType::KeywordUsing) {
    while (!check(TokenType::Semicolon) && !check(TokenType::EndOfFile)) {
      advance();
    }
    advance();
    return parseStatement();
  }

  if (t.type == TokenType::KeywordInt ||
      t.type == TokenType::KeywordDouble ||
      t.type == TokenType::KeywordString) {
    Token typeToken = advance();
    Token nameToken = expect(TokenType::Identifier, "Expected name");

    if (check(TokenType::LParen)) {
      return parseFunctionDecl(typeToken, nameToken);
    }

    expect(TokenType::Equals, "Expected '='");
    auto init = parseExpression();
    expect(TokenType::Semicolon, "Expected ';'");

    auto node = std::make_unique<VarDecl>();
    node->type = typeToken.text;
    node->name = nameToken.text;
    node->init = std::move(init);
    return node;
  }

  if (t.type == TokenType::KeywordReturn) {
    advance();
    auto returnValue = parseExpression();
    expect(TokenType::Semicolon, "Expected ';'");
    auto node = std::make_unique<ReturnStmt>();
    node->value = std::move(returnValue);
    return node;
  }

  if (t.type == TokenType::KeywordIf) {
    advance();
    expect(TokenType::LParen, "Expected a '('");
    auto condition = parseExpression();
    expect(TokenType::RParen, "Expected a ')'");
    auto thenBlock = parseBlock();

    std::unique_ptr<ASTNode> elseBlock = nullptr;
    if (check(TokenType::KeywordElse)) {
      advance();
      elseBlock = parseStatement();
    }

    auto node = std::make_unique<IfStmt>();
    node->cond = std::move(condition);
    node->thenDo = std::move(thenBlock);
    node->elseDo = std::move(elseBlock);
    return node;
  }

  if (t.type == TokenType::KeywordWhile) {
    advance();
    expect(TokenType::LParen, "Expected a '('");
    auto condition = parseExpression();
    expect(TokenType::RParen, "Expected a ')'");
    auto doBlock = parseBlock();

    auto node = std::make_unique<WhileLoop>();
    node->cond = std::move(condition);
    node->body = std::move(doBlock);
    return node;
  }

  if (t.type == TokenType::KeywordFor) {
    advance();
    expect(TokenType::LParen, "Eexpected a '('");

    auto init = parseStatement();
    auto cond = parseExpression();
    expect(TokenType::Semicolon, "Expected a ';'");

    auto step = parseExpression();
    expect(TokenType::RParen, "Expected a ')'");

    auto body = parseBlock();

    auto node = std::make_unique<ForLoop>();
    node->init = std::move(init);
    node->cond = std::move(cond);
    node->step = std::move(step);
    node->body = std::move(body);
    return node;
  }

  if (t.type == TokenType::KeywordSwitch) {
    advance();
    expect(TokenType::LParen, "Expected '('");
    auto expr = parseExpression();
    expect(TokenType::RParen, "Expected ')'");
    expect(TokenType::LBrace, "Expected '{'");

    auto node = std::make_unique<SwitchStmt>();
    node->expr = std::move(expr);

    while (!check(TokenType::RBrace) && !check(TokenType::EndOfFile)) {
      auto caseNode = std::make_unique<CaseStmt>();

      if (check(TokenType::KeywordCase)) {
        advance();
        caseNode->value = parseExpression();
        expect(TokenType::Colon, "Expected ':'");
      } else if (check(TokenType::KeywordDefault)) {
        advance();
        expect(TokenType::Colon, "Expected ':'");
      } else {
        throw std::runtime_error("Expected 'case' or 'default' in switch");
      }

      // Gather statements until the next label or the end of the switch body.
      while (!check(TokenType::KeywordCase) &&
             !check(TokenType::KeywordDefault) &&
             !check(TokenType::RBrace) &&
             !check(TokenType::EndOfFile)) {
        caseNode->stmts.push_back(parseStatement());
      }

      node->cases.push_back(std::move(caseNode));
    }

    expect(TokenType::RBrace, "Expected '}'");
    return node;
  }

  if (t.type == TokenType::KeywordBreak) {
    advance();
    expect(TokenType::Semicolon, "Expected a ';'");
    return std::make_unique<BreakStmt>();
  }

  if (t.type == TokenType::KeywordContinue) {
    advance();
    expect(TokenType::Semicolon, "Expected a ';'");
    return std::make_unique<ContinueStmt>();
  }

  if (t.type == TokenType::KeywordDo) {
    advance();
    auto body = parseBlock();
    expect(TokenType::KeywordWhile, "Expected 'while'");
    expect(TokenType::LParen, "Expected '('");
    auto cond = parseExpression();
    expect(TokenType::RParen, "Expected ')'");
    expect(TokenType::Semicolon, "Expected ';'");

    auto node = std::make_unique<DoWhileLoop>();
    node->body = std::move(body);
    node->cond = std::move(cond);
    return node;
  }

  if (t.type == TokenType::KeywordCout) {
    advance();
    auto node = std::make_unique<CoutStmt>();
    while (check(TokenType::InsertionOp)) {
      advance();
      node->args.push_back(parseExpression());
    }
    expect(TokenType::Semicolon, "Expected ';'");
    return node;
  }

  auto expr = parseExpression();
  expect(TokenType::Semicolon, "Expected a ';'");
  return expr;
}

std::unique_ptr<ASTNode> Parser::parseBlock() {
  expect(TokenType::LBrace, "Expected a '{'");
  auto block = std::make_unique<Block>();

  while (!check(TokenType::RBrace) && (!check(TokenType::EndOfFile))) {
    block->stmts.push_back(parseStatement());
  }

  expect(TokenType::RBrace, "Expected a '}'");
  return block;
}

std::unique_ptr<ASTNode> Parser::parse() {
  auto program = std::make_unique<Block>();

  while (!check(TokenType::EndOfFile)) {
    program->stmts.push_back(parseStatement());
  }

  return program;
}

Parser::Parser(std::vector<Token> tokens)
  : tokens(std::move(tokens)), pos(0) {}
