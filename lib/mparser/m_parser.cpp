/* m_parser.cpp
 *
 * Copyright(C) 2013 Yauhen Yakimovich,
 *              2011 David Wingat
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
#include "json_emitter.hpp"

using namespace std;


static void my_error_printer( pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames )
{
  /* do nothing */
}

MParser::MParser(MEmitter *emitter) { this-> emitter = emitter; }
void MParser::reportError(const char *message) { this->reportError(message, false); }
void MParser::reportError(std::string message, bool die) { this->reportError(message.c_str(), die); }
ParsingResult MParser::getResult() { return this->result; }

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
     *  Ok!  We passed.  Now we can process the resulting AST and emit it into
     *  appropriate events.
     */

    // We skip extra repacking step and take ANLTR Tree as it is, even if it is
    // not extremely nice to work with
    this->emitter->captureEmissions(&retval);
    this->emitter->walkTree(langAST.tree);

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


extern "C" {

    ParsingResult parse_matlab(const char* mfilepath)
    {
        cout << "parse_matlab: \n";
        JsonEmitter emitter;
        MParser parser(&emitter);
        parser.parseFile(mfilepath);
        return parser.getResult();
    }
}

