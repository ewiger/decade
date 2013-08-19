#ifndef __MPARSER_H_
#define __MPARSER_H_

#include "astnode.h"

// ANTLR code-generated parser
#include "output/matlabLexer.h"
#include "output/matlabParser.h"

/* Define MPARSER API helpers */

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define MPARSER_HELPER_DLL_IMPORT __declspec(dllimport)
  #define MPARSER_HELPER_DLL_EXPORT __declspec(dllexport)
  #define MPARSER_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define MPARSER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define MPARSER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define MPARSER_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define MPARSER_HELPER_DLL_IMPORT
    #define MPARSER_HELPER_DLL_EXPORT
    #define MPARSER_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define MPARSER_API and MPARSER_LOCAL.
// MPARSER_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// MPARSER_LOCAL is used for non-api symbols.

#ifdef MPARSER_DLL // defined if MPARSER is compiled as a DLL
  #ifdef MPARSER_DLL_EXPORTS // defined if we are building the MPARSER DLL (instead of using it)
    #define MPARSER_API MPARSER_HELPER_DLL_EXPORT
  #else
    #define MPARSER_API MPARSER_HELPER_DLL_IMPORT
  #endif // MPARSER_DLL_EXPORTS
  #define MPARSER_LOCAL MPARSER_HELPER_DLL_LOCAL
#else // MPARSER_DLL is not defined: this means MPARSER is a static lib.
  #define MPARSER_API
  #define MPARSER_LOCAL
#endif // MPARSER_DLL

/* End of MPARSER API helpers */


/* Returned as parsing result */
typedef struct ParsingResult_struct {
  int error_count;
  //pAstNode ast; // See astnode.h
  const char *ast; // JSON AST
  /* in case of error, these members get populated. */
  int line, char_pos;
  char *msg, *token;
} ParsingResult;


class MPARSER_API MParser {
    private:
        ParsingResult result;
    public:
        void reportError(const char *message) { this->reportError(message, false); };
        void reportError(std::string message, bool die) { this->reportError(message.c_str(), die); };
        void reportError(const char *message, bool die);
        ParsingResult parseInput(pANTLR3_INPUT_STREAM input);
        void parseFile(const char *mfilepath);
        ParsingResult getResult() { return this->result; };
        void walkTree(pANTLR3_BASE_TREE tree);
};

#endif //__MPARSER_H
