#pragma once
#include <iostream>
#include <memory>

// AST nodes for the currently supported C++ subset.
struct ASTNode {
  virtual ~ASTNode() = default;
};

struct NumberLit : ASTNode {
  double value;
};

struct StringLit : ASTNode {
  std::string value;
};

struct Identifier : ASTNode {
  std::string value;
};

struct BinaryExpr : ASTNode {
  std::string op;
  std::unique_ptr<ASTNode> left, right;
};

struct UnaryExpr : ASTNode {
  std::string operation;
  std::unique_ptr<ASTNode> operand;
};

struct VarDecl : ASTNode {
  std::string type, name;
  std::unique_ptr<ASTNode> init;
};

struct AssignExpr : ASTNode {
  std::string name;
  std::unique_ptr<ASTNode> value;
};

struct IfStmt : ASTNode {
  std::unique_ptr<ASTNode> cond, thenDo, elseDo;
};

struct WhileLoop : ASTNode {
  std::unique_ptr<ASTNode> cond, body;
};

struct DoWhileLoop : ASTNode {
  std::unique_ptr<ASTNode> body, cond;
};

struct ForLoop : ASTNode {
  std::unique_ptr<ASTNode> init, cond, step, body;
};

struct CaseStmt : ASTNode {
  std::unique_ptr<ASTNode> value;
  std::vector<std::unique_ptr<ASTNode>> stmts;
};

struct SwitchStmt : ASTNode {
  std::unique_ptr<ASTNode> expr;
  std::vector<std::unique_ptr<ASTNode>> cases;
};

struct BreakStmt    : ASTNode {};
struct ContinueStmt : ASTNode {};

struct CoutStmt : ASTNode {
  std::vector<std::unique_ptr<ASTNode>> args;
};

struct ParamDecl : ASTNode {
  std::string type, name;
};

struct FunctionDecl : ASTNode {
  std::string returnType, name;
  std::vector<std::unique_ptr<ASTNode>> params;
  std::unique_ptr<ASTNode> body;
};

struct Block : ASTNode {
  std::vector<std::unique_ptr<ASTNode>> stmts;
};

struct ReturnStmt : ASTNode {
  std::unique_ptr<ASTNode> value;
};