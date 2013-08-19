#ifndef __M_PARSER_H_
#define __M_PARSER_H_


#include "m_api_helper.h"

#include <string>
#include "output/matlabLexer.h"
#include "output/matlabParser.h"

/* some common references */
#define NUM_CHILDREN(x) (x)->children->size( (x)->children )
#define CHILD(x,y) (pTREE) (x)->children->get( (x)->children, y )
#define TYPE(x) (x)->getType( (x) )
#define TEXT(x) (char *)( (x)->toString( (x) )->chars )


/* a shortcut typedef */
typedef pANTLR3_BASE_TREE pTREE;

/* Returned as parsing result */
typedef struct ParsingResult_struct {
  int error_count;
  //pAstNode ast; // See astnode.h
  const char *ast; // JSON AST
  /* in case of error, these members get populated. */
  int line, char_pos;
  char *msg, *token;
} M_PARSER_API ParsingResult;


/**
 * Tree visitor pattern with events emission.
 *
 */
class M_PARSER_API MEmitter {
  private:
    ParsingResult *result;
  public:
    virtual void captureEmissions(ParsingResult *result);

    virtual void walkTree(pTREE tree);

    virtual void emitProgram(pTREE tree) = 0;
    virtual void emitFunction(pTREE tree) = 0;
    virtual void emitIf(pTREE tree) = 0;
    virtual void emitElseif(pTREE tree) = 0;
    virtual void emitWhile(pTREE tree) = 0;
    virtual void emitFor(pTREE tree) = 0;
    virtual void emitSwitch(pTREE tree) = 0;
    virtual void emitAssign(pTREE tree) = 0;
    virtual void emitExprStmt(pTREE tree) = 0;

    virtual void emitBinaryOp(pTREE tree) = 0;
    virtual void emitUnaryOp(pTREE tree) = 0;

    virtual void emitApply(pTREE tree) = 0;
    virtual void emitCellapply(pTREE tree) = 0;

    virtual void emitFieldaccess(pTREE tree) = 0;
    virtual void emitDynfieldaccess(pTREE tree) = 0;

    virtual void emitCellarray(pTREE tree, int offset) = 0;
    virtual void emitStringarray(pTREE tree, int offset) = 0;

    virtual void emitCell(pTREE tree) = 0;
    virtual void emitMatrix(pTREE tree) = 0;
    virtual void emitVector(pTREE tree) = 0;

    virtual void emitId(pTREE tree) = 0;
    virtual void emitIdNode(pTREE tree) = 0;

    virtual void emitInt(pTREE tree) = 0;
    virtual void emitFloat(pTREE tree) = 0;
    virtual void emitString(pTREE tree) = 0;

    virtual void emitAtOperator(pTREE tree) = 0;

    virtual void emitCase(pTREE tree) = 0;

    virtual void emitReturn(pTREE tree) = 0;
    virtual void emitContinue(pTREE tree) = 0;
    virtual void emitBreak(pTREE tree) = 0;
    virtual void emitClear(pTREE tree) = 0;

    virtual void emitParens(pTREE tree) = 0;

    virtual void emitGlobal(pTREE tree) = 0;
    virtual void emitPersistent(pTREE tree) = 0;

    virtual void emitTry(pTREE tree) = 0;
    virtual void emitCatch(pTREE tree) = 0;

    virtual void emitNull() = 0;
};


class M_PARSER_API MParser {
    private:
        ParsingResult result;
        MEmitter *emitter;
    public:
        MParser(MEmitter *emitter);
        void reportError(const char *message);
        void reportError(std::string message, bool die);
        void reportError(const char *message, bool die);
        ParsingResult parseInput(pANTLR3_INPUT_STREAM input);
        void parseFile(const char *mfilepath);
        ParsingResult getResult();
};




#endif //__M_PARSER_H__
