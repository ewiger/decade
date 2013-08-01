
#
# An example Makefile illustrating dependencies among the different
# parts of mparser (which are pretty simple).
#
# Also illustrates how to compile gtest, a readline-based matlab
# parser useful for debugging.
#

ANTLR= /usr/local
CC= gcc
CPPC= g++
CCOPTS= -I$(ANTLR)/include -L$(ANTLR)/lib
CPPCOPTS= -I$(ANTLR)/include -L$(ANTLR)/lib
#MEX= mex
MEX=/usr/local/MATLAB/R2011a/bin/mex

all: frontend mex_frontend

frontend: parse_matlab

parse_matlab: parse_matlab.cpp mparser.hpp
	$(CPPC) $(CPPCOPTS) -DMPARSER_DLL -DMPARSER_DLL_EXPORTS -shared -Wl,-soname,mparser -o mparser.so -fPIC parse_matlab.cpp

mex_frontend: gtest mparser show_tree

mparser: mparser.cpp output/matlabLexer.h output/matlabParser.h output/matlabLexer.cpp output/matlabParser.cpp
	$(MEX) $(CCOPTS) -o mparser mparser.cpp output/matlabLexer.cpp output/matlabParser.cpp $(CLIBS) -lantlr3c

show_tree: show_tree.cpp output/matlabLexer.h output/matlabParser.h output/matlabLexer.cpp output/matlabParser.cpp
	$(MEX) $(CCOPTS) -o show_tree show_tree.cpp output/matlabLexer.cpp output/matlabParser.cpp $(CLIBS) -lantlr3c

gtest: gtest.c output/matlabLexer.h output/matlabParser.h output/matlabLexer.c output/matlabParser.c
	$(CC) $(CCOPTS) -o gtest gtest.c output/matlabLexer.c output/matlabParser.c $(CLIBS) -lantlr3c -lreadline -lcurses


clean:
	rm -f mparser show_tree gtest *.o *\~ #*

realclean:
	rm -f mparser show_tree gtest output/matlabLexer* output/matlabParser* output/matlab.tokens *.o *\~ #*
