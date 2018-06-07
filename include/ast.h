/*******************************************************************************
 * Name            : regex.h
 * Project         : fcal
 * Module          : ast
 * Description     : Header file for AST
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_AST_H_
#define PROJECT_INCLUDE_AST_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <iostream>
#include <string>
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace ast {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/*! This is the base class for the Stmts, Stmt, Decl, and Expr classes.
This class is purely virtual.
*/
class Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Node() {}
};

/*******************************************************************************
 * Abstract Class Definitions
 ******************************************************************************/
/*! The following classes represent the terminal types from the provided FCAL
grammar. These classes are purely virtual.  */
class Stmts:public Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Stmts() {}
};

class Stmt:public Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Stmt() {}
};

class Decl:public Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Decl() {}
};

class Expr:public Node {
 public:
  virtual std::string unparse() = 0;
  virtual std::string CppCode() = 0;
  virtual ~Expr() {}
};

/*******************************************************************************
 * Program Production
 ******************************************************************************/
/*! The following Program class provides a constructor for the Program syntax
 provided in the FCAL grammar. This is a concrete class.
*/
class Program:public Node {
 public:
  Program(std::string v, Stmts * s) { v_ = v; s_ = s; }
  ~Program() { delete s_; }
  std::string unparse()
    { return v_ + "(" + ")" + "{" + s_->unparse() + "}"; }
  std::string CppCode()
    { return "#include <iostream>\n"
    "#include \"include/Matrix.h\"\n"
    "#include <math.h>\n"
    "using namespace std;\n"
    "int " + v_ + " ( ) {\n" +
    s_->CppCode() +
    "}"; }
 private:
  std::string v_;
  Stmts * s_;
};

/*******************************************************************************
 * Stmts Productions
 ******************************************************************************/
/*! The following classes directly inherit from the Stmts virtual class. These
classes offer means to deconstruct sequences of individual statements as
 defined in the FCAL grammar.
*/
class EmptyStmts:public Stmts {
 public:
  EmptyStmts() {}
  ~EmptyStmts() {}
  std::string unparse() { return ""; }
  std::string CppCode() { return ""; }
};

class SeqStmts:public Stmts {
 public:
  SeqStmts(Stmt * s1, Stmts * s2) { s1_ = s1; s2_ = s2; }
  ~SeqStmts() { delete s1_; delete s2_; }
  std::string unparse() { return s1_->unparse() + s2_->unparse(); }
  std::string CppCode() { return s1_->CppCode() + s2_->CppCode(); }
 private:
  Stmt * s1_;
  Stmts * s2_;
};

/*******************************************************************************
 * Stmt Productions
 ******************************************************************************/
/*! The following classes directly inherit from the virtual Stmt class.
These classes are used to construct the individual statements defined in the
FCAL grammar.
*/
class DeclStmt:public Stmt {
 public:
  explicit DeclStmt(Decl * d) { d_ = d; }
  ~DeclStmt() { delete d_; }
  std::string unparse() { return d_->unparse(); }
  std::string CppCode() { return d_->CppCode(); }
 private:
  Decl * d_;
};

class BracketsStmt:public Stmt {
 public:
  explicit BracketsStmt(Stmts * s) { s_ = s; }
  ~BracketsStmt() { delete s_; }
  std::string unparse() { return "{" + s_->unparse() + "}"; }
  std::string CppCode() { return "{\n" + s_->CppCode() + "}\n"; }
 private:
  Stmts * s_;
};

class IfStmt:public Stmt {
 public:
  IfStmt(Expr * e, Stmt * s) { e_ = e; s_ = s; }
  ~IfStmt() { delete e_; delete s_; }
  std::string unparse()
    { return "if(" + e_->unparse() + ")" + s_->unparse(); }
  std::string CppCode()
    { return "if ( " + e_->CppCode() + " )\n" + s_->CppCode(); }
 private:
  Expr * e_;
  Stmt * s_;
};

class IfElseStmt:public Stmt {
 public:
  IfElseStmt(Expr * e, Stmt * s1, Stmt * s2) { e_ = e; s1_ = s1; s2_ = s2; }
  ~IfElseStmt() { delete e_; delete s1_; delete s2_; }
  std::string unparse()
    { return "if(" + e_->unparse() + ")" + s1_->unparse() + " else " +
    s2_->unparse(); }
  std::string CppCode()
    { return "if ( " + e_->CppCode() + " )\n" + s1_->CppCode() +
    "\nelse\n" + s2_->CppCode(); }
 private:
  Expr * e_;
  Stmt * s1_;
  Stmt * s2_;
};

class AssignStmt:public Stmt {
 public:
  AssignStmt(std::string v, Expr * e) { v_ = v; e_ = e; }
  ~AssignStmt() { delete e_; }
  std::string unparse() { return v_ + "=" + e_->unparse() + ";"; }
  std::string CppCode() { return v_ + " = " + e_->CppCode() + ";\n"; }
 private:
  std::string v_;
  Expr * e_;
};

class MatrixAssignStmt:public Stmt {
 public:
  MatrixAssignStmt(std::string v, Expr * e1, Expr * e2, Expr * e3)
    { v_ = v; e1_ = e1; e2_ = e2; e3_ = e3; }
  ~MatrixAssignStmt() { delete e1_; delete e2_; delete e3_; }
  std::string unparse()
    { return v_ + "[" + e1_->unparse() + ":" + e2_->unparse() + "]" + "=" +
    e3_->unparse() + ";"; }
  std::string CppCode()
    { return "*(" + v_ + ".access(" + e1_->CppCode() + ", " + e2_->CppCode() +
    ")) = " + e3_->CppCode() + ";\n"; }
 private:
  std::string v_;
  Expr * e1_;
  Expr * e2_;
  Expr * e3_;
};

class PrintStmt:public Stmt {
 public:
  explicit PrintStmt(Expr * e) { e_ = e; }
  ~PrintStmt() { delete e_; }
  std::string unparse()
    { return "print(" + e_->unparse() + ");"; }
  std::string CppCode()
    { return "cout << " + e_->CppCode() + ";\n"; }
 private:
  Expr * e_;
};

class RepeatStmt:public Stmt {
 public:
  RepeatStmt(std::string v, Expr * e1, Expr * e2, Stmt * s)
    { v_ = v; e1_ = e1; e2_ = e2; s_ = s; }
  ~RepeatStmt() { delete e1_; delete e2_; delete s_; }
  std::string unparse()
    { return "repeat(" + v_ + " = " + e1_->unparse() + " to " +
    e2_->unparse() + ") " + s_->unparse(); }
  std::string CppCode()
    { return "for (" + v_ + " = " + e1_->CppCode() + "; " + v_ + " <= " +
    e2_->CppCode() + "; " + v_ + "++)\n" + s_->CppCode(); }
 private:
  std::string v_;
  Expr * e1_;
  Expr * e2_;
  Stmt * s_;
};

class WhileStmt:public Stmt {
 public:
  WhileStmt(Expr * e, Stmt * s) { e_ = e; s_ = s; }
  ~WhileStmt() { delete e_; delete s_; }
  std::string unparse()
    { return "while(" + e_->unparse() + ")" + s_->unparse(); }
  std::string CppCode()
    { return "while (" + e_->CppCode() + ")\n" + s_->CppCode(); }
 private:
  Expr * e_;
  Stmt * s_;
};

class SemiColonStmt:public Stmt {
 public:
  SemiColonStmt() {}
  ~SemiColonStmt() {}
  std::string unparse() { return ";"; }
  std::string CppCode() { return ";"; }
};

/*******************************************************************************
 * Decl Productions
 ******************************************************************************/
/*! The following classes directly inherit from the virtual Decl class. These
classes provide constructors for declarations based on the FCAL grammar.
*/
class IntDecl : public Decl {
 public:
  explicit IntDecl(std::string s) { s_ = s; }
  ~IntDecl() {}
  std::string unparse() { return "int " + s_ + ";"; }
  std::string CppCode() { return "int " + s_ + ";\n"; }
 private:
  std::string s_;
};

class FloatDecl : public Decl {
 public:
  explicit FloatDecl(std::string s) { s_ = s; }
  ~FloatDecl() {}
  std::string unparse() { return "float " + s_ + ";"; }
  std::string CppCode() { return "float " + s_ + ";\n"; }
 private:
  std::string s_;
};

class StringDecl : public Decl {
 public:
  explicit StringDecl(std::string s) { s_ = s; }
  ~StringDecl() {}
  std::string unparse() { return "string " + s_ + ";"; }
  std::string CppCode() { return "string " + s_ + ";\n"; }

 private:
  std::string s_;
};

class BooleanDecl : public Decl {
 public:
  explicit BooleanDecl(std::string s) { s_ = s; }
  ~BooleanDecl() {}
  std::string unparse() { return "boolean " + s_ + ";"; }
  std::string CppCode() { return "bool " + s_ + ";\n"; }

 private:
  std::string s_;
};

class MatrixMultiExprDecl : public Decl {
 public:
  MatrixMultiExprDecl(std::string s1, std::string s2, std::string s3,
    Expr * e1, Expr * e2, Expr * e3)
    { s1_ = s1; s2_ = s2; s3_ = s3; e1_ = e1; e2_ = e2; e3_ = e3; }
  ~MatrixMultiExprDecl() { delete e1_; delete e2_; delete e3_; }
  std::string unparse()
    { return "matrix " + s1_ + "[" + e1_->unparse() + ":" + e2_->unparse() + "]"
    + s2_ + " : " + s3_ + " = " + e3_->unparse() + ';'; }

  std::string CppCode()
    { return "matrix " + s1_ + "(" + e1_->CppCode() + ", " + e2_->CppCode() +
    ");\n" + "for (int " + s2_ + " = 0; " + s2_ + " < " + e1_->CppCode() + "; "
    + s2_ + "++) {\nfor (int " + s3_ + " = 0; " + s3_ + " < " + e2_->CppCode() +
    "; " + s3_ + "++) {\n*(" + s1_ + ".access(" + s2_ + ", " + s3_ + ")) = " +
    e3_->CppCode() + ";\n}\n}\n"; }
    
 private:
  std::string s1_;
  std::string s2_;
  std::string s3_;
  Expr * e1_;
  Expr * e2_;
  Expr * e3_;
};

class MatrixSingleExprDecl : public Decl {
 public:
  MatrixSingleExprDecl(std::string s1, Expr * e1) { s1_ = s1; e1_ = e1; }
  ~MatrixSingleExprDecl() { delete e1_; }
  std::string unparse() { return "matrix " + s1_ + " = " + e1_->unparse() +
    ";"; }
  std::string CppCode() { return "matrix " + s1_ + " = " + e1_->CppCode() +
    ";\n"; }
 private:
  std::string s1_;
  Expr * e1_;
};

/*******************************************************************************
 * Expr Productions
 ******************************************************************************/
/*! The following classes directly inherit from the virtual Expr class. These
classes provide constructors for the expressions as defined in the FCAL grammar.
*/
class VarNameExpr : public Expr {
 public:
  explicit VarNameExpr(std::string v) { v_ = v; }
  ~VarNameExpr() {}
  std::string unparse() { return v_; }
  std::string CppCode() { return v_; }
 private:
  std::string v_;
};

class IntegerConstExpr : public Expr {
 public:
  explicit IntegerConstExpr(std::string i) { i_ = i; }
  ~IntegerConstExpr() {}
  std::string unparse() { return i_; }
  std::string CppCode() { return i_; }
 private:
  std::string i_;
};

class FloatConstExpr : public Expr {
 public:
  explicit FloatConstExpr(std::string f) { f_ = f; }
  ~FloatConstExpr() {}
  std::string unparse() { return f_; }
  std::string CppCode() { return f_; }
 private:
  std::string f_;
};

class StringConstExpr : public Expr {
 public:
  explicit StringConstExpr(std::string s) { s_ = s; }
  ~StringConstExpr() {}
  std::string unparse() { return s_; }
  std::string CppCode() { return s_; }
 private:
  std::string s_;
};

class BooleanConstExpr : public Expr {
 public:
  explicit BooleanConstExpr(std::string b) { b_ = b; }
  ~BooleanConstExpr() {}
  std::string unparse() { return b_; }
  std::string CppCode() { return b_; }
 private:
  std::string b_;
};

// Calculation Expressions

class MultiplyExpr : public Expr {
 public:
  MultiplyExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~MultiplyExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " * " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " * " + e2_->CppCode(); }

 private:
  Expr * e1_;
  Expr * e2_;
};

class DivideExpr : public Expr {
 public:
  DivideExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~DivideExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " / " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " / " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class AddExpr : public Expr {
 public:
  AddExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~AddExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " + " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " + " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class SubtractExpr : public Expr {
 public:
  SubtractExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~SubtractExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " - " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " - " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class GreaterExpr : public Expr {
 public:
  GreaterExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~GreaterExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " > " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " > " + e2_->CppCode(); }

 private:
  Expr * e1_;
  Expr * e2_;
};

class GreaterEqualExpr : public Expr {
 public:
  GreaterEqualExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~GreaterEqualExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " >= " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " >= " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class LessExpr : public Expr {
 public:
  LessExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~LessExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " < " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " < " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class LessEqualExpr : public Expr {
 public:
  LessEqualExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~LessEqualExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " <= " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " <= " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class EqualsEqualsExpr : public Expr {
 public:
  EqualsEqualsExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~EqualsEqualsExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " == " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " == " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class NotEqualsExpr : public Expr {
 public:
  NotEqualsExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~NotEqualsExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " != " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " != " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class LogicalAndExpr : public Expr {
 public:
  LogicalAndExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~LogicalAndExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " && " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " && " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class LogicalOrExpr : public Expr {
 public:
  LogicalOrExpr(Expr * e1, Expr * e2) { e1_ = e1; e2_ = e2; }
  ~LogicalOrExpr() { delete e1_; delete e2_; }
  std::string unparse() { return e1_->unparse() + " || " + e2_->unparse(); }
  std::string CppCode() { return e1_->CppCode() + " || " + e2_->CppCode(); }
 private:
  Expr * e1_;
  Expr * e2_;
};

class MatrixREFExpr:public Expr {
 public:
  MatrixREFExpr(std::string v, Expr * e1, Expr * e2)
    { v_ = v; e1_ = e1; e2_ = e2; }
  ~MatrixREFExpr() { delete e1_; delete e2_; }
  std::string unparse()
    { return v_ + "[" + e1_->unparse() + " : " + e2_->unparse() + "]"; }
  std::string CppCode()
    { return "*(" + v_ + ".access(" + e1_->CppCode() + ", " + e2_->CppCode() + 
    "))"; }
 private:
  std::string v_;
  Expr * e1_;
  Expr * e2_;
};

class NestedOrFunctionCallExpr:public Expr {
 public:
  NestedOrFunctionCallExpr(std::string v, Expr * e) { v_ = v; e_ = e; }
  ~NestedOrFunctionCallExpr() { delete e_; }
  std::string unparse() { return v_ + "(" + e_->unparse() + ")"; }
  std::string CppCode() 
  { if (v_ == "n_rows" || v_ == "n_cols") {
      return e_->CppCode() + "." + v_ + "()";
    } else if (v_ == "matrix_read") {
      return "matrix::" + v_ + "( " + e_->CppCode() + " )";
    } else {
      return v_ + "( " + e_->CppCode() + " )";
    } }
 private:
  std::string v_;
  Expr * e_;
};

class ParenthesesExpr:public Expr {
 public:
  explicit ParenthesesExpr(Expr * e) { e_ = e; }
  ~ParenthesesExpr() { delete e_; }
  std::string unparse() { return "(" + e_->unparse() + ")"; }
  std::string CppCode() { return "( " + e_->CppCode() + " )"; }
 private:
  Expr * e_;
};

class LetExpr:public Expr {
 public:
  LetExpr(Stmts * s, Expr * e) { s_ = s; e_ = e; }
  ~LetExpr() { delete s_; delete e_; }
  std::string unparse()
    { return "let " + s_->unparse() + " in " + e_->unparse() + " end"; }
  std::string CppCode()
    { return "({ " + s_->CppCode() + e_->CppCode() + ";\n})"; }
 private:
  Stmts * s_;
  Expr * e_;
};

class IfExpr:public Expr {
 public:
  IfExpr(Expr * e1, Expr * e2, Expr * e3) { e1_ = e1; e2_ = e2; e3_ = e3; }
  ~IfExpr() { delete e1_; delete e2_; delete e3_; }
  std::string unparse()
    { return "if " + e1_->unparse() + " then " + e2_->unparse() + " else " +
    e3_->unparse(); }
  std::string CppCode()
    { return "((" + e1_->CppCode() + ") ? (" + e2_->CppCode() + ") : " +
    e3_->CppCode() + " )"; }
 private:
  Expr * e1_;
  Expr * e2_;
  Expr * e3_;
};

class NotExpr:public Expr {
 public:
  explicit NotExpr(Expr * e) { e_ = e; }
  ~NotExpr() { delete e_; }
  std::string unparse() { return "!" + e_->unparse(); }
  std::string CppCode() { return "!" + e_->CppCode(); }
 private:
  Expr * e_;
};

} /* namespace ast */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_AST_H_
