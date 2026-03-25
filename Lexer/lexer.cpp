#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

// ─────────────────────────────────────────────
//  Token Types
// ─────────────────────────────────────────────

enum class TokenType {
    // Literals
    Identifier,
    Number,
    StringLiteral,

    // Keywords
    KeywordInt,
    KeywordIf,
    KeywordElse,
    KeywordCout,
    KeywordReturn,

    // Operators
    Equals,          // =
    ComprEquals,     // ==
    NotEquals,       // !=
    Plus,            // +
    Negate,          // !
    LT,              // <
    GT,              // >
    LTE,             // <=
    GTE,             // >=
    InsertionOp,     // <<

    // Delimiters
    Semicolon,       // ;
    LParen,          // (
    RParen,          // )
    LBrace,          // {
    RBrace,          // }

    EndOfFile,
    Invalid
};

static const std::unordered_map<TokenType, std::string> TOKEN_NAMES = {
    { TokenType::Identifier,    "Identifier"    },
    { TokenType::Number,        "Number"        },
    { TokenType::StringLiteral, "StringLiteral" },
    { TokenType::KeywordInt,    "KeywordInt"    },
    { TokenType::KeywordIf,     "KeywordIf"     },
    { TokenType::KeywordElse,   "KeywordElse"   },
    { TokenType::KeywordCout,   "KeywordCout"   },
    { TokenType::KeywordReturn,  "KeywordReturn" },
    { TokenType::Equals,        "Equals"        },
    { TokenType::ComprEquals,   "ComprEquals"   },
    { TokenType::NotEquals,     "NotEquals"     },
    { TokenType::Plus,          "Plus"          },
    { TokenType::Negate,        "Negate"        },
    { TokenType::LT,            "LT"            },
    { TokenType::GT,            "GT"            },
    { TokenType::LTE,           "LTE"           },
    { TokenType::GTE,           "GTE"           },
    { TokenType::InsertionOp,   "InsertionOp"   },
    { TokenType::Semicolon,     "Semicolon"     },
    { TokenType::LParen,        "LParen"        },
    { TokenType::RParen,        "RParen"        },
    { TokenType::LBrace,        "LBrace"        },
    { TokenType::RBrace,        "RBrace"        },
    { TokenType::EndOfFile,     "EndOfFile"     },
    { TokenType::Invalid,       "Invalid"       },
};

static const std::unordered_map<std::string, TokenType> KEYWORDS = {
    { "int",  TokenType::KeywordInt  },
    { "if",   TokenType::KeywordIf   },
    { "else", TokenType::KeywordElse },
    { "cout", TokenType::KeywordCout },
    { "return", TokenType::KeywordInt } 
};

std::string tokenTypeName(TokenType type) {
    auto it = TOKEN_NAMES.find(type);
    return it != TOKEN_NAMES.end() ? it->second : "Unknown";
}

// ─────────────────────────────────────────────
//  Token Structure
// ─────────────────────────────────────────────

struct Token {
    TokenType   type;
    std::string text;
    size_t      line;   // line number on source file
};

// ─────────────────────────────────────────────
//  Lexer
// ─────────────────────────────────────────────

class Lexer {
public:
    explicit Lexer(const std::string& source)
        : src(source), pos(0), line(1)
    {
        skipBOM();
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        while (true) {
            skipWhitespaceAndComments();

            if (isAtEnd()) {
                tokens.push_back(make(TokenType::EndOfFile, ""));
                break;
            }

            Token tok = nextToken();
            tokens.push_back(tok);
        }

        return tokens;
    }

private:
    const std::string& src;
    size_t             pos;
    size_t             line;

    // ── Navigation ──────────────────────────

    bool isAtEnd() const {
        return pos >= src.size();
    }

    char peek(size_t offset = 0) const {
        size_t idx = pos + offset;
        return idx < src.size() ? src[idx] : '\0';
    }

    char advance() {
        char c = src[pos++];
        if (c == '\n') ++line;
        return c;
    }

    bool match(char expected, size_t offset = 1) const {
        return peek(offset) == expected;
    }

    // ── Skipping Bloat/Junk ─────────────────────────────

    void skipBOM() {
        if (src.size() >= 3
            && (unsigned char)src[0] == 0xEF
            && (unsigned char)src[1] == 0xBB
            && (unsigned char)src[2] == 0xBF)
        {
            pos = 3;
        }
    }

    void skipWhitespaceAndComments() {
        while (!isAtEnd()) {
            // Whitespace
            if (std::isspace((unsigned char)peek())) {
                advance();
                continue;
            }

            // Single-line comment  //
            if (peek() == '/' && match('/')) {
                while (!isAtEnd() && peek() != '\n')
                    advance();
                continue;
            }

            // Multi-line comment  /* ... */
            if (peek() == '/' && match('*')) {
                advance(); advance(); // consume /*
                while (!isAtEnd()) {
                    if (peek() == '*' && match('/')) {
                        advance(); advance(); // consume */
                        break;
                    }
                    advance();
                }
                continue;
            }

            break;
        }
    }

    // ── Token helpers ────────────────────────

    Token make(TokenType type, const std::string& text) const {
        return Token{ type, text, line };
    }

    // ── Sub-lexers ───────────────────────────

    Token lexIdentifierOrKeyword() {
        size_t start = pos;
        while (!isAtEnd() && (std::isalnum((unsigned char)peek()) || peek() == '_'))
            advance();

        std::string text = src.substr(start, pos - start);

        auto it = KEYWORDS.find(text);
        TokenType type = (it != KEYWORDS.end()) ? it->second : TokenType::Identifier;
        return make(type, text);
    }

    Token lexNumber() {
        size_t start = pos;
        while (!isAtEnd() && std::isdigit((unsigned char)peek()))
            advance();
        return make(TokenType::Number, src.substr(start, pos - start));
    }

    Token lexString() {
        advance(); // consume opening "
        std::string value;

        while (!isAtEnd() && peek() != '"') {
            if (peek() == '\\') {      // basic escape handling
                advance();
                switch (peek()) {
                    case 'n':  value += '\n'; break;
                    case 't':  value += '\t'; break;
                    case '"':  value += '"';  break;
                    case '\\': value += '\\'; break;
                    default:   value += peek(); break;
                }
            } else {
                value += peek();
            }
            advance();
        }

        if (!isAtEnd()) advance(); // consume closing "
        return make(TokenType::StringLiteral, value);
    }

    // ── Main dispatch ────────────────────────

    Token nextToken() {
        char c = peek();

        if (std::isalpha((unsigned char)c) || c == '_') return lexIdentifierOrKeyword();
        if (std::isdigit((unsigned char)c))              return lexNumber();
        if (c == '"')                                    return lexString();

        // Single- or double-character punctuation
        advance(); // consume the leading character

        switch (c) {
            case '=': if (peek() == '=') { advance(); return make(TokenType::ComprEquals,  "=="); }
                      return make(TokenType::Equals, "=");

            case '!': if (peek() == '=') { advance(); return make(TokenType::NotEquals,    "!="); }
                      return make(TokenType::Negate, "!");

            case '<': if (peek() == '=') { advance(); return make(TokenType::LTE,          "<="); }
                      if (peek() == '<') { advance(); return make(TokenType::InsertionOp,  "<<"); }
                      return make(TokenType::LT, "<");

            case '>': if (peek() == '=') { advance(); return make(TokenType::GTE,          ">="); }
                      return make(TokenType::GT, ">");

            case '+': return make(TokenType::Plus,      "+");
            case ';': return make(TokenType::Semicolon, ";");
            case '(': return make(TokenType::LParen,    "(");
            case ')': return make(TokenType::RParen,    ")");
            case '{': return make(TokenType::LBrace,    "{");
            case '}': return make(TokenType::RBrace,    "}");

            default:
                return make(TokenType::Invalid, std::string(1, c));
        }
    }
};

// ─────────────────────────────────────────────
//  Utilities
// ─────────────────────────────────────────────

static std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open file: " + path);

    std::ostringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

// ─────────────────────────────────────────────
//  Entry point
// ─────────────────────────────────────────────

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Improper Usage. Please use: lexer <source-file-path>\n";
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
    for (const auto& tok : lexer.tokenize()) {
        std::cout << "[line " << tok.line << "] "
                  << tokenTypeName(tok.type)
                  << ": " << tok.text << '\n';
    }

    return 0;
}