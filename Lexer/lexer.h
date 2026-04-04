#pragma once
#include "../token.h"
#include <vector>
#include <string>

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    const std::string& src;
    size_t             pos;
    size_t             line;

    // ── Member functions ──────────────────────────
    bool isAtEnd() const;
    char peek(size_t offset = 0) const;
    char advance();
    bool match(char expected, size_t offset = 1) const;
    void skipBOM();
    void skipWhitespaceAndComments();
    Token make(TokenType type, const std::string& text) const;
    Token lexIdentifierOrKeyword();
    Token lexNumber();
    Token lexString();
    Token nextToken();


};