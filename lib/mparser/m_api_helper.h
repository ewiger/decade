#ifndef __M_API_HELPER_H_
#define __M_API_HELPER_H_

/* Define M_PARSER API helpers */

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define M_PARSER_HELPER_DLL_IMPORT __declspec(dllimport)
  #define M_PARSER_HELPER_DLL_EXPORT __declspec(dllexport)
  #define M_PARSER_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define M_PARSER_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define M_PARSER_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define M_PARSER_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define M_PARSER_HELPER_DLL_IMPORT
    #define M_PARSER_HELPER_DLL_EXPORT
    #define M_PARSER_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define M_PARSER_API and M_PARSER_LOCAL.
// M_PARSER_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// M_PARSER_LOCAL is used for non-api symbols.

#ifdef M_PARSER_DLL // defined if M_PARSER is compiled as a DLL
  #ifdef M_PARSER_DLL_EXPORTS // defined if we are building the M_PARSER DLL (instead of using it)
    #define M_PARSER_API M_PARSER_HELPER_DLL_EXPORT
  #else
    #define M_PARSER_API M_PARSER_HELPER_DLL_IMPORT
  #endif // M_PARSER_DLL_EXPORTS
  #define M_PARSER_LOCAL M_PARSER_HELPER_DLL_LOCAL
#else // M_PARSER_DLL is not defined: this means M_PARSER is a static lib.
  #define M_PARSER_API
  #define M_PARSER_LOCAL
#endif // M_PARSER_DLL

/* End of M_PARSER API helpers */

#endif //__M_API_HELPER_H