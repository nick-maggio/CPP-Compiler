#include "semantic.h"

void SemanticAnalyzer::error(const std::string& msg, size_t line) {
  errs.push_back({msg, line});
}

bool SemanticAnalyzer::isNumeric(const std::string& t) const {
  return t == "int" || t == "double";
}

bool SemanticAnalyzer::typesCompatible(const std::string& a, const std::string& b) const {
  if (a == b) return true;
  if (isNumeric(a) && isNumeric(b)) return true;
  return false;
}

std::string SemanticAnalyzer::widenNumeric(const std::string& a, const std::string& b) const {
  if (a == "double" || b == "double") return "double";
  return "int";
}

bool SemanticAnalyzer::analyze(const ASTNode* root) {
  scopes.pushScope();
  analyzeStmt(root);
  scopes.popScope();
  return errs.empty();
}

std::string SemanticAnalyzer::analyzeExpr(const ASTNode* node) {
  if (!node) return "void";

  if (auto *n = dynamic_cast<const NumberLit*>(node)) {
    double v = n->value;
    return (v == static_cast<long long>(v)) ? "int" : "double";
  }

  if (dynamic_cast<const StringLit*>(node)) {
    return "string";
  }

  if (auto *n = dynamic_cast<const Identifier*>(node)) {
    auto sym = scopes.lookup(n->value);
    if (!sym) {
      error("Use of undeclared identifier '" + n->value + "'");
      return "int";
    }
    return sym->type;
  }

  if (auto *n = dynamic_cast<const UnaryExpr*>(node)) {
    std::string opType = analyzeExpr(n->operand.get());

    if (n->operation == "++_prefix"  || n->operation == "--_prefix" ||
        n->operation == "++_postfix" || n->operation == "--_postfix") {
      if (!isNumeric(opType))
          error("Increment/decrement requires a numeric operand, got '" + opType + "'");
      return opType;         
    }

    if (n->operation == "-_prefix") {
      if (!isNumeric(opType))
        error("Unary '-' requires a numeric operand, got '" + opType + "'");
      return opType;
    }

    if (n->operation == "!_prefix") {
      analyzeExpr(n->operand.get());
      return "int";
    }

    return opType;
  }

  if (auto *n = dynamic_cast<const BinaryExpr*>(node)) {
    std::string lType = analyzeExpr(n->left.get());
    std::string rType = analyzeExpr(n->right.get());

    if (lType == "string" || rType == "string") {
      if (n->op != "+")
        error("Operator '" + n->op + "' cannot be applied to string operands");
      if (lType != rType)
        error("Cannot mix 'string' with '" + (lType == "string" ? rType : lType) + "' using '+'");
      return "string";
    }

    if (!isNumeric(lType))
      error("Left operand of '" + n->op + "' has non-numeric type '" + lType + "'");
    if (!isNumeric(rType))
      error("Left operand of '" + n->op + "' has non-numeric type '" + rType + "'");
    
    if (n->op == "%" && (lType == "double" || rType == "double"))
      error("Operator '%' requires integer operands");

    if (n->op == "==" || n->op == "!=" || n->op == "<" ||
        n->op == ">"  || n->op == "<=" || n->op == ">=")
      return "int";

    return widenNumeric(lType, rType);
  }

  if (auto *n = dynamic_cast<const AssignExpr*>(node)) {
    auto sym = scopes.lookup(n->name);
    if (!sym) {
      error("Assignment to undeclared variable '" + n->name + "'");
      analyzeExpr(n->value.get());
      return "int";
    }

    std::string valType = analyzeExpr(n->value.get());
    if (!typesCompatible(sym->type, valType))
      error("Cannot assign '" + valType + "' to variable '" + n->name + "' of type '" + sym->type + "'");
    return sym->type;
  }

  error("Unknown expression");
  return "int";
}

void SemanticAnalyzer::analyzeStmt(const ASTNode *node) {
  if (!node) return;

  if (auto *n = dynamic_cast<const Block*>(node)) {
    analyzeBlock(n);
    return;
  }

  if (auto *n = dynamic_cast<const VarDecl*>(node)) {
    std::string initType = analyzeExpr(n->init.get());
    if (!typesCompatible(n->type, initType))
      error("Cannot initialize '" + n->name + "' of type '" + n->type + "' with expression of type '" + initType + "'");

    Symbol sym;
    sym.name = n->name;
    sym.type = n->type;
    sym.kind = SymbolKind::Variable;
    sym.line = 0;
    if (!scopes.define(sym))
      error("Redeclaration of variable '" + n->name + "'");
    return;
  }

  if (auto *n = dynamic_cast<const FunctionDecl*>(node)) {
    Symbol fnSym;
    fnSym.name = n->name;
    fnSym.type = n->returnType;
    fnSym.kind = SymbolKind::Function;
    fnSym.line = 0;
    for (const auto& p : n->params)
      if (auto *pd = dynamic_cast<const ParamDecl*>(p.get()))
        fnSym.paramTypes.push_back(pd->type);
    
    if (!scopes.define(fnSym))
      error("Redeclaration of function '" + n->name + "'");

    scopes.pushScope();
    std::string prevReturn = currentReturnType;
    currentReturnType = n->returnType;

    for (const auto& p : n->params) {
      if (auto *pd = dynamic_cast<const ParamDecl*>(p.get())) {
        Symbol paramSym;
        paramSym.name = pd->name;
        paramSym.type = pd->type;
        paramSym.kind = SymbolKind::Variable;
        if (!scopes.define(paramSym))
          error("Duplicate parameter '" + pd->name + "' in function '" + n->name + "'");        
      }
    }
    
    analyzeStmt(n->body.get());
    currentReturnType = prevReturn;
    scopes.popScope();
    return;
  }

  if (auto* n = dynamic_cast<const ReturnStmt*>(node)) {
    std::string valType = analyzeExpr(n->value.get());
    if (currentReturnType.empty())
      error("'return' statement outside of a function");
    else if (!typesCompatible(currentReturnType, valType))
      error("Return type mismatch: expected '" + currentReturnType + "', got '" + valType + "'");
    return;
  }

  if (auto* n = dynamic_cast<const IfStmt*>(node)) {
    analyzeExpr(n->cond.get());
    scopes.pushScope();
    analyzeStmt(n->thenDo.get());
    scopes.popScope();
    if (n->elseDo) {
      scopes.pushScope();
      analyzeStmt(n->elseDo.get());
      scopes.popScope();
    }
    return;
  }

  if (auto* n = dynamic_cast<const WhileLoop*>(node)) {
    analyzeExpr(n->cond.get());
    ++loopDepth;
    scopes.pushScope();
    analyzeStmt(n->body.get());
    scopes.popScope();
    --loopDepth;
    return;
  }

  if (auto* n = dynamic_cast<const DoWhileLoop*>(node)) {
    ++loopDepth;
    scopes.pushScope();
    analyzeStmt(n->body.get());
    scopes.popScope();
    --loopDepth;
    analyzeExpr(n->cond.get());
    return;
  }

  if (auto* n = dynamic_cast<const ForLoop*>(node)) {
    scopes.pushScope();
    analyzeStmt(n->init.get());
    analyzeExpr(n->cond.get());
    analyzeExpr(n->step.get());
    ++loopDepth;
    analyzeStmt(n->body.get());
    --loopDepth;
    scopes.popScope();
    return;
  }

  if (auto* n = dynamic_cast<const SwitchStmt*>(node)) {
    std::string exprType = analyzeExpr(n->expr.get());
    if (exprType != "int")
      error("Switch expression must be 'int', got '" + exprType + "'");

    ++loopDepth;  // break is valid inside switch
    for (const auto& c : n->cases) {
      if (auto* cs = dynamic_cast<const CaseStmt*>(c.get())) {
        if (cs->value) {
          std::string caseType = analyzeExpr(cs->value.get());
          if (caseType != "int")
            error("Case label must be an integer, got '" + caseType + "'");
        }
        scopes.pushScope();
        for (const auto& s : cs->stmts)
          analyzeStmt(s.get());
        scopes.popScope();
      }
    }
    --loopDepth;
    return;
  }

  if (dynamic_cast<const BreakStmt*>(node)) {
    if (loopDepth == 0) error("'break' used outside of loop or switch");
    return;
  }

  if (dynamic_cast<const ContinueStmt*>(node)) {
    if (loopDepth == 0) error("'continue' used outside of loop");
    return;
  }

  if (auto* n = dynamic_cast<const CoutStmt*>(node)) {
    for (const auto& arg : n->args)
      analyzeExpr(arg.get());
    return;
  }

  if (dynamic_cast<const AssignExpr*>(node) ||
      dynamic_cast<const UnaryExpr*>(node)  ||
      dynamic_cast<const BinaryExpr*>(node) ||
      dynamic_cast<const Identifier*>(node) ||
      dynamic_cast<const NumberLit*>(node)  ||
      dynamic_cast<const StringLit*>(node)) {
    analyzeExpr(node);
    return;
  }

  if (dynamic_cast<const ParamDecl*>(node)) return;  // handled inside FunctionDecl

  error("Unknown statement node during semantic analysis");
}

void SemanticAnalyzer::analyzeBlock(const Block *node) {
  scopes.pushScope();
  for (const auto& stmt : node->stmts)
    analyzeStmt(stmt.get());
  scopes.popScope();
}