#include <cxxtest/TestSuite.h>
#include <iostream>
#include "include/parser.h"
#include "include/read_input.h"

#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;
using namespace fcal;
using namespace parser;
using namespace ast;
using namespace scanner;

class AstTestSuite : public CxxTest::TestSuite
{
public:

    Parser p ;

    void writeFile ( const string text, const string filename) {
        ofstream out(filename.c_str());
        out << (string) text << endl ;
    }

    void unparse_tests ( string file) {
        string path = "./samples/" + file ;

        // 1. Test that the file can be parsed.
        ParseResult pr1 = p.Parse(ReadInputFromFile( path.c_str()));
        TSM_ASSERT(file + " failed to parse.", pr1.ok()) ;

        // 2. Verify that the ast field is not null
        TSM_ASSERT(file + " failed to generate an AST.", pr1.ast() != NULL);

        // 3. Verify that the "unparsing" is non-empty.
        string up1 = pr1.ast()->unparse() ;
        writeFile(up1,(path+"up1").c_str());
        TSM_ASSERT(file + " failed to generate non-empty unparsing.",
                     up1.length() > 0) ;

        // 4. Verify that the un-parsed string can be parsed.
        ParseResult pr2 = p.Parse(up1.c_str()) ;
        TSM_ASSERT(file + " failed to parse the first un-parsing.",
                     pr2.ok()) ;

        // 5. Verify that the ast field is not null after first unparsing
        TSM_ASSERT(file + " first unparsing failed to generate an AST.",
                     pr2.ast() != NULL);

        // 6. Verify that this second unparsing can be parsed.
        string up2 = pr2.ast()->unparse() ;
        writeFile(up2,(path+"up2").c_str());
        ParseResult pr3 = p.Parse(up2.c_str()) ;
        TSM_ASSERT(file + " failed to unparse the second un-parsing.",
                     pr3.ok()) ;

        string up3 = pr3.ast()->unparse() ;
        writeFile(up3,(path+"up3").c_str());
        // 7. Verify that the first and second unparsings are the same.
        TSM_ASSERT_EQUALS(file + " unparse-1 != unparse-2.", up1, up2) ;
        // 8. Verifty that the second and third unparsings are the same.
        TSM_ASSERT_EQUALS(file + " unparse-2 != unparse-3.", up2, up3) ;
    }
/*! The following tests evaluate the functionality of our parser
Each of the tests were implemented in the order they are written on April 11th, 2017.
We ran into memory issues when the program was exiting. However, we were able
to resolve this issue and pass all of the assertions. 
The tests initially passed on April 11th, 2017 */
    void test_sample_1(void) { unparse_tests("sample_1.dsl"); }
    void test_sample_2(void) { unparse_tests("sample_2.dsl"); }
    void test_sample_3(void) { unparse_tests("sample_3.dsl"); }
    void test_sample_4(void) { unparse_tests("sample_4.dsl"); }
    void test_sample_5(void) { unparse_tests("sample_5.dsl"); }
    void test_mysample(void) { unparse_tests("mysample.dsl"); }
    void test_forest_loss(void) { unparse_tests("forest_loss_v2.dsl"); }
} ;
