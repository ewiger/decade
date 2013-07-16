#include <stdio.h>
#include "matlabLexer.h"
#include "matlabParser.h"

void chomp( char *str ) {
  int len = strlen( str );
  if ( len == 0 ) {
    return;
  }

  if ( str[len-1] == '\n' ) {
    str[len-1] = '\0';
    len--;
  }

  if ( len == 0 ) {
    return;
  }

  if ( str[len-1] == '\r' ) {
    str[len-1] = '\0';
    len--;
  }
}

/*
 * --------------------------------------------------------------------------------
 */

/* prints out an AST into a string without any pretty formatting */

void dense_recurse( pANTLR3_BASE_TREE tree, int depth, char **dest ) {
  pANTLR3_BASE_TREE t;
  ANTLR3_UINT32 i, n, cnt;

  if ( tree == NULL ) {
    return;
  }

  if ( tree->isNilNode(tree) == ANTLR3_FALSE ) {
    cnt = sprintf( *dest, "(%s", tree->toString( tree )->chars );
    *dest += cnt;
  } else {
    cnt = sprintf( *dest, "(<nil>" );
    *dest += cnt;
  }

  if ( tree->children != NULL ) {
    n = tree->children->size( tree->children );
    for ( i=0; i<n; i++ ) {
      t = (pANTLR3_BASE_TREE) tree->children->get( tree->children, i );
      dense_recurse( t, depth+1, dest );
    }
  }

  cnt = sprintf( *dest, ")" );
  *dest += cnt;
}

static void my_error_printer( pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames ) {
  // do nothing
}

//
// =============================================================================
//

// returns the number of errors
int process_input( pANTLR3_INPUT_STREAM input, int verbose, char **dest ) {
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
    if ( verbose ) {
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
    }

    parser->free( parser );
    tokens->free( tokens );
    lex->free( lex );
    return 1;
  }

  //
  //  Ok!  We passed.  Now we can process the resulting AST.
  //

  dense_recurse( tree, 0, dest );

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

#define BUFSIZE 1000000

int main( int argc, char * argv[] ) {
  pANTLR3_INPUT_STREAM input;
  char *expr_buf, *dest_buf, *true_answer, *tmp_dest_buf;
  FILE *fp;
  char *rval, *str;
  int num_errs, lineno, no_compare;

  expr_buf = (char *)malloc( BUFSIZE );
  dest_buf = (char *)malloc( BUFSIZE );
  true_answer = (char *)malloc( BUFSIZE );

  if ( argc != 2 && argc != 3 ) {
    printf( "usage: run_tests fn [-x]\n" );
    printf( "  use the -x to not perform AST comparisons.\n" );
    printf( "  (useful for generating the test file in the first place)\n" );
    return 0;
  }

  no_compare = 0;
  if ( argc == 3 && ( strcmp(argv[2],"-x") == 0 ) ) {
    no_compare = 1;
  }

  fp = fopen( argv[1], "rb" );
  if ( fp == NULL ) {
    fprintf( stdout, "error opening %s\n", argv[1] );
    return 0;
  }

  lineno = 0;
  while ( 1 ) {
    lineno++;
    rval = fgets( expr_buf, BUFSIZE, fp );
    if ( rval == NULL ) {
      break;
    }

    if ( no_compare ) {
      printf( "%s", expr_buf );
    }

    // empty line
    if ( strlen(expr_buf) < 2 ) {
      continue;
    }
    if ( expr_buf[0] == '%' ) {
      continue;
    }

    if ( !no_compare ) {
      lineno++;
      rval = fgets( true_answer, BUFSIZE, fp );
      if ( strlen(true_answer) < 2 ) {
	fprintf( stderr, "Whoa! expecting true answer on line %d!\n", lineno );
	continue;
      }
      if ( true_answer[0] == '%' ) {
	fprintf( stderr, "Whoa! expecting true answer on line %d!\n", lineno );
	continue;
      }

      // snip off the newline
      chomp( true_answer );
    }

    input = antlr3NewAsciiStringInPlaceStream( expr_buf, strlen(expr_buf), NULL );
    tmp_dest_buf = dest_buf;
    num_errs = process_input( input, 1, &tmp_dest_buf );
    input->close( input );

    if ( num_errs != 0 ) {
      fprintf( stderr, "Parse error on line %d\n", lineno );
      continue;
    }

    if ( no_compare ) {
      printf( "%s\n", dest_buf );

    } else {
      if ( strcmp( dest_buf, true_answer ) != 0 ) {
	fprintf( stderr, "Whoa! AST mismatch on line %d\n", lineno );
	fprintf( stderr, "I parsed as:  [%s]\n", dest_buf );
	fprintf( stderr, "But expected: [%s]\n", true_answer );
	continue;
      }
    }

  }

  fclose( fp );

}
