#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "../Parser/ast.h"

enum class SymbolKind {Variable, Function};

struct Symbol {
  std::string name;
  std::string type;
  SymbolKind  kind;
  size_t      line;
  std::vector<std::string> paramTypes;
};

class Scope {
public:
  bool define(const Symbol& sym) {
    if (table.count(sym.name)) return false;
    table[sym.name] = sym;
    return true;
  }

  const Symbol* lookup(const std::string& name) const {
    auto it = table.find(name);
    if (it != table.end()) return &it->second;
    return nullptr;
  }

private:
  std::unordered_map<std::string, Symbol> table;
};

class ScopeStack {
public:
  void pushScope() { scopes.emplace_back(); }
  void popScope()  { scopes.pop_back(); }

  bool define(const Symbol& sym) {
    return scopes.back().define(sym);
  }

  const Symbol* lookup(const std::string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
      auto result = it->lookup(name);
      if (result) return result;
    }
    return nullptr;
  }

private:
  std::vector<Scope> scopes;
};

struct SemanticError {
  std::string message;
  size_t      line;
};

class SemanticAnalyzer {
public:
  bool analyze(const ASTNode* root);
  const std::vector<SemanticError>& errors() const { return errs; }

private:
  ScopeStack                 scopes;
  std::vector<SemanticError> errs;
  std::string                currentReturnType;
  int                        loopDepth = 0;

  std::string  analyzeExpr(const ASTNode* node);
  void         analyzeStmt(const ASTNode* node);
  void         analyzeBlock(const Block* node);

  void         error(const std::string& msg, size_t line = 0);
  bool         isNumeric(const std::string& t) const;
  bool         typesCompatible(const std::string& a, const std::string& b) const;
  std::string  widenNumeric(const std::string& a, const std::string& b) const;
};