/* show_tree.cpp
 *
 * Copyright(C) 2011 David Wingate
 *
 * This file is part of the mparser package, which is licensed under
 * the MIT license.  See the file COPYING for details.
 */

/*

A mex file which accepts a string as input and returns a deeply
nested set of structures and cell arrays representing a parse of
the string.  The parse is represented as an AST.

 */

#include <stdio.h>

#include "mex.h"

#include "output/matlabLexer.h"
#include "output/matlabParser.h"

/*
 * ----------------------------------------------------------------------------
 */

/* prints out an AST with pretty formatting */

void recurse( pANTLR3_BASE_TREE tree, int depth ) {
  pANTLR3_BASE_TREE t;
  ANTLR3_UINT32 i, n;

  if ( tree == NULL ) {
    return;
  }

  for ( i=0; i<depth; i++ ) {
    printf( "  " );
  }

  if ( tree->isNilNode(tree) == ANTLR3_FALSE ) {
    printf( "(%s", tree->toString( tree )->chars );
  } else {
    printf( "(<nil>" );
  }

  //  printf( "type=%d", tree->getType( tree ) );

  if ( tree->children != NULL ) {
    n = tree->children->size( tree->children );
    if ( n > 0 ) {
      printf( "\n" );
    }
    for ( i=0; i<n; i++ ) {
      t = (pANTLR3_BASE_TREE) tree->children->get( tree->children, i );
      recurse( t, depth+1 );
    }
    if ( n > 0 ) {
      for ( i=0; i<depth; i++ ) {
	printf( "  " );
      }
    }
  }

  printf( ")\n" );
}

static void my_error_printer( pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames ) {
  // do nothing
}

//
// =============================================================================
//

void process_input( pANTLR3_INPUT_STREAM input ) {
  pmatlabLexer lex;
  pANTLR3_COMMON_TOKEN_STREAM tokens;
  pmatlabParser parser;

  //
  //  Connect it up: input -> lexer -> tokenstream -> parser
  //

  lex    = matlabLexerNew( input );
  if ( lex == NULL ) {
    mexErrMsgTxt( "unable to allocate lexer!\n" );
  }

  tokens = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE(lex) );
  if ( tokens == NULL ) {
    mexErrMsgTxt( "unable to create token stream!\n" );
  }

  parser = matlabParserNew( tokens );
  if ( parser == NULL ) {
    mexErrMsgTxt( "unable to create parser!\n" );
  }

  // install a null error printer - we'll handle this in a custom way.
  parser->pParser->rec->displayRecognitionError = my_error_printer;

  //
  //  Do the parse!
  //

  matlabParser_program_return langAST = parser->program( parser );

  //
  //  Check for errors
  //

  int num_errs = parser->pParser->rec->getNumberOfSyntaxErrors( parser->pParser->rec );

  if ( num_errs != 0 ) {

    pANTLR3_STRING ttext;
    pANTLR3_COMMON_TOKEN theToken;
    pANTLR3_EXCEPTION ex;

    ex = parser->pParser->rec->state->exception;
    theToken = (pANTLR3_COMMON_TOKEN)(ex->token);
    ttext = theToken->toString(theToken);

    // TODO: make this more informative!

    mexErrMsgTxt( "parse error" );

    // retval.error_count = num_errs;
    // retval.line = ex->line;
    // retval.pchar = ex->charPositionInLine;
    // retval.msg = strdup( (char *)( ex->message ) );
    // retval.token = strdup( (char *)( ttext->chars ) );

    parser->free( parser );
    tokens->free( tokens );
    lex->free( lex );

    return;
  }

  //
  //  Ok!  We passed.  Now we can process the resulting AST.
  //

  recurse( langAST.tree, 0 );

  //
  // Must manually clean up
  //

  parser->free( parser );
  tokens->free( tokens );
  lex->free( lex );
}

//
// =============================================================================
//

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] ) {

  pANTLR3_INPUT_STREAM input;
  char *buf;

  if ( (nrhs != 1 && nrhs != 2 ) || !mxIsChar( prhs[0] ) || nlhs != 0 ) {
    mexErrMsgTxt( "usage: show_tree( string [, is_filename] )" );
  }

  buf = mxArrayToString( prhs[0] );
  if ( buf == NULL ) {
    mexErrMsgTxt( "error converting input argument to string!" );
  }

  if ( nrhs == 2 ) {
    input  = antlr3AsciiFileStreamNew( (pANTLR3_UINT8)buf );
    if ( input == NULL ) {
      mexErrMsgTxt( "unable to open file!\n" );
    }
  } else {
    input = antlr3NewAsciiStringInPlaceStream( (pANTLR3_UINT8)buf, strlen(buf), NULL );
    if ( input == NULL ) {
      mexErrMsgTxt( "unable to create input stream from string!\n" );
    }
  }
  
  process_input( input );

  input->close( input );

  free( buf );
}
