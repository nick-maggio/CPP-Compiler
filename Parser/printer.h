#pragma once
#include <iostream>
#include <string>
#include "ast.h"

static void printNode(const ASTNode* node, int indent = 0);

static std::string indentStr(int indent) {
  return std::string(indent * 2, ' ');
}

static void printNumberLit(const NumberLit* node, int indent) {
  std::cout << "-" << indentStr(indent) << "↳ Number (" << node->value << ")\n";
}

static void printStringLit(const StringLit* node, int indent) {
  std::cout << "-" << indentStr(indent) << "↳ String (\"" << node->value << "\")\n";
}

static void printIdentifier(const Identifier* node, int indent) {
  std::cout << "-" << indentStr(indent) << "↳ Variable (" << node->value << ")\n";
}

static void printAssignExpr(const AssignExpr* node, int indent) {
  std::cout << indentStr(indent) << "AssignExpr (" << node->name << ")\n";
  std::cout << indentStr(indent + 1) << "↳ value:\n";
  printNode(node->value.get(), indent + 2);
}

static void printUnaryExpr(const UnaryExpr* node, int indent) {
  std::cout << "-" << indentStr(indent) << "Operation (" << node->operation << ")\n";
  printNode(node->operand.get(), indent + 2);
}

static void printBinaryExpr(const BinaryExpr* node, int indent) {
  std::cout << "-" << indentStr(indent) << "Operation (" << node->op << ")\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ left:\n";
  printNode(node->left.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ right:\n";
  printNode(node->right.get(), indent + 2);
}

static void printVarDecl(const VarDecl* node, int indent) {
  std::cout << "-" << indentStr(indent) << "Variable Declared: (" << node->type << " " << node->name << ")\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ initialized as:\n";
  printNode(node->init.get(), indent + 2);
}

static void printIfStmt(const IfStmt* node, int indent) {
  std::cout << "-" << indentStr(indent) << "If Statement\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ condition:\n";
  printNode(node->cond.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ then:\n";
  printNode(node->thenDo.get(), indent + 2);
  if (node->elseDo) {
    std::cout << "-" << indentStr(indent + 1) << "↳ else:\n";
    printNode(node->elseDo.get(), indent + 2);
  }
}

static void printWhileStmt(const WhileLoop* node, int indent) {
  std::cout << "-" << indentStr(indent) << "While Loop\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ cond:\n";
  printNode(node->cond.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ body:\n";
  printNode(node->body.get(), indent + 2);
}

static void printForStmt(const ForLoop* node, int indent) {
  std::cout << "-" << indentStr(indent) << "ForStmt\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ initialize:\n";
  printNode(node->init.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ condition:\n";
  printNode(node->cond.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ step:\n";
  printNode(node->step.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ body:\n";
  printNode(node->body.get(), indent + 2);
}

static void printDoWhileStmt(const DoWhileLoop* node, int indent) {
  std::cout << "-" << indentStr(indent) << "DoWhileLoop\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ body:\n";
  printNode(node->body.get(), indent + 2);
  std::cout << "-" << indentStr(indent + 1) << "↳ cond:\n";
  printNode(node->cond.get(), indent + 2);
}

static void printCaseStmt(const CaseStmt* node, int indent) {
  if (node->value) {
    std::cout << "-" << indentStr(indent) << "↳ Case:\n";
  } else {
    std::cout << "-" << indentStr(indent) << "↳ Default:\n";
  }

  if (node->value) {
    printNode(node->value.get(), indent + 1);
  }
  for (const auto& s : node->stmts) {
    printNode(s.get(), indent + 1);
  }
}

static void printSwitchStmt(const SwitchStmt* node, int indent) {
  std::cout << "-" << indentStr(indent) << "SwitchStmt\n";
  std::cout << "-" << indentStr(indent + 1) << "expr:\n";
  printNode(node->expr.get(), indent + 2);
  for (const auto& c : node->cases) {
    printNode(c.get(), indent + 1);
  }
}

static void printBreakStmt(const BreakStmt*, int indent) {
  std::cout << "-" << indentStr(indent) << "Break\n";
}

static void printContinueStmt(const ContinueStmt*, int indent) {
  std::cout << "-" << indentStr(indent) << "Continue\n";
}

static void printCoutStmt(const CoutStmt* node, int indent) {
  std::cout << "-" << indentStr(indent) << "CoutStmt\n";
  for (const auto& arg : node->args) {
    printNode(arg.get(), indent + 1);
  }
}

static void printParamDecl(const ParamDecl* node, int indent) {
  std::cout << "-" << indentStr(indent) << "ParamDecl (" << node->type << " " << node->name << ")\n";
}

static void printFunctionDecl(const FunctionDecl* node, int indent) {
  std::cout << "-" << indentStr(indent) << "FunctionDecl (" << node->returnType << " " << node->name << ")\n";
  if (!node->params.empty()) {
    std::cout << "-" << indentStr(indent + 1) << "↳ params:\n";
    for (const auto& p : node->params) {
      printNode(p.get(), indent + 2);
    }
  }
  std::cout << "-" << indentStr(indent + 1) << "↳ body:\n";
  printNode(node->body.get(), indent + 2);
}

static void printReturnStmt(const ReturnStmt* node, int indent) {
  std::cout << "-" << indentStr(indent) << "Return\n";
  std::cout << "-" << indentStr(indent + 1) << "↳ value:\n";
  printNode(node->value.get(), indent + 2);
}

static void printBlock(const Block* node, int indent) {
  std::cout << "-" << indentStr(indent) << "Block\n";
  for (const auto& stmt : node->stmts) {
    printNode(stmt.get(), indent + 1);
  }
}

static void printNode(const ASTNode* node, int indent) {
  if (!node) {
    std::cout << "-" << indentStr(indent) << "(null)\n";
  }

  if (auto* n = dynamic_cast<const NumberLit*>(node)) {
    printNumberLit(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const StringLit*>(node)) {
    printStringLit(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const Identifier*>(node)) {
    printIdentifier(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const BinaryExpr*>(node)) {
    printBinaryExpr(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const UnaryExpr*>(node)) {
    printUnaryExpr(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const VarDecl*>(node)) {
    printVarDecl(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const IfStmt*>(node)) {
    printIfStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const AssignExpr*>(node)) {
    printAssignExpr(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const WhileLoop*>(node)) {
    printWhileStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const ForLoop*>(node)) {
    printForStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const DoWhileLoop*>(node)) {
    printDoWhileStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const SwitchStmt*>(node)) {
    printSwitchStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const CaseStmt*>(node)) {
    printCaseStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const BreakStmt*>(node)) {
    printBreakStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const ContinueStmt*>(node)) {
    printContinueStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const CoutStmt*>(node)) {
    printCoutStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const FunctionDecl*>(node)) {
    printFunctionDecl(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const ParamDecl*>(node)) {
    printParamDecl(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const ReturnStmt*>(node)) {
    printReturnStmt(n, indent);
    return;
  }
  if (auto* n = dynamic_cast<const Block*>(node)) {
    printBlock(n, indent);
    return;
  }

  std::cout << indentStr(indent) << "(unknown node)\n";
}
