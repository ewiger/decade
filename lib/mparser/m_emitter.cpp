#include "m_parser.hpp"

/*
 * ----------------------------------------------------------------------------
 */

/*

These are the kinds of expression-y things we know about:

binary_operators:
  LOG_OR
  LOG_AND
  BIN_OR
  BIN_AND
  NEQ | DOUBLE_EQ | GRTE | GRT | LSTE | LST
  COLON
  PLUS | MINUS
  LEFTDIV | RIGHTDIV | TIMES | EL_LEFTDIV | EL_RIGHTDIV | EL_TIMES
  EXP | EL_EXP

unary operators:
  postfix_operators:
    CCT | EL_CCT
  prefix_operators:
    PLUS | MINUS | NEG

base_expressions:
  ID (plus_indexers)
  INT
  FLOAT
  STRING
  anon_func_handle
  cell
  matrix
*/

int isUnaryOp( pTREE tree ) {
  ANTLR3_UINT32 type = TYPE( tree );

  if ( type == NEG || type == EL_CCT || type == CCT ) {
    return 1;
  }

  if ( ( type == PLUS || type == MINUS ) &&
       ( NUM_CHILDREN( tree ) == 1 ) ) {
    return 1;
  }

  return 0;
}

/* test whether a node represents a binary operation.
   note that assignment (single EQ) is not treated as a binary operator here -- this is for expressions.
*/
int isBinaryOp( pTREE tree ) {
  ANTLR3_UINT32 type = TYPE( tree );

  if ( type == LOG_OR ||
       type == LOG_AND ||
       type == BIN_OR ||
       type == BIN_AND ||
       type == NEQ ||
       type == DOUBLE_EQ ||
       type == GRTE ||
       type == GRT ||
       type == LSTE ||
       type == LST ||
       type == COLON ||
       type == LEFTDIV ||
       type == RIGHTDIV ||
       type == TIMES ||
       type == EL_LEFTDIV ||
       type == EL_RIGHTDIV ||
       type == EL_TIMES ||
       type == EXP ||
       type == EL_EXP ) {
    return 1;
  }

  if ( ( type == PLUS || type == MINUS ) &&
       ( NUM_CHILDREN( tree ) == 2 ) ) {
    return 1;
  }

  return 0;
}


void MEmitter::captureEmissions(ParsingResult *result)
{
  this->result = result;
}

void MEmitter::walkTree(pTREE tree)
{
if ( tree == NULL ) {
    this->emitNull();
    return;
  }

  if ( isBinaryOp( tree ) ) {
    this->emitBinaryOp( tree );
    return;
  }

  if ( isUnaryOp( tree ) ) {
    this->emitUnaryOp( tree );
    return;
  }

  switch ( TYPE( tree ) ) {

  case PROGRAM:         this->emitProgram( tree ); break;
  case FUNCTION:        this->emitFunction( tree ); break;
  case IF:              this->emitIf( tree ); break;
  case ELSEIF:          this->emitElseif( tree ); break;
  case WHILE:           this->emitWhile( tree ); break;
  case FOR:             this->emitFor( tree ); break;
  case SWITCH:          this->emitSwitch( tree ); break;
  case ASSIGN:          this->emitAssign( tree ); break;
  case STATEMENT_LIST:  this->emitCellarray( tree, 0 ); break;
  case EXPR_STMT:       this->emitExprStmt( tree ); break;

  case APPLY:           this->emitApply( tree ); break;
  case CELLACCESS:      this->emitCellapply( tree ); break;

  case FIELDACCESS:     this->emitFieldaccess( tree ); break;
  case DYNFIELDACCESS:  this->emitDynfieldaccess( tree ); break;

  case PARAMETER_LIST:  this->emitStringarray( tree, 0 ); break;
  case FUNCTION_RETURN: this->emitStringarray( tree, 0 ); break;
  case FUNCTION_PARAMETER_LIST:
                        this->emitCellarray( tree, 0 ); break;

  case CELL:            this->emitCell( tree ); break;
  case MATRIX:          this->emitMatrix( tree ); break;
  case VECTOR:          this->emitVector( tree ); break;

  case ID:              this->emitId( tree ); break;
  case ID_NODE:         this->emitIdNode( tree ); break;

  case INT:             this->emitInt( tree ); break;
  case FLOAT:           this->emitFloat( tree ); break;
  case STRING:          this->emitString( tree ); break;

  case AT:              this->emitAtOperator( tree ); break;

  case CASE:            this->emitCase( tree ); break;

  case RETURNS:         this->emitReturn( tree ); break;
  case CONTINUE:        this->emitContinue( tree ); break;
  case BREAK:           this->emitBreak( tree ); break;
  case CLEAR:           this->emitClear( tree ); break;

  case PARENS:          this->emitParens( tree ); break;

  case GLOBAL:          this->emitGlobal( tree ); break;
  case PERSISTENT:      this->emitPersistent( tree ); break;

  case TRY:             this->emitTry( tree ); break;
  case CATCH:           this->emitCatch( tree ); break;

  /* These are empty container nodes. */
  case EXPRESSION:      this->walkTree( CHILD( tree, 0 ) ); break;
  case RHS:             this->walkTree( CHILD( tree, 0 ) ); break;
  case LHS:             this->walkTree( CHILD( tree, 0 ) ); break;
  case ELSE:            this->walkTree( CHILD( tree, 0 ) ); break;
  case OTHERWISE:       this->walkTree( CHILD( tree, 0 ) ); break;

  case NULL_STMT:       this->emitNull(); break;

  default:
    fprintf( stderr, "Whoa!  Unknown node type %d in the ANTLR AST!\n", TYPE(tree) );
    this->emitNull();
    return;
  }
}
