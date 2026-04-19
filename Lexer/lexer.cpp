#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cctype>

#include "../token.h"
#include "lexer.h"

Lexer::Lexer(const std::string& source)
  : src(source), pos(0), line(1) {
  skipBOM();
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;

  while (true) {
    skipWhitespaceAndComments();

    if (isAtEnd()) {
      tokens.push_back(make(TokenType::EndOfFile, ""));
      break;
    }

    tokens.push_back(nextToken());
  }

  return tokens;
}

bool Lexer::isAtEnd() const {
  return pos >= src.size();
}

char Lexer::peek(size_t offset) const {
  size_t idx = pos + offset;
  return idx < src.size() ? src[idx] : '\0';
}

char Lexer::advance() {
  char c = src[pos++];
  if (c == '\n') {
    ++line;
  }
  return c;
}

bool Lexer::match(char expected, size_t offset) const {
  return peek(offset) == expected;
}

void Lexer::skipBOM() {
  if (src.size() >= 3 &&
      (unsigned char)src[0] == 0xEF &&
      (unsigned char)src[1] == 0xBB &&
      (unsigned char)src[2] == 0xBF) {
    pos = 3;
  }
}

void Lexer::skipWhitespaceAndComments() {
  while (!isAtEnd()) {
    if (std::isspace((unsigned char)peek())) {
      advance();
      continue;
    }

    if (peek() == '/' && match('/')) {
      while (!isAtEnd() && peek() != '\n') {
        advance();
      }
      continue;
    }

    if (peek() == '/' && match('*')) {
      advance();
      advance();
      while (!isAtEnd()) {
        if (peek() == '*' && match('/')) {
          advance();
          advance();
          break;
        }
        advance();
      }
      continue;
    }

    break;
  }
}

Token Lexer::make(TokenType type, const std::string& text) const {
  return Token{ type, text, line };
}

Token Lexer::lexIdentifierOrKeyword() {
  size_t start = pos;
  while (!isAtEnd() && (std::isalnum((unsigned char)peek()) || peek() == '_')) {
    advance();
  }

  std::string text = src.substr(start, pos - start);
  auto it = KEYWORDS.find(text);
  TokenType type = (it != KEYWORDS.end()) ? it->second : TokenType::Identifier;
  return make(type, text);
}

Token Lexer::lexNumber() {
  size_t start = pos;
  while (!isAtEnd() && std::isdigit((unsigned char)peek())) {
    advance();
  }

  if (!isAtEnd() && peek() == '.') {
    advance();
    while (!isAtEnd() && std::isdigit((unsigned char)peek())) {
      advance();
    }
  }

  return make(TokenType::Number, src.substr(start, pos - start));
}

Token Lexer::lexString() {
  advance();
  std::string value;

  while (!isAtEnd() && peek() != '"') {
    if (peek() == '\\') {
      advance();
      switch (peek()) {
        case 'n':
          value += '\n';
          break;
        case 't':
          value += '\t';
          break;
        case '"':
          value += '"';
          break;
        case '\\':
          value += '\\';
          break;
        default:
          value += peek();
          break;
      }
    } else {
      value += peek();
    }
    advance();
  }

  if (isAtEnd()) {
    return make(TokenType::Invalid, "Unterminated string literal");
  }

  advance();
  return make(TokenType::StringLiteral, value);
}

Token Lexer::nextToken() {
  char c = peek();

  if (std::isalpha((unsigned char)c) || c == '_') {
    return lexIdentifierOrKeyword();
  }
  if (std::isdigit((unsigned char)c) || (c == '.' && std::isdigit((unsigned char)peek(1)))) {
    return lexNumber();
  }
  if (c == '"') {
    return lexString();
  }

  advance();

  switch (c) {
    case '=':
      if (peek() == '=') {
        advance();
        return make(TokenType::ComprEquals, "==");
      }
      return make(TokenType::Equals, "=");

    case '!':
      if (peek() == '=') {
        advance();
        return make(TokenType::NotEquals, "!=");
      }
      return make(TokenType::Negate, "!");

    case '<':
      if (peek() == '=') {
        advance();
        return make(TokenType::LTE, "<=");
      }
      if (peek() == '<') {
        advance();
        return make(TokenType::InsertionOp, "<<");
      }
      return make(TokenType::LT, "<");

    case '>':
      if (peek() == '=') {
        advance();
        return make(TokenType::GTE, ">=");
      }
      if (peek() == '>') {
        advance();
        return make(TokenType::ExtractionOp, ">>");
      }
      return make(TokenType::GT, ">");

    case '+':
      if (peek() == '+') {
        advance();
        return make(TokenType::Increment, "++");
      }
      return make(TokenType::Plus, "+");

    case '-':
      if (peek() == '-') {
        advance();
        return make(TokenType::Decrement, "--");
      }
      return make(TokenType::Minus, "-");

    case '*':
      return make(TokenType::Multiply, "*");
    case '/':
      return make(TokenType::Divide, "/");
    case '%':
      return make(TokenType::Modulo, "%");
    case ';':
      return make(TokenType::Semicolon, ";");
    case '(':
      return make(TokenType::LParen, "(");
    case ')':
      return make(TokenType::RParen, ")");
    case '{':
      return make(TokenType::LBrace, "{");
    case '}':
      return make(TokenType::RBrace, "}");
    case ',':
      return make(TokenType::Comma, ",");
    case ':':
      return make(TokenType::Colon, ":");

    case '#': {
      size_t start = pos - 1;
      while (!isAtEnd() && std::isalpha((unsigned char)peek())) {
        advance();
      }
      std::string text = src.substr(start, pos - start);
      auto it = KEYWORDS.find(text);
      if (it != KEYWORDS.end()) {
        return make(it->second, text);
      }
      return make(TokenType::Invalid, text);
    }

    default:
      return make(TokenType::Invalid, std::string(1, c));
  }
}