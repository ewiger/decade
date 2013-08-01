
#
# An example Makefile illustrating dependencies among the different
# parts of mparser (which are pretty simple).
#
# Also illustrates how to compile gtest, a readline-based matlab
# parser useful for debugging.
#

ANTLR= /usr/local
CC= gcc
CCOPTS= -I$(ANTLR)/include -L$(ANTLR)/lib
#MEX= mex
MEX=/usr/local/MATLAB/R2011a/bin/mex

all: gtest mparser show_tree

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
