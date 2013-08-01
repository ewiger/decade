/* mparser.cpp
 *
 * Copyright(C) 2011 David Wingate
 *
 * This file is part of the mparser package, which is licensed under
 * the MIT license.  See the file COPYING for details.
 */

/*

A mex file which accepts a string (or filename) as input and returns a
deeply nested set of structures and cell arrays that form an abstract
syntax tree of the contents of the string (or file).

You might be wondering: doesn't ANTLR already spit out a parse tree?

Yes, it does.  But it's not a "nice" parse tree, in my opinion, for
various reasons -- it's fragile, there's superfluous nodes, optional
nodes create problems for reasonable AST manipulation, things aren't
named well, etc.

The point of mparser is to translate all that mess to give everything
reasonable names in a nice structure.  The idea is that the structure
which is passed back up to Matlab looks clean, and which isn't
dependent on any details of the way that ANTLR represents its parse
tree -- thereby insulating Matlab from the details of the Antlr parser
implementation.

 */

#include <stdio.h>
#include <stdarg.h>

#include "mex.h"

#include "output/matlabLexer.h"
#include "output/matlabParser.h"

typedef struct ast_t {
  int error_count;
  mxArray *ast;
  /* in case of error, these members get populated. */
  int line, pchar;
  char *msg, *token;
} ast_t;

/* a simple structure to help us define nodes */
typedef struct field_t {
  const char *field_name;
  ANTLR3_UINT32 node_type;
} field_t;

/* a shortcut typedef */
typedef pANTLR3_BASE_TREE pTREE;

/* our lone prototype.  I'm such a lazy coder. */
mxArray *recurse( pTREE tree );

/*
 * ----------------------------------------------------------------------------
 */

/* a MEX assertion macro */
#define MASSERT(x) if ( !(x) ) { \
  char errmsgbuf[1024];\
  sprintf( errmsgbuf, "assertion failed at file %s, line %d", __FILE__, __LINE__ );\
  mexErrMsgTxt( errmsgbuf );\
}

#define DIE_IF_NULL(x) \
  if ( x == NULL ) { \
  char errmsgbuf[1024];\
  sprintf( errmsgbuf, "Out of memory at file %s, line %d", __FILE__, __LINE__ );\
  mexErrMsgTxt( errmsgbuf );\
}

/* some common references */
#define NUM_CHILDREN(x) (x)->children->size( (x)->children )
#define CHILD(x,y) (pTREE) (x)->children->get( (x)->children, y )
#define TYPE(x) (x)->getType( (x) )
#define TEXT(x) (char *)( (x)->toString( (x) )->chars )

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

int is_unary_op( pTREE tree ) {
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
int is_binary_op( pTREE tree ) {
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

/*
 * ----------------------------------------------------------------------------
 */

/* three convenience functions */

/*

This is used when you have detailed knowledge of the AST that ANTLR is
spitting out.  You have to know exactly which child number corresponds
to the thing you want, and what type it is.  This is pretty fragile,
because if someone modifies the ANTLR grammar and those positions
change, this will break.  But it's convenient.

 */

void direct_set_child( mxArray *retval, const char *field_name, pTREE tree, int child_index, ANTLR3_UINT32 type ) {
  /* make sure we've got the right kind of node! */
  if ( type != EOF ) {
    if ( TYPE(CHILD(tree,child_index)) != type ) {
      char buf[1024];
      sprintf( buf, "Expecting node type %d, but got type %d\n", type, TYPE(CHILD(tree,child_index)));
      mexErrMsgTxt( buf );
    }
  }
  /* go ahead and set the field value */
  mxSetField( retval, 0, field_name, recurse( CHILD(tree,child_index) ) );  
}

void set_string( mxArray *retval, const char *field, const char *val ) {
  mxSetField( retval, 0, field, mxCreateString( val ) );
}

void set_type( mxArray *retval, const char *val ) {
  set_string( retval, "type", val );
}

/*
 * ----------------------------------------------------------------------------
 */

/*

Creates a matlab structure with a bunch of named fields.

 */

mxArray *create_typed_node( const char *name, int num_fields, const char **other_field_names) {
  mxArray *retval;
  const char **field_names;
  int i;

  field_names = (const char **)malloc( sizeof(const char *)*(num_fields+1) );
  DIE_IF_NULL( field_names );

  field_names[0] = "type";

  for ( i=0; i<num_fields; i++ ) {
    field_names[i+1] = other_field_names[i];
  }

  retval = mxCreateStructMatrix( 1, 1, num_fields+1, field_names );

  free( field_names );

  set_type( retval, name );

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

/* a little bit of macro foo to make code below more readable. */

/*
  The PP_NARG macro returns the number of arguments that have been
  passed to it.

  Code found on this link:
http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5?pli=1

  */

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 

/* CTN stands for "create typed node".  the idea is that we want to be
   able to declare and initialize a structure easily... the tricky bit
   is specifying a varible number of field names.  So, this declares a
   matlab structure called "retval", allocates it, and populates it
   with a bunch of fields. */

#define CTN( name, other_field_names... ) \
  const char *ctn_field_names[ PP_NARG(other_field_names) ] = { other_field_names }; \
  mxArray *retval = create_typed_node( name, PP_NARG(other_field_names), ctn_field_names )

/*
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */

/* this function is a bit of a convenience function also.  The idea is
   that we want to specify a bunch of fields in the matlab structure,
   and a bunch of corresponding node types in the antlr AST.  Then we
   want to match them up one by one. */

mxArray *make_node( pTREE tree, const char *name, int num_fields, field_t *fields) {
  mxArray *retval;
  int i, j;
  pTREE t;

  const char **field_names;
  field_names = (const char **)malloc( sizeof(const char *)*(num_fields+1) );
  DIE_IF_NULL( field_names );
  field_names[0] = "type";
  for ( i=0; i<num_fields; i++ ) {
    field_names[i+1] = fields[i].field_name;
  }
  retval = mxCreateStructMatrix( 1, 1, num_fields+1, field_names );
  free( field_names );

  set_type( retval, name );

  /* now, find each specified field */

  MASSERT( tree->children != NULL );
  int num_children = NUM_CHILDREN( tree );

  for ( i=0; i<num_fields; i++ ) {

    /* search the children of this tree node to find the field specified by the field descriptor */
    for ( j=0; j<num_children; j++ ) {
      t = CHILD( tree, j );
      if ( TYPE(t) == fields[i].node_type ) {
	mxSetField( retval, 0, fields[i].field_name, recurse( t ) );
	break;
      }
    }

    /* not found -- but this is not an error.  we assume all fields are optional */
  }

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

int find_semi( pTREE tree ) {
  /* is one of the children of this node a semicolon?  if so, which one?
     returns the index of the child, or -1 if none is found.
   */
  if ( tree->children == NULL ) {
    return -1;
  }

  int n = NUM_CHILDREN( tree );
  int i;
  for ( i=0; i<n; i++ ) {
    if ( TYPE(CHILD(tree,i)) == SEMI ) {
      return i;
    }
  }

  return -1;
}

/*
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */

mxArray *make_function( pTREE tree ) {
  field_t fs[] = {{ "name", ID },
		  { "return_params", FUNCTION_RETURN },
		  { "input_params", PARAMETER_LIST },
		  { "statements", STATEMENT_LIST }};
  return make_node( tree, "function_definition", 4, fs );
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_while( pTREE tree ) {
  field_t fs[] = {{ "condition", EXPRESSION },
		  { "statements", STATEMENT_LIST }};
  return make_node( tree, "while_statement", 2, fs );
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_for( pTREE tree ) {
  field_t fs[] = {{ "iterator", ID },
		  { "values", EXPRESSION },
		  { "statements", STATEMENT_LIST }};
  return make_node( tree, "for_statement", 3, fs );
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_if( pTREE tree ) {
  CTN( "if_statement", "condition", "statements", "elseifs", "else" );

  direct_set_child( retval, "condition", tree, 0, EXPRESSION );
  direct_set_child( retval, "statements", tree, 1, STATEMENT_LIST );

  int n = NUM_CHILDREN( tree );

  int elseif_cnt = 0;
  for ( int i=2; i<n; i++ ) {
    int type = TYPE( CHILD(tree,i) );

    if ( type == ELSEIF ) {
      elseif_cnt++;

    } else if ( type == ELSE ) {
      direct_set_child( retval, "else", tree, i, ELSE );

    }
  }

  if ( elseif_cnt > 0 ) {
    mxArray *tmp = mxCreateCellMatrix( 1, elseif_cnt );

    int elseif_cnter = 0;
    for ( int i=2; i<n; i++ ) {
      if ( TYPE( CHILD(tree,i) ) == ELSEIF ) {
	mxSetCell( tmp, elseif_cnter, recurse( CHILD(tree,i) ) );
	elseif_cnter++;
      }
    }

    mxSetField( retval, 0, "elseifs", tmp );
  }

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_elseif( pTREE tree ) {
  field_t fs[] = {{ "expression", EXPRESSION },
                  { "statements", STATEMENT_LIST }};
  return make_node( tree, "elseif", 2, fs );
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_switch( pTREE tree ) {
  CTN( "switch_statement", "expression", "cases", "otherwise" );

  direct_set_child( retval, "expression", tree, 0, EXPRESSION );

  int n = NUM_CHILDREN( tree );
  int case_cnt = 0;

  for ( int i=1; i<n; i++ ) {
    int type = TYPE( CHILD(tree,i) );
    if ( type == CASE ) {
      case_cnt++;
    } else if ( type == OTHERWISE ) {
      direct_set_child( retval, "otherwise", tree, i, OTHERWISE );
    }
  }

  if ( case_cnt > 0 ) {
    mxArray *tmp = mxCreateCellMatrix( 1, case_cnt );

    int case_cnter = 0;
    for ( int i=1; i<n; i++ ) {
      if ( TYPE( CHILD(tree,i) ) == CASE ) {
	mxSetCell( tmp, case_cnter, recurse( CHILD(tree,i) ) );
	case_cnter++;
      }
    }

    mxSetField( retval, 0, "cases", tmp );
  }

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_try( pTREE tree ) {
  field_t fs[] = {{ "statements", STATEMENT_LIST },
		  { "catch", CATCH }};
  return make_node( tree, "try_statement", 2, fs );
}

mxArray *make_catch( pTREE tree ) {
  field_t fs[] = {{ "statements", STATEMENT_LIST },
		  { "id", ID }};
  return make_node( tree, "catch_statement", 2, fs );
}

/*
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 */

/* just translates all of the children into a simple cell array of
strings.  in a horrible hack, we allow the caller to skip the last
item.  This is mostly useful for skipping semicolons.
 */
mxArray* make_stringarray( pTREE tree, int skip_last_one ) {
  mxArray *retval;
  int i, n;
  pTREE t;

  MASSERT( tree->children != NULL );

  n = NUM_CHILDREN( tree );
  if ( skip_last_one ) {
    n--;
  }

  if ( n <= 0 ) {
    return NULL;
  }

  retval = mxCreateCellMatrix( 1, n );

  for ( i=0; i<n; i++ ) {
    t = CHILD( tree, i );
    mxSetCell( retval, i, mxCreateString( TEXT(t) ) );
  }

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_binary_op( pTREE tree ) {
  CTN( "binary_op", "op", "lhs", "rhs" );
  set_string( retval, "op", TEXT( tree ) );

  MASSERT( tree->children != NULL );
  direct_set_child( retval, "lhs", tree, 0, EOF );
  direct_set_child( retval, "rhs", tree, 1, EOF );

  return retval;
}

mxArray *make_unary_op( pTREE tree ) {
  CTN( "unary_op", "op", "lhs" );

  set_string( retval, "op", TEXT( tree ) );
  direct_set_child( retval, "lhs", tree, 0, EOF );

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray* make_cellarray( pTREE tree, int offset ) {
  mxArray *retval;
  int i, n;
  pTREE t;

  if ( tree == NULL || tree->children == NULL || NUM_CHILDREN(tree) - offset <= 0 ) {
    return NULL;
  }

  n = NUM_CHILDREN( tree );

  retval = mxCreateCellMatrix( 1, n - offset );
  for ( i=offset; i<n; i++ ) {
    t = CHILD( tree, i );
    mxSetCell( retval, i-offset, recurse( t ) );
  }

  return retval;
}


mxArray *make_expr_stmt( pTREE tree ) {
  CTN( "expression_stmt", "expression", "semicolon" );

  direct_set_child( retval, "expression", tree, 0, EXPRESSION );

  int has_semi = ( NUM_CHILDREN(tree) == 2 );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );

  return retval;
}

mxArray *make_id_node( pTREE tree ) {
  CTN( "id", "id", "indexers" );

  MASSERT( NUM_CHILDREN(tree) >= 1 );

  /* the first child is the actual identifier.  the remaining children are indexers. */
  pTREE t = CHILD( tree, 0 );
  mxSetField( retval, 0, "id", mxCreateString( TEXT(t) ) );

  if ( NUM_CHILDREN( tree ) > 1 ) {
    mxSetField( retval, 0, "indexers", make_cellarray( tree, 1 ) );
  }

  return retval;
}

mxArray *make_id( pTREE tree ) {
  /* we should have zero children.  any children of this node are
     indexers -- but then someone upstream should have called
     "make_id_node", not "make_id"... */
  MASSERT( tree->children == NULL || NUM_CHILDREN(tree) == 0 ); 
  mxArray *tmp = mxCreateString( TEXT(tree) );
  return tmp;
}

mxArray *make_assign( pTREE tree ) {
  CTN( "assignment_stmt", "lhs", "rhs", "semicolon" );

  MASSERT( tree->children != NULL );

  direct_set_child( retval, "lhs", tree, 0, LHS );
  direct_set_child( retval, "rhs", tree, 1, RHS );

  int has_semi = ( NUM_CHILDREN( tree ) == 3 );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

/* we store this as a string to retain all formatting / precision */
mxArray *make_float( pTREE tree ) {
  CTN( "constant", "ctype", "value" );
  set_string( retval, "ctype", "float" );
  set_string( retval, "value", TEXT(tree) );
  return retval;
}

/* strings come out of ANTLR surrounded by their single quote marks.
   we leave those in... */
mxArray *make_string( pTREE tree ) {
  CTN( "constant", "ctype", "value" );
  set_string( retval, "ctype", "string" );
  set_string( retval, "value", TEXT(tree) );
  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

/* one child: we have a basic function handle, like "@randn".
  two children: we have an anonymous function, such as "@(x,y)rand(x,y,3)"
*/
mxArray *make_at_operator( pTREE tree ) {
  int n = NUM_CHILDREN( tree );

  if ( n == 1 ) {
    CTN( "function_handle", "function_id" );
    pTREE t = CHILD( tree, 0 );
    mxSetField( retval, 0, "function_id", mxCreateString( TEXT( t ) ) );
    return retval;
  }

  if ( n == 2 ) {
    field_t fs[] = {{ "parameters", PARAMETER_LIST },
		    { "expression", EXPRESSION} };
    return make_node( tree, "anon_function", 2, fs );
  }

  fprintf( stderr, "whoa! I have %d children!\n", n );
  mexErrMsgTxt( "unknown AT node!" );
  return NULL;
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_fieldaccess( pTREE tree ) {
  field_t fs[] = {{ "id", ID }};
  return make_node( tree, "fieldaccess", 1, fs );
}

mxArray *make_dynfieldaccess( pTREE tree ) {
  field_t fs[] = {{ "expression", EXPRESSION }};
  return make_node( tree, "dynfieldaccess", 1, fs );
}

mxArray *make_apply( pTREE tree ) {
  field_t fs[] = {{ "parameter_list", FUNCTION_PARAMETER_LIST }};
  return make_node( tree, "apply", 1, fs );
}

mxArray *make_cellapply( pTREE tree ) {
  field_t fs[] = {{ "parameter_list", FUNCTION_PARAMETER_LIST }};
  return make_node( tree, "cellapply", 1, fs );
}

mxArray *make_case( pTREE tree ) {
  field_t fs[] = {{ "expression", EXPRESSION },
                  { "statements", STATEMENT_LIST }};
  return make_node( tree, "case", 2, fs );
}

/*
 * ----------------------------------------------------------------------------
 */

/* a cell is a list of vectors.  each vector is a list of expressions. */
mxArray *make_cell( pTREE tree ) {
  CTN( "cell", "vectors" );
  mxSetField( retval, 0, "vectors", make_cellarray( tree, 0 ) );
  return retval;
}

mxArray *make_matrix( pTREE tree ) {
  CTN( "matrix", "vectors" );
  mxSetField( retval, 0, "vectors", make_cellarray( tree, 0 ) );
  return retval;
}

mxArray *make_vector( pTREE tree ) {
  CTN( "vector", "expressions" );
  mxSetField( retval, 0, "expressions", make_cellarray( tree, 0 ) );
  return retval;
}


mxArray *make_return( pTREE tree ) {
  CTN( "return", "semicolon" );
  int has_semi = ( NUM_CHILDREN( tree ) == 1 );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );
  return retval;
}

mxArray *make_break( pTREE tree ) {
  CTN( "break", "semicolon" );
  int has_semi = ( NUM_CHILDREN( tree ) == 1 );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );
  return retval;
}

mxArray *make_continue( pTREE tree ) {
  CTN( "continue", "semicolon" );
  int has_semi = ( NUM_CHILDREN( tree ) == 1 );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );
  return retval;
}

mxArray *make_parens( pTREE tree ) {
  CTN( "parens", "expression" );
  direct_set_child( retval, "expression", tree, 0, EXPRESSION );
  return retval;
}


/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_global( pTREE tree ) {
  CTN( "global", "ids", "semicolon" );

  int has_semi = ( find_semi( tree ) != -1 );
  mxSetField( retval, 0, "ids", make_stringarray( tree, has_semi ) );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );

  return retval;
}

mxArray *make_persistent( pTREE tree ) {
  CTN( "persistent", "ids", "semicolon" );

  int has_semi = ( find_semi( tree ) != -1 );
  mxSetField( retval, 0, "ids", make_stringarray( tree, has_semi ) );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );

  return retval;
}

mxArray *make_clear( pTREE tree ) {
  CTN( "clear", "ids", "semicolon" );

  int has_semi = ( find_semi( tree ) != -1 );
  mxSetField( retval, 0, "ids", make_stringarray( tree, has_semi ) );
  mxSetField( retval, 0, "semicolon", mxCreateLogicalScalar( has_semi ) );

  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

mxArray *make_program( pTREE tree ) {
  CTN( "program", "statements" );
  direct_set_child( retval, "statements", tree, 0, STATEMENT_LIST );
  return retval;
}

/*
 * ----------------------------------------------------------------------------
 */

/*

The workhorse.  Given an ANTLR AST, translate it into something that's
recognizable in Matlab.

 */

mxArray *recurse( pTREE tree ) {

  if ( tree == NULL ) {
    return NULL;
  }

  if ( is_binary_op( tree ) ) {
    return make_binary_op( tree );
  }

  if ( is_unary_op( tree ) ) {
    return make_unary_op( tree );
  }

  switch ( TYPE( tree ) ) {

  case PROGRAM:        return make_program( tree );
  case FUNCTION:       return make_function( tree );
  case IF:             return make_if( tree );
  case ELSEIF:         return make_elseif( tree );
  case WHILE:          return make_while( tree );
  case FOR:            return make_for( tree );
  case SWITCH:         return make_switch( tree );
  case ASSIGN:         return make_assign( tree );
  case STATEMENT_LIST: return make_cellarray( tree, 0 );
  case EXPR_STMT:      return make_expr_stmt( tree );

  case APPLY:          return make_apply( tree );
  case CELLACCESS:     return make_cellapply( tree );

  case FIELDACCESS:    return make_fieldaccess( tree );
  case DYNFIELDACCESS: return make_dynfieldaccess( tree );

  case PARAMETER_LIST: return make_stringarray( tree, 0 );
  case FUNCTION_RETURN:return make_stringarray( tree, 0 );
  case FUNCTION_PARAMETER_LIST:
                       return make_cellarray( tree, 0 );

  case CELL:           return make_cell( tree );
  case MATRIX:         return make_matrix( tree );
  case VECTOR:         return make_vector( tree );

  case ID:             return make_id( tree );
  case ID_NODE:        return make_id_node( tree );

  case INT:            return make_float( tree );  /* should this be make_int? */
  case FLOAT:          return make_float( tree );
  case STRING:         return make_string( tree );

  case AT:             return make_at_operator( tree );

  case CASE:           return make_case( tree );

  case RETURNS:        return make_return( tree );
  case CONTINUE:       return make_continue( tree );
  case BREAK:          return make_break( tree );
  case CLEAR:          return make_clear( tree );

  case PARENS:         return make_parens( tree );

  case GLOBAL:         return make_global( tree );
  case PERSISTENT:     return make_persistent( tree );

  case TRY:            return make_try( tree );
  case CATCH:          return make_catch( tree );

  /* These are empty container nodes. */
  case EXPRESSION:     return recurse( CHILD( tree, 0 ) );
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

/*
 * =============================================================================
 */

/*

this function accepts an input stream and parses it.

it then takes the resulting ANTLR parse tree and translates it into a set of matlab structures.

 */

static void my_error_printer( pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames ) {
  /* do nothing */
}

ast_t process_input( pANTLR3_INPUT_STREAM input ) {
  pmatlabLexer lex;
  pANTLR3_COMMON_TOKEN_STREAM tokens;
  pmatlabParser parser;
  ast_t retval;

  retval.error_count = 0;
  retval.ast = NULL;
  retval.line = -1;
  retval.pchar = -1;
  retval.msg = NULL;
  retval.token = NULL;

  /*
   *  Connect it up: input -> lexer -> tokenstream -> parser
   */

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
    retval.pchar = ex->charPositionInLine;
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

  retval.ast = recurse( langAST.tree );

  /*
   * Must manually clean up
   */

  parser->free( parser );
  tokens->free( tokens );
  lex->free( lex );

  return retval;
}

/*
 * =============================================================================
 */

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] ) {

  pANTLR3_INPUT_STREAM input;
  char *buf;
  ast_t retval;

  if ( (nrhs != 1 && nrhs != 2 ) || !mxIsChar( prhs[0] ) || nlhs != 2 ) {
    mexErrMsgTxt( "usage: [retval,ast] = mparser( string [, is_filename] )" );
  }

  buf = mxArrayToString( prhs[0] );
  if ( buf == NULL ) {
    mexErrMsgTxt( "error converting input argument to string!" );
  }

  if ( nrhs == 2 ) {
    input  = antlr3FileStreamNew( (pANTLR3_UINT8)buf, ANTLR3_ENC_8BIT );
    if ( input == NULL ) {
      mexErrMsgTxt( "unable to open file!\n" );
    }
  } else {
    input = antlr3StringStreamNew( (pANTLR3_UINT8)buf, ANTLR3_ENC_8BIT, strlen(buf), NULL );
    if ( input == NULL ) {
      mexErrMsgTxt( "unable to create input stream from string!\n" );
    }
  }
  
  retval = process_input( input );

  plhs[0] = mxCreateDoubleScalar( retval.error_count );

  if ( retval.error_count == 0 ) {
    plhs[1] = retval.ast;

  } else {

    fprintf( stderr, "Parse error!\n" );

    const char *err_fieldnames[4] = { "line","pchar","msg","token" };
    plhs[1] = mxCreateStructMatrix( 1, 1, 4, err_fieldnames );

    mxSetField( plhs[1], 0, "line", mxCreateDoubleScalar( retval.line ) );
    mxSetField( plhs[1], 0, "pchar", mxCreateDoubleScalar( retval.pchar ) );
    mxSetField( plhs[1], 0, "msg", mxCreateString( retval.msg ) );
    mxSetField( plhs[1], 0, "token", mxCreateString( retval.token ) );
  }

  input->close( input );

  free( buf );
}
