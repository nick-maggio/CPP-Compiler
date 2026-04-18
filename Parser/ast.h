#pragma once
#include <iostream>
#include <memory>

// One struct per supported language construct



struct ASTNode { virtual ~ASTNode() = default; };

struct NumberLit : ASTNode {
  double value;
};

struct Identifier : ASTNode {
  std::string value;
};

struct BinaryExpr : ASTNode {
  std::string op;
  std::unique_ptr<ASTNode> left, right;
};

struct UnaryExpr : ASTNode {
  std::string op;
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

struct WhileStmt : ASTNode {
  std::unique_ptr<ASTNode> cond, body;
};

struct ForStmt : ASTNode {
  std::unique_ptr<ASTNode> init, cond, step, body;
};

struct Block : ASTNode {
  std::vector<std::unique_ptr<ASTNode>> stmts;
};

struct ReturnStmt : ASTNode {
  std::unique_ptr<ASTNode> value;
};