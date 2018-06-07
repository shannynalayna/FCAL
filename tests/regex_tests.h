#include <cxxtest/TestSuite.h>
#include "include/regex.h"

using namespace std;
class RegexTestSuite : public CxxTest::TestSuite {
 public:
  // Tests for makeRegex and matchRegex
  // --------------------------------------------------
  /* These tests ensure that the makeRegex and matchRegex work as
     expected.  These tests are independent from the scanner or
     WordCount application.
   */

/* Tests to ensure the regular expressions created in scanner.cc
	function as expected */



 void make_regex_test(const char * regex_string, const char * text, int match_chars) {
  regex_t *re = fcal::scanner::make_regex(regex_string);
  TS_ASSERT(re);
  int numMatchedChars = fcal::scanner::match_regex(re, text);
  TS_ASSERT(numMatchedChars == match_chars);
 }
 

 void test_make_matchRegex_match_IntKwd(void) {
  make_regex_test("^int", "int ", 3);
 }

 void test_make_matchRegex_match_FloatKwd(void) {
  make_regex_test("^float", "float ", 5);
 }

 void test_make_matchRegex_match_BoolKwd(void) {
  make_regex_test("^boolean", "boolean ", 7);

 }

 void test_make_matchRegex_match_TrueKwd(void) {
  make_regex_test("^True", "True ", 4);
 }

 void test_make_matchRegex_match_FalseKwd(void) {
  make_regex_test("^False", "False ",  5);
 }


 void test_make_matchRegex_match_StringKwd(void) {
  make_regex_test("^string", "string ", 6);
 }

 void test_make_matchRegex_match_MatrixKwd(void) {
  make_regex_test("^matrix", "matrix ", 6);
 }

 void test_make_matchRegex_match_LetKwd(void) {
  make_regex_test("^let", "let ", 3);
 }


 void test_make_matchRegex_match_InKwd(void) {
  make_regex_test("^in", "in ", 2);
 }

 void test_make_matchRegex_match_EndKwd(void) {
  make_regex_test("^end", "end ", 3);
 }

 void test_make_matchRegex_match_IfKwd(void) {
  make_regex_test("^if", "if ", 2);
 }

 void test_make_matchRegex_match_ThenKwd(void) {
  make_regex_test("^then", "then ", 4);
 }

 void test_make_matchRegex_match_ElseKwd(void) {
  make_regex_test("^else", "else ", 4);
 }

 void test_make_matchRegex_match_WhileKwd(void) {
  make_regex_test("^while", "while ", 5);
 }

 void test_make_matchRegex_match_RepeatKwd(void) {
  make_regex_test("^repeat", "repeat ", 6);
 }

 void test_make_matchRegex_match_PrintKwd(void) {
  make_regex_test("^print", "print ", 5);
 }

 void test_make_matchRegex_match_ToKwd(void) {
  make_regex_test("^to", "to ",  2);
 }

 void test_make_matchRegex_match_IntConst(void) {
  make_regex_test("^[0-9]+", "123 ", 3);
 }

 void test_make_matchRegex_match_FloatConst(void) {
  make_regex_test("^[0-9]+\\.[0-9]+", "123.123 ", 7);
 }

 void test_make_matchRegex_match_StringConst(void) {
  make_regex_test("^\"[^(\"||\n)]*\"", "\"String\" ", 8);
 }


 void test_make_matchRegex_match_VarName(void) {
  make_regex_test("^([a-zA-Z_][a-zA-Z0-9_]*)", "VariableName ", 12);
 }


 void test_make_matchRegex_match_LeftParen(void) {
  make_regex_test("^\\(", "( ", 1);
 }


 void test_make_matchRegex_match_RightParen(void) {
  make_regex_test("^)", ") ", 1);
 }


 void test_make_matchRegex_match_LeftCurly(void) {
  make_regex_test("^\\{", "{ ", 1);
 }


 void test_make_matchRegex_match_RightCurly(void) {
  make_regex_test("^}", "} ", 1);
 }


 void test_make_matchRegex_match_LeftSquare(void) {
  make_regex_test("^\\[", "[ ", 1);
 }


 void test_make_matchRegex_match_RightSquare(void) {
  make_regex_test("^]", "] ", 1);
 }


 void test_make_matchRegex_match_SemiColon(void) {
  make_regex_test("^;", "; ", 1);
 }


 void test_make_matchRegex_match_Colon(void) {
  make_regex_test("^:", ": ", 1);
 }


 void test_make_matchRegex_match_Assign(void) {
  make_regex_test("^=", "= ", 1);
 }


 void test_make_matchRegex_match_PlusSign(void) {
  make_regex_test("^\\+", "+ ", 1);
 }


 void test_make_matchRegex_match_Star(void) {
  make_regex_test("^\\*", "* ", 1);
 }


 void test_make_matchRegex_match_Dash(void) {
  make_regex_test("^-", "- ", 1);
 }


 void test_make_matchRegex_match_ForwardSlash(void) {
  make_regex_test("^/", "/ ", 1);
 }


 void test_make_matchRegex_match_LessThan(void) {
  make_regex_test("^<", "< ", 1);
 }


 void test_make_matchRegex_match_LessThanEqual(void) {
  make_regex_test("^<=", "<= ", 2);
 }


 void test_make_matchRegex_match_GreaterThan(void) {
  make_regex_test("^>", "> ", 1);
 }


 void test_make_matchRegex_match_GreaterThanEqual(void) {
  make_regex_test("^>=", ">= ", 2);
 }


 void test_make_matchRegex_match_EqualsEquals(void) {
  make_regex_test("^==", "== ", 2);
 }


 void test_make_matchRegex_match_NotEquals(void) {
  make_regex_test("^!=", "!= ", 2);
 }


 void test_make_matchRegex_match_AndOp(void) {
  make_regex_test("^&&", "&& ", 2);
 }


  void test_make_matchRegex_match_OrOp(void) {
    make_regex_test("^\\|\\|", "|| ", 2);
  }


  void test_make_matchRegex_match_NotOp(void) {
    make_regex_test("^!", "! ", 1);
  }

/* Pre-written tests for matchRegex */

  void test_make_matchRegex_match(void) {
    regex_t *re = fcal::scanner::make_regex("^[0-9]+");
    TS_ASSERT(re);
    int numMatchedChars = fcal::scanner::match_regex(re, "123 ");
    TS_ASSERT(numMatchedChars == 3);
  }

  void test_make_matchRegex_no_match(void) {
    regex_t *re = fcal::scanner::make_regex("^[0-9]+");
    TS_ASSERT(re);
    int numMatchedChars = fcal::scanner::match_regex(re, " 123 ");
    TS_ASSERT(numMatchedChars == 0);
  }

  void test_make_matchRegex_match_string_copy(void) {
    regex_t *re = fcal::scanner::make_regex("^[0-9]+");
    TS_ASSERT(re);
    const char *text = "123 ";
    int numMatchedChars = fcal::scanner::match_regex(re, text);
    TS_ASSERT(numMatchedChars == 3);
    std::string lex(text, numMatchedChars);
    TS_ASSERT_EQUALS(lex, "123");
  }

};
