#
# An example Makefile illustrating dependencies among the different
# parts of mparser (which are pretty simple).
#
# Also illustrates how to compile gtest, a readline-based matlab
# parser useful for debugging.
#
# Test section is helpful to test core parts of the parsing functionality in
# C. Can be triggered by:
#
# make tests
#
SHELL=/bin/bash
ANTLR= /usr/local
LIBJSON= ../json
CC= gcc
CPPC= g++
CCOPTS= -I$(ANTLR)/include -L$(ANTLR)/lib -I$(LIBJSON) -L$(LIBJSON)
CPPCOPTS= -I$(ANTLR)/include -L$(ANTLR)/lib -I$(LIBJSON) -L$(LIBJSON) -g
#MEX= mex
MEX=/usr/local/MATLAB/R2011a/bin/mex


all: frontend mex_frontend

frontend: m_parser

m_parser: m_parser.hpp m_parser.cpp
	$(CPPC) $(CPPCOPTS) -DMPARSER_DLL -DMPARSER_DLL_EXPORTS -shared -Wl,-soname,m_parser -o m_parser.so \
	-fPIC m_parser.cpp m_emitter.cpp json_emitter.cpp output/matlabLexer.cpp output/matlabParser.cpp \
	-lantlr3c -lboost_filesystem

mex_frontend: mparser show_tree

mparser: mparser.cpp output/matlabLexer.h output/matlabParser.h output/matlabLexer.cpp output/matlabParser.cpp
	$(MEX) $(CCOPTS) -o mparser mparser.cpp output/matlabLexer.cpp output/matlabParser.cpp $(CLIBS) -lantlr3c

show_tree: show_tree.cpp output/matlabLexer.h output/matlabParser.h output/matlabLexer.cpp output/matlabParser.cpp
	$(MEX) $(CCOPTS) -o show_tree show_tree.cpp output/matlabLexer.cpp output/matlabParser.cpp $(CLIBS) -lantlr3c

#tests: gtest asttest
tests: asttest


gtest: gtest.c output/matlabLexer.h output/matlabParser.h output/matlabLexer.c output/matlabParser.c
	$(CC) $(CCOPTS) -o gtest gtest.c output/matlabLexer.c output/matlabParser.c $(CLIBS) \
	-lantlr3c -lreadline -lcurses

asttest: tests/astnodetest.cpp astnode.h astnode.c
	@[ ! -f ./astnodetest ] || rm astnodetest
	@echo "Compiling test"
	$(CPPC) $(CPPCOPTS) -o astnodetest tests/astnodetest.cpp astnode.c $(CLIBS) -lgtest -pthread
	@if [ -f ./astnodetest ]; then \
	  echo "Running test";\
	  ./astnodetest;\
	else \
	  echo "Compilation failed" ;\
	fi

clean:
	rm -f mparser show_tree gtest *.o *\~ #*

realclean:
	rm -f mparser show_tree gtest output/matlabLexer* output/matlabParser* output/matlab.tokens *.o *\~ #*
