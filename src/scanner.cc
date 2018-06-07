/*******************************************************************************
 * Name            : scanner.cc
 * Project         : fcal
 * Module          : scanner
 * Description     : This program creates a linked list of Tokens from an FCAL
 *                   program using regular expressions.
 * Copyright       : Tristan Mansfield and Shannyn Telander. All rights
 *                   reserved.
 * Original Authors: Tristan Mansfield, Shannyn Telander
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <include/regex.h>
#include <include/scanner.h>

/*******************************************************************************
 * Namespaces
 ******************************************************************************/

namespace fcal {
namespace scanner {

/*******************************************************************************
 * Functions
 ******************************************************************************/

  // Token class definitions

  Token::Token() {
    lexeme_ = "";
    next_ = nullptr;
  }

  Token::Token(std::string s, TokenType t, Token * n) {
    lexeme_ = s;
    terminal_ = t;
    next_ = n;
  }

  Token::~Token() {}

  TokenType Token::terminal() const {
    return terminal_;
  }

  void Token::set_terminal(TokenType t) {
    terminal_ = t;
  }

  std::string Token::lexeme() const {
    return lexeme_;
  }

  void Token::set_lexeme(std::string s) {
    lexeme_ = s;
  }

  Token * Token::next() const {
    return next_;
  }

  void Token::set_next(Token * n) {
    next_ = n;
  }

  // Scanner class definitions

  Scanner::Scanner() {
    // Create array of regular expressions
    reg_exprs_[kIntKwd] = make_regex("^int");
    reg_exprs_[kFloatKwd] = make_regex("^float");
    reg_exprs_[kBoolKwd] = make_regex("^boolean");
    reg_exprs_[kTrueKwd] = make_regex("^True");
    reg_exprs_[kFalseKwd] = make_regex("^False");
    reg_exprs_[kStringKwd] = make_regex("^string");
    reg_exprs_[kMatrixKwd] = make_regex("^matrix");
    reg_exprs_[kLetKwd] = make_regex("^let");
    reg_exprs_[kInKwd] = make_regex("^in");
    reg_exprs_[kEndKwd] = make_regex("^end");
    reg_exprs_[kIfKwd] = make_regex("^if");
    reg_exprs_[kThenKwd] = make_regex("^then");
    reg_exprs_[kElseKwd] = make_regex("^else");
    reg_exprs_[kRepeatKwd] = make_regex("^repeat");
    reg_exprs_[kWhileKwd] = make_regex("^while");
    reg_exprs_[kPrintKwd] = make_regex("^print");
    reg_exprs_[kToKwd] = make_regex("^to");

    reg_exprs_[kIntConst] = make_regex("^[0-9]+");
    reg_exprs_[kFloatConst] = make_regex("^[0-9]+\\.[0-9]+");
    reg_exprs_[kStringConst] = make_regex("^\"[^(\"||\n)]*\"");

    reg_exprs_[kVariableName] = make_regex("^([a-zA-Z_][a-zA-Z0-9_]*)");

    reg_exprs_[kLeftParen] = make_regex("^\\(");
    reg_exprs_[kRightParen] = make_regex("^)");
    reg_exprs_[kLeftCurly] = make_regex("^\\{");
    reg_exprs_[kRightCurly] = make_regex("^}");
    reg_exprs_[kLeftSquare] = make_regex("^\\[");
    reg_exprs_[kRightSquare] = make_regex("^]");
    reg_exprs_[kSemiColon] = make_regex("^;");
    reg_exprs_[kColon] = make_regex("^:");

    reg_exprs_[kAssign] = make_regex("^=");
    reg_exprs_[kPlusSign] = make_regex("^\\+");
    reg_exprs_[kStar] = make_regex("^\\*");
    reg_exprs_[kDash] = make_regex("^-");
    reg_exprs_[kForwardSlash] = make_regex("^/");
    reg_exprs_[kLessThan] = make_regex("^<");
    reg_exprs_[kLessThanEqual] = make_regex("^<=");
    reg_exprs_[kGreaterThan] = make_regex("^>");
    reg_exprs_[kGreaterThanEqual] = make_regex("^>=");
    reg_exprs_[kEqualsEquals] = make_regex("^==");
    reg_exprs_[kNotEquals] = make_regex("^!=");
    reg_exprs_[kAndOp] = make_regex("^&&");
    reg_exprs_[kOrOp] = make_regex("^\\|\\|");
    reg_exprs_[kNotOp] = make_regex("^!");

    reg_exprs_[kWhiteSpace] = make_regex("^[\n\t\r ]+");
    reg_exprs_[kBlockComment] = make_regex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    reg_exprs_[kLineComment] = make_regex("^(\\/\\/[^\n]*)");
  }

  Scanner::~Scanner() {}

  Token * Scanner::Scan(const char * text) {
    // Initialize counting variables and the token list
    int max_num_matched_chars = 0;
    int num_matched_chars = 0;
    Token * token_list = new Token();
    Token * return_token = token_list;

    // Consume leading white space and comments
    text += ConsumeWhitespaceAndComments(text,
                                         reg_exprs_[kWhiteSpace],
                                         reg_exprs_[kBlockComment],
                                         reg_exprs_[kLineComment]);

    // Match words until end of file
    while (text[0] != '\0') {
      max_num_matched_chars = 0;
      TokenType matched_token = kLexicalError;

      // Try to match Int Kwd
      num_matched_chars = match_regex(reg_exprs_[kIntKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kIntKwd;
      }
      // Try to match Float Kwd
      num_matched_chars = match_regex(reg_exprs_[kFloatKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kFloatKwd;
      }
      // Try to match Bool Kwd
      num_matched_chars = match_regex(reg_exprs_[kBoolKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kBoolKwd;
      }
      // Try to match True Kwd
      num_matched_chars = match_regex(reg_exprs_[kTrueKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kTrueKwd;
      }
      // Try to match False Kwd
      num_matched_chars = match_regex(reg_exprs_[kFalseKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kFalseKwd;
      }
      // Try to match String Kwd
      num_matched_chars = match_regex(reg_exprs_[kStringKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kStringKwd;
      }
      // Try to match Matrix Kwd
      num_matched_chars = match_regex(reg_exprs_[kMatrixKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kMatrixKwd;
      }
      // Try to match Let Kwd
      num_matched_chars = match_regex(reg_exprs_[kLetKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kLetKwd;
      }
      // Try to match In Kwd
      num_matched_chars = match_regex(reg_exprs_[kInKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kInKwd;
      }
      // Try to match End Kwd
      num_matched_chars = match_regex(reg_exprs_[kEndKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kEndKwd;
      }
      // Try to match If Kwd
      num_matched_chars = match_regex(reg_exprs_[kIfKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kIfKwd;
      }
      // Try to match Then Kwd
      num_matched_chars = match_regex(reg_exprs_[kThenKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kThenKwd;
      }
      // Try to match Else Kwd
      num_matched_chars = match_regex(reg_exprs_[kElseKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kElseKwd;
      }
      // Try to match Repeat Kwd
      num_matched_chars = match_regex(reg_exprs_[kRepeatKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kRepeatKwd;
      }
      // Try to match While Kwd
      num_matched_chars = match_regex(reg_exprs_[kWhileKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kWhileKwd;
      }
      // Try to match Print Kwd
      num_matched_chars = match_regex(reg_exprs_[kPrintKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kPrintKwd;
      }
      // Try to match To Kwd
      num_matched_chars = match_regex(reg_exprs_[kToKwd], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kToKwd;
      }
      // Try to match Int Const
      num_matched_chars = match_regex(reg_exprs_[kIntConst], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kIntConst;
      }
      // Try to match Float Const
      num_matched_chars = match_regex(reg_exprs_[kFloatConst], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kFloatConst;
      }
      // Try to match String Const
      num_matched_chars = match_regex(reg_exprs_[kStringConst], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kStringConst;
      }
      // Try to match Var Name
      num_matched_chars = match_regex(reg_exprs_[kVariableName], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kVariableName;
      }
      // Try to match Left Paren
      num_matched_chars = match_regex(reg_exprs_[kLeftParen], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kLeftParen;
      }
      // Try to match Right Paren
      num_matched_chars = match_regex(reg_exprs_[kRightParen], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kRightParen;
      }
      // Try to match Left Curly
      num_matched_chars = match_regex(reg_exprs_[kLeftCurly], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kLeftCurly;
      }
      // Try to match Right Curly
      num_matched_chars = match_regex(reg_exprs_[kRightCurly], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kRightCurly;
      }
      // Try to match Left Square
      num_matched_chars = match_regex(reg_exprs_[kLeftSquare], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kLeftSquare;
      }
      // Try to match Right Square
      num_matched_chars = match_regex(reg_exprs_[kRightSquare], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kRightSquare;
      }
      // Try to match Semi Colon
      num_matched_chars = match_regex(reg_exprs_[kSemiColon], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kSemiColon;
      }
      // Try to match Colon
      num_matched_chars = match_regex(reg_exprs_[kColon], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kColon;
      }
      // Try to match Assign
      num_matched_chars = match_regex(reg_exprs_[kAssign], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kAssign;
      }
      // Try to match Plus Sign
      num_matched_chars = match_regex(reg_exprs_[kPlusSign], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kPlusSign;
      }
      // Try to match Star
      num_matched_chars = match_regex(reg_exprs_[kStar], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kStar;
      }
      // Try to match Dash
      num_matched_chars = match_regex(reg_exprs_[kDash], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kDash;
      }
      // Try to match Forward Slash
      num_matched_chars = match_regex(reg_exprs_[kForwardSlash], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kForwardSlash;
      }
      // Try to match Less Than
      num_matched_chars = match_regex(reg_exprs_[kLessThan], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kLessThan;
      }
      // Try to match Less Than Equal
      num_matched_chars = match_regex(reg_exprs_[kLessThanEqual], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kLessThanEqual;
      }
      // Try to match Greater Than
      num_matched_chars = match_regex(reg_exprs_[kGreaterThan], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kGreaterThan;
      }
      // Try to match Greater Than Equal
      num_matched_chars = match_regex(reg_exprs_[kGreaterThanEqual], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kGreaterThanEqual;
      }
      // Try to match Equals Equals
      num_matched_chars = match_regex(reg_exprs_[kEqualsEquals], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kEqualsEquals;
      }
      // Try to match Not Equals
      num_matched_chars = match_regex(reg_exprs_[kNotEquals], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kNotEquals;
      }
      // Try to match And Op
      num_matched_chars = match_regex(reg_exprs_[kAndOp], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kAndOp;
      }
      // Try to match Or Op
      num_matched_chars = match_regex(reg_exprs_[kOrOp], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kOrOp;
      }
      // Try to match Not Op
      num_matched_chars = match_regex(reg_exprs_[kNotOp], text);
      if (num_matched_chars > max_num_matched_chars) {
        max_num_matched_chars = num_matched_chars;
        matched_token = kNotOp;
      }
      // Check if there was no match
      if (matched_token == kLexicalError) {
        max_num_matched_chars = 1;
      }

      // Create Token
      token_list->set_terminal(matched_token);
      std::string lexeme = "";
      for (int i = 0; i < max_num_matched_chars; i++) {
        lexeme += text[i];
      }
      token_list->set_lexeme(lexeme);
      token_list->set_next(new Token());
      token_list = token_list->next();

      // Increment Text
      text += max_num_matched_chars;

      // Consume white space and comments
      text += ConsumeWhitespaceAndComments(text,
                                           reg_exprs_[kWhiteSpace],
                                           reg_exprs_[kBlockComment],
                                           reg_exprs_[kLineComment]);
    }

    // Add EOF Token to end of list
    token_list->set_terminal(kEndOfFile);

    return return_token;
  }

  int Scanner::ConsumeWhitespaceAndComments(const char *text,
                                               regex_t * white_space,
                                               regex_t * block_comment,
                                               regex_t * line_comment) {
    int num_matched_chars = 0;
    int total_num_matched_chars = 0;
    int still_consuming_white_space;

    do {
      still_consuming_white_space = 0;  // exit loop if not reset by a match

      // Try to match white space
      num_matched_chars = match_regex(white_space, text);
      total_num_matched_chars += num_matched_chars;
      if (num_matched_chars > 0) {
        text = text + num_matched_chars;
        still_consuming_white_space = 1;
      }

      // Try to match block comments
      num_matched_chars = match_regex(block_comment, text);
      total_num_matched_chars += num_matched_chars;
      if (num_matched_chars > 0) {
        text = text + num_matched_chars;
        still_consuming_white_space = 1;
      }

      // Try to match line comments
      num_matched_chars = match_regex(line_comment, text);
      total_num_matched_chars += num_matched_chars;
      if (num_matched_chars > 0) {
        text = text + num_matched_chars;
        still_consuming_white_space = 1;
      }
    } while (still_consuming_white_space);

    return total_num_matched_chars;
  } /* ConsumeWhitespaceAndComments() */

} /* namespace scanner */
} /* namespace fcal */
