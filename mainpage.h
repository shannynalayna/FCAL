/*! \mainpage Group WoollyMammoth Index Page


\section intro_sec Introduction

Multiple parts of this project are used to translate the sample program to the target language. 

We have a defined list of token types within this program that correspond to constructs, constants, keywords, and expression types that exist in the target language's 'grammar.'

The scanner for this program takes a sample program in the FCAL (Forest Cover Analysis Language), recognizes the defined tokens in the program and returns them in a linked list. This is a lexical check of the program, if the scanner fails, then the program is lexically incorrect. 

The parser for this project takes in the sequence of Token object created by the scanner. The parser creates a syntax tree of the given tokens based off of the grammar of the target language, assuming the input is correct. If the input is incorrect, the structure of the syntax tree cannot be created and the program is syntactically incorrect. 

\section install_sec Compilation

In order to compile this project and run it through the tests defined in the tests/ directory, run the command:

	%make

You can also run the command:

	%make run-tests

to accomplish compiling the program and running the provided tests in the tests/ directory. 

In order to clear the additional files that were created in this compilation, run the command:

	%make clean


\section Additional Information

*/  
