/* gtest.c
 *
 * Copyright(C) 2011 David Wingate
 *
 * This file is part of the mparser package, which is licensed under
 * the MIT license.  See the file COPYING for details.
 */

/*

a simple readline-enabled interactive utility for entering expressions and
spitting out pretty ASTs for visual inspection.

 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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

int process_input( pANTLR3_INPUT_STREAM input ) {
  pmatlabLexer lex;
  pANTLR3_COMMON_TOKEN_STREAM tokens;
  pmatlabParser parser;
  pANTLR3_BASE_TREE tree;

  //
  //  Connect it up: input -> lexer -> tokenstream -> parser
  //

  lex    = matlabLexerNew( input );
  tokens = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE(lex) );
  parser = matlabParserNew( tokens );

  // install a null error printer - we'll handle this in a custom way.
  parser->pParser->rec->displayRecognitionError = my_error_printer;

  //
  //  Do the parse!
  //

  matlabParser_program_return langAST = parser->program( parser );
  tree = langAST.tree;

  //
  //  Check for errors
  //

  int num_errs = parser->pParser->rec->getNumberOfSyntaxErrors( parser->pParser->rec );

  if ( num_errs != 0 ) {
    printf( "FOUND %d ERRORS\n", num_errs );
    pANTLR3_EXCEPTION ex = parser->pParser->rec->state->exception;
    fprintf( stderr, "Parse error at line %d, char %d\n", 
	     ex->line, ex->charPositionInLine );

    pANTLR3_STRING ttext;
    pANTLR3_COMMON_TOKEN theToken;
    theToken = (pANTLR3_COMMON_TOKEN)(ex->token);
    ttext = theToken->toString(theToken);
    fprintf( stderr, "Message: %s\n", (char *)( ex->message ) );
    fprintf( stderr, "Token: %s\n", ttext->chars );

    parser->free( parser );
    tokens->free( tokens );
    lex->free( lex );
    return 1;
  }

  //
  //  Ok!  We passed.  Now we can process the resulting AST.
  //

  //  printf("Nodes: %s\n", langAST.tree->toStringTree(langAST.tree)->chars);

  recurse( tree, 0 );

  //  printf( "[%s]\n", langAST.start->getText( langAST.start ) );
  //  printf( "[%s]\n", langAST.start->tokText.chars );

  //
  // Must manually clean up
  //

  parser->free( parser );
  tokens->free( tokens );
  lex->free( lex );

  return 0;
}

//
// =============================================================================
//

int main( int argc, char * argv[] ) {
  pANTLR3_INPUT_STREAM input;
  char *buf, *tmp;
  int len;

  if ( argc == 2 ) {
    input  = antlr3FileStreamNew( (pANTLR3_UINT8)argv[1], ANTLR3_ENC_8BIT );
    process_input( input );
    input->close( input );

  } else {

    while ( 1 ) {
      buf = readline( "> " );
      if ( buf == NULL ) {
      	printf( "\n" );
      	continue;
      }
      if ( strlen(buf) == 0 ) {
	continue;
      }

      // tack on a semicolon for parsing completeness.
      len = strlen( buf );
      tmp = (char *)malloc( len + 2 );
      memcpy( tmp, buf, len );
      tmp[ len ] = ';';
      tmp[ len+1 ] = '\0';
      add_history( buf ); // add the one without the semicolon to the history

      //input = antlr3NewAsciiStringInPlaceStream( tmp, len+1, NULL );
      input = antlr3StringStreamNew( tmp, ANTLR3_ENC_8BIT, len+1, NULL );
      process_input( input );
      input->close( input );

      free( tmp );
    }

  }
}
