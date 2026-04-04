#pragma once

// ────────────────────────────────────────────
//  Token Types
// ────────────────────────────────────────────


enum class TokenType {
    // Literals
    Identifier,
    Number,
    StringLiteral,

    // Keywords
    KeywordInt,
    KeywordDouble,
    KeywordString,
    KeywordIf,
    KeywordElse,
    KeywordWhile,
    KeywordDo,
    KeywordFor,
    KeywordSwitch,
    KeywordCase,
    KeywordDefault,
    KeywordBreak,
    KeywordContinue,
    KeywordCin,
    KeywordCout,
    KeywordReturn,
    KeywordInclude,
    KeywordUsing,
    KeywordNamespace,


    // Operators
    Equals,          // =
    ComprEquals,     // ==
    NotEquals,       // !=
    Plus,            // +
    Minus,           // -
    Multiply,        // *
    Divide,          // /
    Modulo,          // %
    Negate,          // !
    LT,              // <
    GT,              // >
    LTE,             // <=
    GTE,             // >=
    InsertionOp,     // <<
    ExtractionOp,    // >>
    Colon,           // :
    Increment,       // ++
    Decrement,       // --

    // Delimiters
    Semicolon,       // ;
    LParen,          // (
    RParen,          // )
    LBrace,          // {
    RBrace,          // }
    Comma,           // ,

    EndOfFile,
    Invalid
};

static const std::unordered_map<TokenType, std::string> TOKEN_NAMES = {
    { TokenType::Identifier,    "Identifier"    },
    { TokenType::Number,        "Number"        },
    { TokenType::StringLiteral, "StringLiteral" },
    { TokenType::KeywordInt,    "KeywordInt"    },
    { TokenType::KeywordDouble, "KeywordDouble" },
    { TokenType::KeywordString, "KeywordString" },
    { TokenType::KeywordIf,     "KeywordIf"     },
    { TokenType::KeywordElse,   "KeywordElse"   },
    { TokenType::KeywordWhile,  "KeywordWhile"  },
    { TokenType::KeywordDo,     "KeywordDo"     },
    { TokenType::KeywordFor,    "KeywordFor"    },
    { TokenType::KeywordSwitch, "KeywordSwitch" },
    { TokenType::KeywordCase,   "KeywordCase"   },
    { TokenType::KeywordDefault, "KeywordDefault" },
    { TokenType::KeywordBreak,  "KeywordBreak"  },
    { TokenType::KeywordContinue, "KeywordContinue" },
    { TokenType::KeywordCin,    "KeywordCin"    },
    { TokenType::KeywordCout,   "KeywordCout"   },
    { TokenType::KeywordReturn,  "KeywordReturn" },
    { TokenType::KeywordInclude, "KeywordInclude" },
    { TokenType::KeywordUsing,   "KeywordUsing"   },
    { TokenType::KeywordNamespace, "KeywordNamespace" },
    { TokenType::Equals,        "Equals"        },
    { TokenType::ComprEquals,   "ComprEquals"   },
    { TokenType::NotEquals,     "NotEquals"     },
    { TokenType::Plus,          "Plus"          },
    { TokenType::Minus,         "Minus"         },
    { TokenType::Multiply,      "Asterisk"      },
    { TokenType::Divide,         "Slash"        },
    { TokenType::Modulo,         "Modulo"        },
    { TokenType::Negate,        "Negate"        },
    { TokenType::LT,            "LT"            },
    { TokenType::GT,            "GT"            },
    { TokenType::LTE,           "LTE"           },
    { TokenType::GTE,           "GTE"           },
    { TokenType::InsertionOp,   "InsertionOp"   },
    { TokenType::ExtractionOp,  "ExtractionOp"  },
    { TokenType::Colon,         "Colon"         },
    { TokenType::Increment,     "Increment"     },
    { TokenType::Decrement,     "Decrement"     },
    { TokenType::Semicolon,     "Semicolon"     },
    { TokenType::LParen,        "LParen"        },
    { TokenType::RParen,        "RParen"        },
    { TokenType::LBrace,        "LBrace"        },
    { TokenType::RBrace,        "RBrace"        },
    { TokenType::Comma,         "Comma"         },
    { TokenType::EndOfFile,     "EndOfFile"     },
    { TokenType::Invalid,       "Invalid"       },
};

static const std::unordered_map<std::string, TokenType> KEYWORDS = {
    { "int",  TokenType::KeywordInt  },
    { "double", TokenType::KeywordDouble },
    { "string", TokenType::KeywordString },
    { "if",   TokenType::KeywordIf   },
    { "else", TokenType::KeywordElse },
    { "while", TokenType::KeywordWhile },
    { "do",    TokenType::KeywordDo    },
    { "for",   TokenType::KeywordFor   },
    { "switch", TokenType::KeywordSwitch },
    { "case",   TokenType::KeywordCase   },
    { "default", TokenType::KeywordDefault },
    { "break",  TokenType::KeywordBreak  },
    { "continue", TokenType::KeywordContinue },
    { "cin",   TokenType::KeywordCin   },
    { "cout", TokenType::KeywordCout },
    { "return", TokenType::KeywordReturn },
    { "#include", TokenType::KeywordInclude },
    { "using", TokenType::KeywordUsing },
    { "namespace", TokenType::KeywordNamespace },
};

inline std::string tokenTypeName(TokenType type) {
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