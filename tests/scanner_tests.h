#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include "include/read_input.h"
#include "include/scanner.h"

using namespace std;
namespace scanner = fcal::scanner;
class ScannerTestSuite : public CxxTest::TestSuite
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    scanner::Scanner *s;
    void test_setup_code () {
      s = new scanner::Scanner();
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You should test the regular expressions you use to make and match
       regular expressions,and the functions used by makeRegex and
       matchRegex regex_tests.h.
       However, You will likely need to write several tests to
       adequately test  the functions that are called from "scan".
       Once you are confident that the components used by "scan"
       work properly, then you can run tests on the "scan" method itself.
    */

    // You should place tests for these other functions used by
    // scan below.


    /* Below is one of the tests for these components in the project
       solution created by your instructor.  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly () {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{");
        }


        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
	
	    */
	
	void make_scanner_test(const char * text, scanner::TokenType test_token) {
		scanner::Token *tks = s->Scan(text);
		TS_ASSERT(tks != NULL);
		TS_ASSERT(tks->terminal() == test_token);
	} 
	


	void test_terminal_IntKwd (void) {
		make_scanner_test("int ", scanner::kIntKwd);
	}
		//Can now change all tests to this, runs fine
	void test_terminal_FloatKwd (void) {
		make_scanner_test("float ", scanner::kFloatKwd);
	}

	void test_terminal_BoolKwd (void) {
		make_scanner_test("boolean ", scanner::kBoolKwd);
	}

	void test_terminal_TrueKwd (void) {
		make_scanner_test("True ", scanner::kTrueKwd);
	}

	void test_terminal_FalseKwd (void) {
		make_scanner_test("False ", scanner::kFalseKwd);
	}

	void test_terminal_StringKwd (void) {
		make_scanner_test("string ", scanner::kStringKwd);
	}


	void test_terminal_MatrixKwd (void) {
		make_scanner_test("matrix ", scanner::kMatrixKwd);
	}


	void test_terminal_LetKwd (void) {
		make_scanner_test("let ", scanner::kLetKwd);
	}


	void test_terminal_InKwd (void) {
		make_scanner_test("in ", scanner::kInKwd);
	}


	void test_terminal_EndKwd (void) {
		make_scanner_test("end ", scanner::kEndKwd);
	}


	void test_terminal_IfKwd (void) {
		make_scanner_test("if ", scanner::kIfKwd);
	}


	void test_terminal_ThenKwd (void) {
		make_scanner_test("then ", scanner::kThenKwd);
	}


	void test_terminal_ElseKwd (void) {
		make_scanner_test("else ", scanner::kElseKwd);
	}


	void test_terminal_RepeatKwd (void) {
		make_scanner_test("repeat ", scanner::kRepeatKwd);
	}

	void test_terminal_WhileKwd (void) {
		make_scanner_test("while ", scanner::kWhileKwd);
	}

	void test_terminal_PrintKwd (void) {
		make_scanner_test("print ", scanner::kPrintKwd);
	}


	void test_terminal_ToKwd (void) {
		make_scanner_test("to ", scanner::kToKwd);
	}


	void test_terminal_IntConst (void) {
		make_scanner_test("123 ", scanner::kIntConst);
	}

	void test_terminal_FloatConst (void) {
		make_scanner_test("123.123 ", scanner::kFloatConst);
	}


	void test_terminal_StringConst (void) {
		make_scanner_test("\"This is a string\" ", scanner::kStringConst);
	}


	void test_terminal_VarName (void) {
		make_scanner_test("exampleVariableName ", scanner::kVariableName);
	}


	void test_terminal_LeftParen (void) {
		make_scanner_test("( ", scanner::kLeftParen);
	}


	void test_terminal_RightParen (void) {
		make_scanner_test(") ", scanner::kRightParen);
	}


	void test_terminal_LeftCurly (void) {
		make_scanner_test("{ ", scanner::kLeftCurly);
	}


	void test_terminal_RightCurly (void) {
		make_scanner_test("} ", scanner::kRightCurly);
	}


	void test_terminal_LeftSquare (void) {
		make_scanner_test("[ ", scanner::kLeftSquare);
	}


	void test_terminal_RightSquare (void) {
                make_scanner_test("] ", scanner::kRightSquare);
	}


	void test_terminal_SemiColon (void) {
                make_scanner_test("; ", scanner::kSemiColon);
	}


	void test_terminal_Colon (void) {
                make_scanner_test(": ", scanner::kColon);;
	}


	void test_terminal_Assign (void) {
                make_scanner_test("= ", scanner::kAssign);
	}

	void test_terminal_PlusSign (void) {
                make_scanner_test("+ ", scanner::kPlusSign);
	}


	void test_terminal_Star (void) {
                make_scanner_test("* ", scanner::kStar);
	}


	void test_terminal_Dash (void) {
                make_scanner_test("- ", scanner::kDash);
	}


	void test_terminal_ForwardSlash (void) {
                make_scanner_test("/ ", scanner::kForwardSlash);
	}


	void test_terminal_LessThan(void) {
                make_scanner_test("< ", scanner::kLessThan);
	}


	void test_terminal_LessThanEqual (void) {
                make_scanner_test("<= ", scanner::kLessThanEqual);
	}


	void test_terminal_GreaterThan (void) {
                make_scanner_test("> ", scanner::kGreaterThan);
	}


	void test_terminal_GreaterThanEqual (void) {
                make_scanner_test(">= ", scanner::kGreaterThanEqual);
	}


	void test_terminal_EqualEquals (void) {
                make_scanner_test("== ", scanner::kEqualsEquals);

	}


	void test_terminal_NotEqualsKwd (void) {
                make_scanner_test("!= ", scanner::kNotEquals);
	}


	void test_terminal_AndOp (void) {
                make_scanner_test("&& ", scanner::kAndOp);
	}


	void test_terminal_OrOp (void) {
                make_scanner_test("|| ", scanner::kOrOp);
	}


	void test_terminal_NotOp (void) {
                make_scanner_test("! ", scanner::kNotOp);
	}

	void test_terminal_lexicalError (void) {
                make_scanner_test("$& ", scanner::kLexicalError);
	}

	void test_terminal_EndOfFile (void) {
                make_scanner_test("  ", scanner::kEndOfFile);
	}

    /* Tests for each terminal type above
	Tests for each terminal type's regular expression written
	in scanner.cc are in regex_tests.h
    */

    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */



    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (scanner::Token *tks) {
        scanner::Token *currentToken = tks;
        while (currentToken != NULL) {
          if (currentToken->terminal() == scanner::kLexicalError) {
              printf("problem: %s\n",currentToken->lexeme().c_str());
              fflush(stdout);
                                return false;
            }
            else {
              currentToken = currentToken->next();
            }
        }
        return true;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned.
    */
    void scanFileNoLexicalErrors(const char* filename) {
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT (text);
        scanner::Token *tks = s->Scan(text);
        TS_ASSERT(tks != NULL);
        TS_ASSERT(noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
  bool sameTerminals(scanner::Token *tks, int numTerms,
                     scanner::TokenType *ts) {
        scanner::Token *currentToken = tks;
        int termIndex = 0;
        while(currentToken != NULL && termIndex < numTerms) {

            if(currentToken->terminal() != ts[termIndex]) {
                printf("item at index: %i: terminal: %i should be terminal: %i\n",termIndex,currentToken->terminal(), ts[termIndex]);
				fflush(stdout);
                return false;
            }
            else {
                ++ termIndex;
                currentToken = currentToken->next();
            }
        }
        return true;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty() {
        scanner::Token *tks = s->Scan("  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    void test_scan_empty_comment() {
        scanner::Token *tks = s->Scan(" /* a comment */ ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }

    // When a lexical error occurs, the scanner creates a token with a
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors() {
        scanner::Token *tks = s->Scan("$&1  ");
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "$");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kLexicalError);
        TS_ASSERT_EQUALS(tks->lexeme(), "&");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kIntConst);
        TS_ASSERT_EQUALS(tks->lexeme(), "1");
        tks = tks->next();
        TS_ASSERT(tks != NULL);
        TS_ASSERT_EQUALS(tks->terminal(), scanner::kEndOfFile);
        TS_ASSERT(tks->next() == NULL);
    }


    // A test for scanning numbers and a variable.

    void test_scan_nums_vars() {
        scanner::Token *tks = s->Scan(" 123 x 12.34 ");
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = { scanner::kIntConst, scanner::kVariableName,
                                           scanner::kFloatConst, scanner::kEndOfFile };
        TS_ASSERT( sameTerminals( tks, 4, ts));
    }


    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexemes
       are correct.
     */

    void test_scan_bad_syntax_good_tokens() {
        const char *filename = "./samples/bad_syntax_good_tokens.dsl";
        char *text =  scanner::ReadInputFromFile(filename);
        TS_ASSERT(text);
        scanner::Token *tks = NULL;
        tks = s->Scan( text);
        TS_ASSERT(tks != NULL);
        scanner::TokenType ts[] = {
          scanner::kIntConst, scanner::kIntConst, scanner::kIntConst, scanner::kIntConst,
          scanner::kStringConst, scanner::kStringConst, scanner::kStringConst,
          scanner::kFloatConst, scanner::kFloatConst, scanner::kFloatConst,
          scanner::kMatrixKwd,
          scanner::kSemiColon, scanner::kColon, scanner::kToKwd,
          scanner::kLeftCurly, scanner::kLeftParen, scanner::kRightCurly, scanner::kRightParen,
          scanner::kPlusSign, scanner::kStar, scanner::kDash, scanner::kForwardSlash,
          scanner::kEqualsEquals, scanner::kLessThanEqual,
          scanner::kGreaterThanEqual, scanner::kNotEquals,
          scanner::kAssign,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kVariableName, scanner::kVariableName, scanner::kVariableName,
          scanner::kIntKwd, scanner::kFloatKwd, scanner::kStringKwd,
          scanner::kEndOfFile
        };
        int count = 38;
        TS_ASSERT( sameTerminals( tks, count, ts));
    }

    void test_scan_sample_forestLoss() {
        scanFileNoLexicalErrors("./samples/forest_loss_v2.dsl");
    }

};
