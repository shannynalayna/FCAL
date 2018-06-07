/*******************************************************************************
 * Name            : parser.h
 * Project         : fcal
 * Module          : parser
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Erik Van Wyk
 * Modifications by: Dan Challou, John Harwell
 *
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "include/parser.h"
#include <assert.h>
#include <stdio.h>
#include "include/ext_token.h"
#include "include/scanner.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace parser {

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Parser::~Parser() {
  if (scanner_) delete scanner_;

  scanner::ExtToken *to_delete1;
  curr_token_ = tokens_;
  while (curr_token_) {
    to_delete1 = curr_token_;
    curr_token_ = curr_token_->next();
    delete to_delete1;
  } /* while() */

  scanner::Token *curr_scanner_token = stokens_;
  scanner::Token *to_delete2;
  while (curr_scanner_token) {
    to_delete2 = curr_scanner_token;
    curr_scanner_token = curr_scanner_token->next();
    delete to_delete2;
  } /* while() */
} /* Parser::~Parser() */

ParseResult Parser::Parse(const char *text) {
  assert(text != NULL);

  ParseResult pr;
  try {
  scanner_ = new scanner::Scanner();
  stokens_ = scanner_->Scan(text);
  tokens_ = tokens_->ExtendTokenList(this, stokens_);

  assert(tokens_ != NULL);
  curr_token_ = tokens_;
  pr = ParseProgram();
  }
  catch (std::string errMsg) {
    pr.ok(false);
    pr.errors(errMsg);
    pr.ast(NULL);
  }
  return pr;
} /* Parser::parse() */

/*
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

// Program
ParseResult Parser::ParseProgram() {
  ParseResult pr;
  // root
  // Program ::= varName '(' ')' '{' Stmts '}'
  match(scanner::kVariableName);
  std::string varName = prev_token_->lexeme();
  match(scanner::kLeftParen);
  match(scanner::kRightParen);
  match(scanner::kLeftCurly);
  ParseResult prStmts = parse_stmts();
  ast::Stmts * s = NULL;
  if (prStmts.ast()) {
    s = dynamic_cast<ast::Stmts *>(prStmts.ast());
    if (!s) throw((std::string) "Bad cast of State in ParseProgram");
  }
  match(scanner::kRightCurly);
  match(scanner::kEndOfFile);
  pr.ast(new ast::Program(varName, s));
  return pr;
} /* Parser::ParseProgram() */

// MatrixDecl
// identical purpose of parse_decl, handles special matrix syntax.
  ParseResult Parser::parse_matrix_decl() {
  ParseResult pr;
  match(scanner::kMatrixKwd);
  match(scanner::kVariableName);
  std::string varName1 = prev_token_->lexeme();

  // Decl ::= 'matrix' varName '[' Expr ':' Expr ']' varName ':' varName  '='
  // Expr ';'
  if (attempt_match(scanner::kLeftSquare)) {
    ParseResult prExpr1 = parse_expr(0);
    ast::Expr * e1 = NULL;
    if (prExpr1.ast()) {
      e1 = dynamic_cast<ast::Expr *>(prExpr1.ast());
      if (!e1) throw((std::string) "Bad cast of State in parse_matrix_decl");
    }
    match(scanner::kColon);
    ParseResult prExpr2 = parse_expr(0);
    ast::Expr * e2 = NULL;
    if (prExpr2.ast()) {
      e2 = dynamic_cast<ast::Expr *>(prExpr2.ast());
      if (!e2) throw((std::string) "Bad cast of State in parse_matrix_decl");
    }
    match(scanner::kRightSquare);
    match(scanner::kVariableName);
    std::string varName2 = prev_token_->lexeme();
    match(scanner::kColon);
    match(scanner::kVariableName);
    std::string varName3 = prev_token_->lexeme();
    match(scanner::kAssign);
    ParseResult prExpr3 = parse_expr(0);
    ast::Expr * e3 = NULL;
    if (prExpr3.ast()) {
      e3 = dynamic_cast<ast::Expr *>(prExpr3.ast());
      if (!e3) throw((std::string) "Bad cast of State in parse_matrix_decl");
    }
    pr.ast(new ast::MatrixMultiExprDecl(varName1, varName2, varName3, e1, e2,
      e3));
  } else if (attempt_match(scanner::kAssign)) {
    // Decl ::= 'matrix' varName '=' Expr ';'
    ParseResult prExpr = parse_expr(0);
    ast::Expr * e = NULL;
    if (prExpr.ast()) {
      e = dynamic_cast<ast::Expr *>(prExpr.ast());
      if (!e) throw((std::string) "Bad cast of State in parse_matrix_decl");
    }
    pr.ast(new ast::MatrixSingleExprDecl(varName1, e));
  } else {
    throw((std::string) "Bad Syntax of Matrix Decl in in parseMatrixDecl");
  }

  match(scanner::kSemiColon);

  return pr;
}
// standardDecl
// Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
ParseResult Parser::parse_standard_decl() {
  ParseResult pr;
  if (attempt_match(scanner::kIntKwd)) {  // Type ::= intKwd
    match(scanner::kVariableName);
    std::string varName = prev_token_->lexeme();
    match(scanner::kSemiColon);
    pr.ast(new ast::IntDecl(varName));
  } else if (attempt_match(scanner::kFloatKwd)) {  // Type ::= floatKwd
    match(scanner::kVariableName);
    std::string varName = prev_token_->lexeme();
    match(scanner::kSemiColon);
    pr.ast(new ast::FloatDecl(varName));
  } else if (attempt_match(scanner::kStringKwd)) {  // Type ::= stringKwd
    match(scanner::kVariableName);
    std::string varName = prev_token_->lexeme();
    match(scanner::kSemiColon);
    pr.ast(new ast::StringDecl(varName));
  } else if (attempt_match(scanner::kBoolKwd)) {  // Type ::= boolKwd
    match(scanner::kVariableName);
    std::string varName = prev_token_->lexeme();
    match(scanner::kSemiColon);
    pr.ast(new ast::BooleanDecl(varName));
  }
  return pr;
}

// Decl
ParseResult Parser::parse_decl() {
  ParseResult pr;
  // Decl :: matrix variableName ....
  if (next_is(scanner::kMatrixKwd)) {
    pr = parse_matrix_decl();
  } else {
    // Decl ::= Type variableName semiColon
    pr = parse_standard_decl();
  }
  return pr;
}

// Stmts
ParseResult Parser::parse_stmts() {
  ParseResult pr;
  if (!next_is(scanner::kRightCurly) && !next_is(scanner::kInKwd)) {
    // Stmts ::= Stmt Stmts
    ParseResult prStmt = parse_stmt();
    ast::Stmt * s1 = NULL;
    if (prStmt.ast()) {
      s1 = dynamic_cast<ast::Stmt *>(prStmt.ast());
      if (!s1) throw((std::string) "Bad cast of State in parse_stmts");
    }
    ParseResult prStmts = parse_stmts();
    ast::Stmts * s2 = NULL;
    if (prStmts.ast()) {
      s2 = dynamic_cast<ast::Stmts *>(prStmts.ast());
      if (!s2) throw((std::string) "Bad cast of State in parse_stmts");
    }
    pr.ast(new ast::SeqStmts(s1, s2));
  } else {
    // Stmts ::=
    // nothing to match.k
    pr.ast(new ast::EmptyStmts());
  }
  return pr;
}

// Stmt
ParseResult Parser::parse_stmt() {
  ParseResult pr;

  // Stmt ::= Decl
  if (next_is(scanner::kIntKwd) || next_is(scanner::kFloatKwd) ||
      next_is(scanner::kMatrixKwd) || next_is(scanner::kStringKwd) ||
      next_is(scanner::kBoolKwd)) {
    ParseResult prDecl = parse_decl();
    ast::Decl * d = NULL;
    if (prDecl.ast()) {
      d = dynamic_cast<ast::Decl *>(prDecl.ast());
      if (!d) throw((std::string) "Bad cast of State in parse_stmt");
    }
    pr.ast(new ast::DeclStmt(d));
  } else if (attempt_match(scanner::kLeftCurly)) {
    // Stmt ::= '{' Stmts '}'
    ParseResult prStmts = parse_stmts();
    ast::Stmts * s = NULL;
    if (prStmts.ast()) {
      s = dynamic_cast<ast::Stmts *>(prStmts.ast());
      if (!s) throw((std::string) "Bad cast of State in parse_stmt");
    }
    match(scanner::kRightCurly);
    pr.ast(new ast::BracketsStmt(s));
  } else if (attempt_match(scanner::kIfKwd)) {
    // Stmt ::= 'if' '(' Expr ')' Stmt
    // Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    match(scanner::kLeftParen);
    ParseResult prExpr = parse_expr(0);
    ast::Expr * e = NULL;
    if (prExpr.ast()) {
      e = dynamic_cast<ast::Expr *>(prExpr.ast());
      if (!e) throw((std::string) "Bad cast of State in parse_stmt");
    }
    match(scanner::kRightParen);
    ParseResult prStmt1 = parse_stmt();
    ast::Stmt * s1 = NULL;
    if (prStmt1.ast()) {
      s1 = dynamic_cast<ast::Stmt *>(prStmt1.ast());
      if (!s1) throw((std::string) "Bad cast of State in parse_stmt");
    }
    if (attempt_match(scanner::kElseKwd)) {
      ParseResult prStmt2 = parse_stmt();
      ast::Stmt * s2 = NULL;
      if (prStmt2.ast()) {
        s2 = dynamic_cast<ast::Stmt *>(prStmt2.ast());
        if (!s2) throw((std::string) "Bad cast of State in parse_stmt");
      }
      pr.ast(new ast::IfElseStmt(e, s1, s2));
    } else {
      pr.ast(new ast::IfStmt(e, s1));
    }
  } else if (attempt_match(scanner::kVariableName)) {
    /*
     * Stmt ::= varName '=' Expr ';'  | varName '[' Expr ':' Expr ']'
     * '=' Expr ';'
     */
    std::string varName = prev_token_->lexeme();
    if (attempt_match(scanner::kLeftSquare)) {
      ParseResult prExpr1 = parse_expr(0);
      ast::Expr * e1 = NULL;
      if (prExpr1.ast()) {
        e1 = dynamic_cast<ast::Expr *>(prExpr1.ast());
        if (!e1) throw((std::string) "Bad cast of State in parse_stmt");
      }
      match(scanner::kColon);
      ParseResult prExpr2 = parse_expr(0);
      ast::Expr * e2 = NULL;
      if (prExpr2.ast()) {
        e2 = dynamic_cast<ast::Expr *>(prExpr2.ast());
        if (!e2) throw((std::string) "Bad cast of State in parse_stmt");
      }
      match(scanner::kRightSquare);
      match(scanner::kAssign);
      ParseResult prExpr3 = parse_expr(0);
      ast::Expr * e3 = NULL;
      if (prExpr3.ast()) {
        e3 = dynamic_cast<ast::Expr *>(prExpr3.ast());
        if (!e3) throw((std::string) "Bad cast of State in parse_stmt");
      }
      match(scanner::kSemiColon);
      pr.ast(new ast::MatrixAssignStmt(varName, e1, e2, e3));
    } else {
      match(scanner::kAssign);
      ParseResult prExpr = parse_expr(0);
      ast::Expr * e = NULL;
      if (prExpr.ast()) {
        e = dynamic_cast<ast::Expr *>(prExpr.ast());
        if (!e) throw((std::string) "Bad cast of State in parse_stmt");
      }
      match(scanner::kSemiColon);
      pr.ast(new ast::AssignStmt(varName, e));
    }
  } else if (attempt_match(scanner::kPrintKwd)) {
    // Stmt ::= 'print' '(' Expr ')' ';'
    match(scanner::kLeftParen);
    ParseResult prExpr = parse_expr(0);
    ast::Expr * e = NULL;
    if (prExpr.ast()) {
      e = dynamic_cast<ast::Expr *>(prExpr.ast());
      if (!e) throw((std::string) "Bad cast of State in parse_stmt");
    }
    match(scanner::kRightParen);
    match(scanner::kSemiColon);
    pr.ast(new ast::PrintStmt(e));
  } else if (attempt_match(scanner::kRepeatKwd)) {
    // Stmt ::= 'repeat' '(' varName '=' Expr 'to' Expr ')' Stmt
    match(scanner::kLeftParen);
    match(scanner::kVariableName);
    std::string varName = prev_token_->lexeme();
    match(scanner::kAssign);
    ParseResult prExpr1 = parse_expr(0);
    ast::Expr * e1 = NULL;
    if (prExpr1.ast()) {
      e1 = dynamic_cast<ast::Expr *>(prExpr1.ast());
      if (!e1) throw((std::string) "Bad cast of State in parse_stmt");
    }
    match(scanner::kToKwd);
    ParseResult prExpr2 = parse_expr(0);
    ast::Expr * e2 = NULL;
    if (prExpr2.ast()) {
      e2 = dynamic_cast<ast::Expr *>(prExpr2.ast());
      if (!e2) throw((std::string) "Bad cast of State in parse_stmt");
    }
    match(scanner::kRightParen);
    ParseResult prStmt = parse_stmt();
    ast::Stmt * s = NULL;
    if (prStmt.ast()) {
      s = dynamic_cast<ast::Stmt *>(prStmt.ast());
      if (!s) throw((std::string) "Bad cast of State in parse_stmt");
    }
    pr.ast(new ast::RepeatStmt(varName, e1, e2, s));
  } else if (attempt_match(scanner::kWhileKwd)) {
    // Stmt ::= 'while' '(' Expr ')' Stmt
    match(scanner::kLeftParen);
    ParseResult prExpr = parse_expr(0);
    ast::Expr * e = NULL;
    if (prExpr.ast()) {
      e = dynamic_cast<ast::Expr *>(prExpr.ast());
      if (!e) throw((std::string) "Bad cast of State in parse_stmt");
    }
    match(scanner::kRightParen);
    ParseResult prStmt = parse_stmt();
    ast::Stmt * s = NULL;
    if (prStmt.ast()) {
      s = dynamic_cast<ast::Stmt *>(prStmt.ast());
      if (!s) throw((std::string) "Bad cast of State in parse_stmt");
    }
    pr.ast(new ast::WhileStmt(e, s));
  } else if (attempt_match(scanner::kSemiColon)) {
    // Stmt ::= ';
    // parsed a skip
    pr.ast(new ast::SemiColonStmt());
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a statement");
  }
  // Stmt ::= variableName assign Expr semiColon
  return pr;
}

// Expr
ParseResult Parser::parse_expr(int rbp) {
  /* Examine current token, without consuming it, to call its
     associated parse methods.  The ExtToken objects have 'nud' and
     'led' methods that are dispatchers that call the appropriate
     parse methods.*/
  ParseResult left = curr_token_->nud();

  while (rbp < curr_token_->lbp()) {
    left = curr_token_->led(left);
  }

  return left;
}

/*
 * parse methods for Expr productions
 * ----------------------------------
 */

// Expr ::= trueKwd
ParseResult Parser::parse_true_kwd() {
  ParseResult pr;
  match(scanner::kTrueKwd);
  std::string lexeme = prev_token_->lexeme();
  pr.ast(new ast::BooleanConstExpr(lexeme));
  return pr;
}

// Expr ::= falsekwd
ParseResult Parser::parse_false_kwd() {
  ParseResult pr;
  match(scanner::kFalseKwd);
  std::string lexeme = prev_token_->lexeme();
  pr.ast(new ast::BooleanConstExpr(lexeme));
  return pr;
}

// Expr ::= intConst
ParseResult Parser::parse_int_const() {
  ParseResult pr;
  match(scanner::kIntConst);
  std::string lexeme = prev_token_->lexeme();
  pr.ast(new ast::IntegerConstExpr(lexeme));
  return pr;
}

// Expr ::= floatConst
ParseResult Parser::parse_float_const() {
  ParseResult pr;
  match(scanner::kFloatConst);
  std::string lexeme = prev_token_->lexeme();
  pr.ast(new ast::FloatConstExpr(lexeme));
  return pr;
}

// Expr ::= stringConst
ParseResult Parser::parse_string_const() {
  ParseResult pr;
  match(scanner::kStringConst);
  std::string lexeme = prev_token_->lexeme();
  pr.ast(new ast::StringConstExpr(lexeme));
  return pr;
}

// Expr ::= variableName .....
ParseResult Parser::parse_variable_name() {
  ParseResult pr;
  match(scanner::kVariableName);
  std::string varName = prev_token_->lexeme();
  if (attempt_match(scanner::kLeftSquare)) {
    ParseResult prExpr1 = parse_expr(0);
    ast::Expr * e1 = NULL;
    if (prExpr1.ast()) {
      e1 = dynamic_cast<ast::Expr *>(prExpr1.ast());
      if (!e1) throw((std::string) "Bad cast of state in parse_variable_name");
    }
    match(scanner::kColon);
    ParseResult prExpr2 = parse_expr(0);
    ast::Expr * e2 = NULL;
    if (prExpr2.ast()) {
      e2 = dynamic_cast<ast::Expr *>(prExpr2.ast());
      if (!e2) throw((std::string) "Bad cast of state in parse_variable_name");
    }
    match(scanner::kRightSquare);
    pr.ast(new ast::MatrixREFExpr(varName, e1, e2));
  } else if (attempt_match(scanner::kLeftParen)) {
    // Expr ::= varableName '(' Expr ')'
    ParseResult prExpr = parse_expr(0);
    ast::Expr * e = NULL;
    if (prExpr.ast()) {
      e = dynamic_cast<ast::Expr *>(prExpr.ast());
      if (!e) throw((std::string) "Bad cast of state in parse_nested_expr");
    }
    match(scanner::kRightParen);
    pr.ast(new ast::NestedOrFunctionCallExpr(varName, e));
  } else {
    // variable
    pr.ast(new ast::VarNameExpr(varName));
  }
  return pr;
}

// Expr ::= leftParen Expr rightParen
ParseResult Parser::parse_nested_expr() {
  ParseResult pr;
  match(scanner::kLeftParen);
  ParseResult prExpr = parse_expr(0);
  ast::Expr * e = NULL;
  if (prExpr.ast()) {
    e = dynamic_cast<ast::Expr *>(prExpr.ast());
    if (!e) throw((std::string) "Bad cast of state in parse_nested_expr");
  }
  match(scanner::kRightParen);
  pr.ast(new ast::ParenthesesExpr(e));
  return pr;
}

// Expr ::= 'if' Expr 'then' Expr 'else' Expr
ParseResult Parser::parse_if_expr() {
  ParseResult pr;

  match(scanner::kIfKwd);
  ParseResult prExpr1 = parse_expr(0);
  ast::Expr * e1 = NULL;
  if (prExpr1.ast()) {
    e1 = dynamic_cast<ast::Expr *>(prExpr1.ast());
    if (!e1) throw((std::string) "Bad cast of state in parse_if_expr");
  }
  match(scanner::kThenKwd);
  ParseResult prExpr2 = parse_expr(0);
  ast::Expr * e2 = NULL;
  if (prExpr2.ast()) {
    e2 = dynamic_cast<ast::Expr *>(prExpr2.ast());
    if (!e2) throw((std::string) "Bad cast of state in parse_if_expr");
  }
  match(scanner::kElseKwd);
  ParseResult prExpr3 = parse_expr(0);
  ast::Expr * e3 = NULL;
  if (prExpr3.ast()) {
    e3 = dynamic_cast<ast::Expr *>(prExpr3.ast());
    if (!e3) throw((std::string) "Bad cast of state in parse_if_expr");
  }
  pr.ast(new ast::IfExpr(e1, e2, e3));
  return pr;
}

// Expr ::= 'let' Stmts 'in' Expr 'end'
ParseResult Parser::parse_let_expr() {
  ParseResult pr;
  match(scanner::kLetKwd);
  ParseResult prStmts = parse_stmts();
  ast::Stmts * s = NULL;
  if (prStmts.ast()) {
    s = dynamic_cast<ast::Stmts *>(prStmts.ast());
    if (!s) throw((std::string) "Bad cast of state in parse_let_expr");
  }
  match(scanner::kInKwd);
  ParseResult prExpr = parse_expr(0);
  ast::Expr * e = NULL;
  if (prExpr.ast()) {
    e = dynamic_cast<ast::Expr *>(prExpr.ast());
    if (!e) throw((std::string) "Bad cast of state in parse_let_expr");
  }
  match(scanner::kEndKwd);
  pr.ast(new ast::LetExpr(s, e));
  return pr;
}

// Expr ::= '!' Expr
ParseResult Parser::parse_not_expr() {
  ParseResult pr;
  match(scanner::kNotOp);
  ParseResult prExpr = parse_expr(0);
  ast::Expr * e = NULL;
  if (prExpr.ast()) {
    e = dynamic_cast<ast::Expr *>(prExpr.ast());
    if (!e) throw((std::string) "Bad cast of state in parse_let_expr");
  }
  pr.ast(new ast::NotExpr(e));
  return pr;
}
// Expr ::= Expr plusSign Expr
ParseResult Parser::parse_addition(ParseResult prLeft) {
  // parser has already matchekD left expression
  ast::Expr * eLeft = NULL;
  if (prLeft.ast()) {
    eLeft = dynamic_cast<ast::Expr *>(prLeft.ast());
    if (!eLeft) throw((std::string) "Bad cast of state in parse_addition");
  }
  ParseResult pr;
  match(scanner::kPlusSign);
  ParseResult prRight = parse_expr(prev_token_->lbp());
  ast::Expr * eRight = NULL;
  if (prRight.ast()) {
    eRight = dynamic_cast<ast::Expr *>(prRight.ast());
    if (!eRight) throw((std::string) "Bad cast of state in parse_addition");
  }
  pr.ast(new ast::AddExpr(eLeft, eRight));
  return pr;
}

// Expr ::= Expr star Expr
ParseResult Parser::parse_multiplication(ParseResult prLeft) {
  // parser has already matchekD left expression
  ast::Expr * eLeft = NULL;
  if (prLeft.ast()) {
    eLeft = dynamic_cast<ast::Expr *>(prLeft.ast());
    if (!eLeft) throw((std::string)
      "Bad cast of state in parse_multiplication");
  }
  ParseResult pr;
  match(scanner::kStar);
  ParseResult prRight = parse_expr(prev_token_->lbp());
  ast::Expr * eRight = NULL;
  if (prRight.ast()) {
    eRight = dynamic_cast<ast::Expr *>(prRight.ast());
    if (!eRight) throw((std::string)
      "Bad cast of state in parse_multiplication");
  }
  pr.ast(new ast::MultiplyExpr(eLeft, eRight));
  return pr;
}

// Expr ::= Expr dash Expr
ParseResult Parser::parse_subtraction(ParseResult prLeft) {
  // parser has already matchekD left expression
  ast::Expr * eLeft = NULL;
  if (prLeft.ast()) {
    eLeft = dynamic_cast<ast::Expr *>(prLeft.ast());
    if (!eLeft) throw((std::string) "Bad cast of state in parse_subtraction");
  }
  ParseResult pr;
  match(scanner::kDash);
  ParseResult prRight = parse_expr(prev_token_->lbp());
  ast::Expr * eRight = NULL;
  if (prRight.ast()) {
    eRight = dynamic_cast<ast::Expr *>(prRight.ast());
    if (!eRight) throw((std::string) "Bad cast of state in parse_subtraction");
  }
  pr.ast(new ast::SubtractExpr(eLeft, eRight));
  return pr;
}

// Expr ::= Expr forwardSlash Expr
ParseResult Parser::parse_division(ParseResult prLeft) {
  // parser has already matchekD left expression
  ast::Expr * eLeft = NULL;
  if (prLeft.ast()) {
    eLeft = dynamic_cast<ast::Expr *>(prLeft.ast());
    if (!eLeft) throw((std::string) "Bad cast of state in parse_division");
  }
  ParseResult pr;
  match(scanner::kForwardSlash);
  ParseResult prRight = parse_expr(prev_token_->lbp());
  ast::Expr * eRight = NULL;
  if (prRight.ast()) {
    eRight = dynamic_cast<ast::Expr *>(prRight.ast());
    if (!eRight) throw((std::string) "Bad cast of state in parse_division");
  }
  pr.ast(new ast::DivideExpr(eLeft, eRight));
  return pr;
}

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
*/
ParseResult Parser::parse_relational_expr(ParseResult prLeft) {
  // parser has already matchekD left expression
  ast::Expr * eLeft = NULL;
  if (prLeft.ast()) {
    eLeft = dynamic_cast<ast::Expr *>(prLeft.ast());
    if (!eLeft) throw((std::string)
      "Bad cast of state in parse_relational_expr");
  }
  // parse right side and create appropriate AST
  ParseResult pr;
  ast::Expr * eRight = NULL;
  if (attempt_match(scanner::kLessThan)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::LessExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kLessThanEqual)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::LessEqualExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kGreaterThan)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::GreaterExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kGreaterThanEqual)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::GreaterEqualExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kEqualsEquals)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::EqualsEqualsExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kNotEquals)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::NotEqualsExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kAndOp)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::LogicalAndExpr(eLeft, eRight));
  } else if (attempt_match(scanner::kOrOp)) {
    ParseResult prRight = parse_expr(prev_token_->lbp());
    if (prRight.ast()) {
      eRight = dynamic_cast<ast::Expr *>(prRight.ast());
      if (!eRight) throw((std::string)
        "Bad cast of state in parse_relational_expr");
    }
    pr.ast(new ast::LogicalOrExpr(eLeft, eRight));
  } else {
    throw(make_error_msg(curr_token_->terminal()) +
          " while parsing a relational expression");
  }
  return pr;
}

// Helper function used by the parser.

void Parser::match(const scanner::TokenType &tt) {
  if (!attempt_match(tt)) {
    throw(make_error_msg_expected(tt));
  }
}

bool Parser::attempt_match(const scanner::TokenType &tt) {
  if (curr_token_->terminal() == tt) {
    next_token();
    return true;
  }
  return false;
}

bool Parser::next_is(const scanner::TokenType &tt) {
  return curr_token_->terminal() == tt;
}

void Parser::next_token() {
  if (curr_token_ == NULL) {
    throw(std::string(
        "Internal Error: should not call nextToken in unitialized state"));
  } else if (curr_token_->terminal() == scanner::kEndOfFile &&
           curr_token_->next() == NULL) {
    prev_token_ = curr_token_;
  } else if (curr_token_->terminal() != scanner::kEndOfFile &&
             curr_token_->next() == NULL) {
    throw(make_error_msg("Error: tokens end with endOfFile"));
  } else {
    prev_token_ = curr_token_;
    curr_token_ = curr_token_->next();
  }
}

std::string Parser::terminal_description(const scanner::TokenType &terminal) {
  scanner::Token *dummy_token = new scanner::Token("", terminal, NULL);
  scanner::ExtToken *dummy_ext_token = new scanner::ExtToken(this, dummy_token);
  std::string s = dummy_ext_token->ExtendToken(this, dummy_token)
    ->description();
  delete dummy_token;
  delete dummy_ext_token;
  return s;
}

std::string Parser::make_error_msg_expected(
    const scanner::TokenType &terminal) {
  std::string s = (std::string) "Expected " + terminal_description(terminal) +
                  " but found " + curr_token_->description();
  return s;
}

std::string Parser::make_error_msg(const scanner::TokenType &terminal) {
  std::string s = "Unexpected symbol " + terminal_description(terminal);
  return s;
}

std::string Parser::make_error_msg(const char *msg) { return msg; }

} /* namespace parser */
} /* namespace fcal */

