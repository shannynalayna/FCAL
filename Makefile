#Note, you may have to update CXX_DIR and
#CXXTEXT to point the right locations and executables
#to get it to work - look at your Lab 3 Makefile
CXX_DIR = ./lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -I. -std=gnu++11

all: run-tests

# Program files.
read_input.o:	src/read_input.cc include/read_input.h
	g++ $(FLAGS) -c src/read_input.cc 

regex.o:	src/regex.cc include/regex.h
	g++ $(FLAGS) -c src/regex.cc 

# Below is a possible way to make scanner
# Yours may vary depending on your implementation
scanner.o:	src/scanner.cc include/scanner.h include/regex.h
	g++ $(FLAGS) -c src/scanner.cc 

parser.o:	src/parser.cc include/parser.h include/parse_result.h include/scanner.h
	g++ $(FLAGS) -c src/parser.cc 

ext_token.o:	src/ext_token.cc include/ext_token.h include/parser.h include/scanner.h
	g++ $(FLAGS) -c src/ext_token.cc



# Testing files and targets.  
# run-tests should work once 
# and run regex_tests once you put the files
# we gave you in the right places
# and update the CXX_DIR and CXXTEST variables so they
# reference the correct directory locations
# Add scanner_tests to the dependency list and uncomment when
# you are ready to start testing units with scanner_tests.
run-tests:	regex_tests scanner_tests parser_tests ast_tests codegeneration_tests
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests
	./codegeneration_tests

#This should work once you put the files
#we gave you in the right places
regex_tests:	regex_tests.cc regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o regex_tests regex.o regex_tests.cc

regex_tests.cc:	include/regex.h tests/regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cc tests/regex_tests.h

# Below is a possible way to make scanner_tests and scanner_tests.cc
# Yours may vary depending on your design and implementation 
scanner_tests:	scanner_tests.cc scanner.o regex.o read_input.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o scanner_tests \
		scanner.o regex.o read_input.o scanner_tests.cc

scanner_tests.cc:	scanner.o tests/scanner_tests.h include/read_input.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cc tests/scanner_tests.h

# Parser tests

parser_tests:	parser_tests.cc parser.o scanner.o ext_token.o regex.o read_input.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o parser_tests \
		parser.o scanner.o ext_token.o regex.o read_input.o parser_tests.cc

parser_tests.cc:	parser.o scanner.o tests/parser_tests.h 
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cc tests/parser_tests.h

# AST tests

ast_tests:	ast_tests.cc parser.o scanner.o ext_token.o regex.o read_input.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o ast_tests \
		parser.o scanner.o ext_token.o regex.o read_input.o ast_tests.cc

ast_tests.cc:	parser.o scanner.o tests/ast_tests.h
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cc tests/ast_tests.h

# Code Generation tests

codegeneration_tests:	codegeneration_tests.cc parser.o scanner.o ext_token.o regex.o read_input.o
	g++ $(FLAGS) -I$(CXX_DIR) -I. -o codegeneration_tests \
		parser.o scanner.o ext_token.o regex.o read_input.o codegeneration_tests.cc

codegeneration_tests.cc:	parser.o read_input.o tests/codegeneration_tests.h
	$(CXXTEST) $(CXXFLAGS) -o codegeneration_tests.cc tests/codegeneration_tests.h

clean:
	rm -Rf *.o \
		regex_tests regex_tests.cc \
		scanner_tests scanner_tests.cc \
		parser_tests parser_tests.cc \
		ast_tests ast_tests.cc \
		codegeneration_tests codegeneration_tests.cc

