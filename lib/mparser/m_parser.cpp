/* m_parser.cpp
 *
 * Copyright(C) 2013 Yauhen Yakimovich
 *
 * This file is part of the mparser package, which is licensed under
 * the MIT license.  See the file COPYING for details.
 */
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <boost/filesystem.hpp>
#include "m_parser.hpp"

using namespace std;


/* some common references */
#define NUM_CHILDREN(x) (x)->children->size( (x)->children )
#define CHILD(x,y) (pTREE) (x)->children->get( (x)->children, y )
#define TYPE(x) (x)->getType( (x) )
#define TEXT(x) (char *)( (x)->toString( (x) )->chars )


static void my_error_printer( pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames )
{
  /* do nothing */
}


void MParser::reportError(const char *message, bool die)
{
    cerr << message << "\n";
    exit(EXIT_FAILURE);
}


ParsingResult MParser::parseInput( pANTLR3_INPUT_STREAM input )
{
    pmatlabLexer lex;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    pmatlabParser parser;
    ParsingResult retval;

    retval.error_count = 0;
    retval.ast = NULL;
    retval.line = -1;
    retval.char_pos = -1;
    retval.msg = NULL;
    retval.token = NULL;

    /*
     *  Connect it up: input -> lexer -> tokenstream -> parser
     */

    lex = matlabLexerNew( input );
    if ( lex == NULL ) {
        this->reportError("unable to allocate lexer!", true); // and die afterwards
    }

    tokens = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE(lex) );
    if ( tokens == NULL ) {
        this->reportError("unable to create token stream!", true);
    }

    parser = matlabParserNew( tokens );
    if ( parser == NULL ) {
        this->reportError("unable to create parser!", true);
    }

    /* install a null error printer - we'll handle this in a custom way. */
    parser->pParser->rec->displayRecognitionError = my_error_printer;

    /*
     *  Do the parse!
     */

    matlabParser_program_return langAST = parser->program( parser );

    /*
     *  Check for errors
     */

    int num_errs = parser->pParser->rec->getNumberOfSyntaxErrors( parser->pParser->rec );

    if ( num_errs != 0 ) {

      pANTLR3_STRING ttext;
      pANTLR3_COMMON_TOKEN theToken;
      pANTLR3_EXCEPTION ex;

      ex = parser->pParser->rec->state->exception;
      theToken = (pANTLR3_COMMON_TOKEN)(ex->token);
      ttext = theToken->toString(theToken);

      retval.error_count = num_errs;
      retval.ast = NULL;
      retval.line = ex->line;
      retval.char_pos = ex->charPositionInLine;
      retval.msg = strdup( (char *)( ex->message ) );
      retval.token = strdup( (char *)( ttext->chars ) );

      parser->free( parser );
      tokens->free( tokens );
      lex->free( lex );

      return retval;
    }

    /*
     *  Ok!  We passed.  Now we can process the resulting AST into the
     *  appropriate matlab structures.
     */

    // We skip extra repacking step and take ANLTR Tree as it is, even if it is
    // not extremly nice to work with
    retval.ast = this->walkTree(langAST.tree);

    /*
     * Must manually clean up
     */

    parser->free( parser );
    tokens->free( tokens );
    lex->free( lex );

    return retval;
}

void MParser::parseFile(const char *mfilepath)
{
    pANTLR3_INPUT_STREAM input;
    if (!boost::filesystem::exists(mfilepath)) {
        stringstream message;
        message << "File does not exists: " << mfilepath << "\n";
        this->reportError(message.str(), true);
    }
    input  = antlr3FileStreamNew((pANTLR3_UINT8)mfilepath, ANTLR3_ENC_8BIT);
    this->result = this->parseInput(input);
}


void MParser::walkTree(pANTLR3_BASE_TREE tree)
{
if ( tree == NULL ) {
    return NULL;
  }

  if ( isBinaryOp( tree ) ) {
    return this->emitBinaryOp( tree );
  }

  if ( isUnaryOp( tree ) ) {
    return this->emitUnaryOp( tree );
  }

  switch ( TYPE( tree ) ) {

  case PROGRAM:        return this->emitProgram( tree );
  case FUNCTION:       return this->emitFunction( tree );
  case IF:             return this->emitIf( tree );
  case ELSEIF:         return this->emitElseif( tree );
  case WHILE:          return this->emitWhile( tree );
  case FOR:            return this->emitFor( tree );
  case SWITCH:         return this->emitSwitch( tree );
  case ASSIGN:         return this->emitAssign( tree );
  case STATEMENT_LIST: return this->emitCellarray( tree, 0 );
  case EXPR_STMT:      return this->emitExprStmt( tree );

  case APPLY:          return this->emitApply( tree );
  case CELLACCESS:     return this->emitCellapply( tree );

  case FIELDACCESS:    return this->emitFieldaccess( tree );
  case DYNFIELDACCESS: return this->emitDynfieldaccess( tree );

  case PARAMETER_LIST: return this->emitStringarray( tree, 0 );
  case FUNCTION_RETURN:return this->emitStringarray( tree, 0 );
  case FUNCTION_PARAMETER_LIST:
                       return this->emitCellarray( tree, 0 );

  case CELL:           return this->emitCell( tree );
  case MATRIX:         return this->emitMatrix( tree );
  case VECTOR:         return this->emitVector( tree );

  case ID:             return this->emitId( tree );
  case ID_NODE:        return this->emitIdNode( tree );

  case INT:            return this->emitInt( tree );
  case FLOAT:          return this->emitFloat( tree );
  case STRING:         return this->emitString( tree );

  case AT:             return this->emitAtOperator( tree );

  case CASE:           return this->emitCase( tree );

  case RETURNS:        return this->emitReturn( tree );
  case CONTINUE:       return this->emitContinue( tree );
  case BREAK:          return this->emitBreak( tree );
  case CLEAR:          return this->emitClear( tree );

  case PARENS:         return this->emitParens( tree );

  case GLOBAL:         return this->emitGlobal( tree );
  case PERSISTENT:     return this->emitPersistent( tree );

  case TRY:            return this->emitTry( tree );
  case CATCH:          return this->emitCatch( tree );

  /* These are empty container nodes. */
  case EXPRESSION:     return this->walkTree( CHILD( tree, 0 ) );
  case RHS:            return recurse( CHILD( tree, 0 ) );
  case LHS:            return recurse( CHILD( tree, 0 ) );
  case ELSE:           return recurse( CHILD( tree, 0 ) );
  case OTHERWISE:      return recurse( CHILD( tree, 0 ) );

  case NULL_STMT:      return NULL;

  default:
    fprintf( stderr, "Whoa!  Unknown node type %d in the ANTLR AST!\n", TYPE(tree) );
    return NULL;

  }
}


extern "C" {

    ParsingResult parse_matlab(const char* mfilepath)
    {
        cout << "parse_matlab: \n";
        MParser parser;
        parser.parseFile(mfilepath);
        return parser.getResult();
    }
}

