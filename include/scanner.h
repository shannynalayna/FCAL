/*******************************************************************************
 * Name            : scanner.h
 * Project         : fcal
 * Module          : scanner
 * Description     : Header file for scanner module
 * Copyright       : 2017 CSCI3081W Staff. All rights reserved.
 * Original Author : Eric Van Wyk
 * Modifications by: John Harwell, Tristan Mansfield, and Shannyn Telander
 ******************************************************************************/

#ifndef PROJECT_INCLUDE_SCANNER_H_
#define PROJECT_INCLUDE_SCANNER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <regex.h>
#include <string>


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/* below are the possible terminal types for each token */

enum kTokenEnumType {
  kIntKwd,
  kFloatKwd,
  kBoolKwd,
  kTrueKwd,
  kFalseKwd,
  kStringKwd,
  kMatrixKwd,
  kLetKwd,
  kInKwd,
  kEndKwd,
  kIfKwd,
  kThenKwd,
  kElseKwd,
  kRepeatKwd,
  kWhileKwd,
  kPrintKwd,
  kToKwd,

  // Constants
  kIntConst,
  kFloatConst,
  kStringConst,

  // Names
  kVariableName,

  // Punctuation
  kLeftParen,
  kRightParen,
  kLeftCurly,
  kRightCurly,
  kLeftSquare,
  kRightSquare,
  kSemiColon,
  kColon,

  // Operators
  kAssign,
  kPlusSign,
  kStar,
  kDash,
  kForwardSlash,
  kLessThan,
  kLessThanEqual,
  kGreaterThan,
  kGreaterThanEqual,
  kEqualsEquals,
  kNotEquals,
  kAndOp,
  kOrOp,
  kNotOp,

  // Special terminal types
  kEndOfFile,
  kLexicalError
};
typedef enum kTokenEnumType TokenType;

/*******************************************************************************
 * Class Definitions
 * 
 * You will need to define a Token class (remember, a pointer of type Token *
 * is returned by the scan method of the scanner class.) 
 * Also, Classes are typically declared (and sometimes also defined) in
 * their own header files
 * 
 * You will also need to define a Scanner class with a scan and other methods
 ******************************************************************************/
/*!
  The Token class maintains a singly linked list of Token objects. Each Token
  has a terminal of type TokenType (a previously defined enum that is used to
  identify the type of a token), a string lexeme that contains the actual text
  that was scanned for the token, and a pointer to the next Token in the list.
  Each of these parameters has a get and set funcition.
 */
class Token {
 public:
    Token();
    Token(std::string s, TokenType t, Token * n);
    ~Token();

    TokenType terminal() const;
    void set_terminal(TokenType t);

    std::string lexeme() const;
    void set_lexeme(std::string s);

    Token * next() const;
    void set_next(Token * n);

 private:
    TokenType terminal_;
    std::string lexeme_;
    Token * next_;
};

/*!
  The Scanner class is used to scan an FCAL program (inputted as a character 
  string) for tokens and send them to a parser. The Scan function is the main
  component of this class; it does all of the regex matching to determine the
  next token in the program and creates a Token object list to be returned.
  This function uses an array of regex_t regular expressions (which correspond
  to FCAL language tokens) for token matching.
 */
class Scanner {
 public:
    Scanner();
    ~Scanner();

    Token * Scan(const char * text);

    int ConsumeWhitespaceAndComments(const char *text,
                                        regex_t * white_space,
                                        regex_t * block_comment,
                                        regex_t * line_comment);

 private:
    // These values are assigned so that changes to the token enum type do not
    // break the code
    static const int kNumRegularExpressions = kLexicalError + 2;
    static const int kWhiteSpace = kNumRegularExpressions - 3;
    static const int kBlockComment = kNumRegularExpressions - 2;
    static const int kLineComment = kNumRegularExpressions - 1;

    regex_t * reg_exprs_[kNumRegularExpressions];
};

} /* namespace scanner */
} /* namespace fcal */

#endif  // PROJECT_INCLUDE_SCANNER_H_
