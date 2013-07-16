grammar matlab;

/*

Things to fix:	

0) lots of warnings.  This is because of anonymous function handles; see details in appropriate XXX comment.
   if you comment out one line, all of the warnings go away.  I believe that even with the warnings,
   the grammar is doing the right thing.
1) "clear" statements can accept wildcards as part of IDs, but we can't (due to the way lexing happens)
2) control flow statements require a newline or comma to resolve an ambiguity (but matlab doesn't)
3) vector construction requires comma delimiters because of whitespace madness (but matlab doesn't)
4) matrix transpose doesn't work (again because of lexing, and distinguishing strings from transposes).
5) block comments don't work (and I can't figure out why)

For each case, see the appropriate XXX comment below.

*/

options {
	output=AST;
	// comment out both of the following lines for Java.
	language=C;
	ASTLabelType=pANTLR3_BASE_TREE;
}

tokens {
	// imaginary nodes for our AST
	PROGRAM;
	FUNCTION;
	FUNCTION_RETURN;
	PARAMETER_LIST;
	FUNCTION_PARAMETER_LIST;
	STATEMENT_LIST;
	EXPRESSION;
	EXPR_STMT;
	NULL_STMT;
	ASSIGN;
	APPLY;
	FIELDACCESS;
	DYNFIELDACCESS;
	CELLACCESS;
	MATRIX;
	VECTOR;
	CELL;
	CLEAR;
	LHS;
	RHS;
	ID_NODE;
	PARENS;
}

@parser::members{

// This scans backwards in the token stream looking for a hidden newline.
// The newline must occur after the last visible token and before the current token.

// C IMPLEMENTATION


    int check_for_hidden_newline( pmatlabParser ctx ) {

      pANTLR3_TOKEN_STREAM ts = ctx->pParser->tstream;

      int tok_ind = INDEX(); // this is the index of LT(1)
      pANTLR3_COMMON_TOKEN cur_tok = LT(1);

      ANTLR3_UINT32 cur_tok_chan = cur_tok->getChannel( cur_tok ); // this is the current channel.

      int rval = 0;

      while ( tok_ind > 0 ) {
	tok_ind--;
	cur_tok = ts->get( ts, tok_ind );
	if ( cur_tok->getChannel( cur_tok ) == cur_tok_chan ) {
	  // uh-oh.  we found a non-hidden token further back in the stream, but no newline in between.  fail.
	  break;
	}
	if ( cur_tok->getType( cur_tok ) == NL ) {
	  // found it!
	  rval = 1;
	  break;
	}
      }

      return rval;
    }




// JAVA IMPLEMENTATION
// Useful for debugging.  NOTE: UNCOMMENT THE RELEVANT LINE IN hidden_nl!

/*
boolean check_for_hidden_newline( TokenStream input ) {
        int tok_ind = input.index(); // this is the index of LT(1)
        Token cur_tok = input.LT(1); 
        int cur_tok_chan = cur_tok.getChannel(); // this is the current channel.

        boolean rval = false;

        while ( tok_ind > 0 ) {
        	tok_ind--;
        	cur_tok = input.get( tok_ind );
        	if ( cur_tok.getChannel() == cur_tok_chan ) {
        		// uh-oh.  we found a non-hidden token further back in the stream, but no newline in between.  fail.
        		break;
        	}
        	if ( cur_tok.getType() == NL ) {
        		// found it!
        		rval = true;
        		break;
        	}
        }

        return rval;
}

*/

} // end of parser::members

//
// ==================================================================
//
// PARSER RULES
//

// we mostly want to ignore whitespace, but every now and then
// it's significant -- as a statement delimiter, as part of
// matrix construction, etc.  This checks the hiddent channel for a newline.

hidden_nl
	// Java
	//: ( { check_for_hidden_newline( input ) }? )
	// C
	: ( { check_for_hidden_newline( ctx ) }? )
	;

nloc	: ( hidden_nl | COMMA ); // mnemonic: newline or comma
nlos	: ( hidden_nl | SEMI ); // mnemonic: newline or semicolon

//
// scripts and m-files
//

mfile 	: function_definition+ -> ^(PROGRAM function_definition+ ) ;

scriptfile: statement_list;

program	: func_or_statement_list -> ^(PROGRAM func_or_statement_list);

//
// functions and statements
//

function_definition
	: FUNCTION function_return? ID parameter_list? nloc
	  func_or_statement_list
	  END -> ^(FUNCTION ID function_return parameter_list func_or_statement_list)
	;

function_return
	: ID EQ -> ^(FUNCTION_RETURN ID)
	| LSBRACE (options {greedy=false;} : ID COMMA?)+ RSBRACE EQ -> ^(FUNCTION_RETURN ID+)
	;

// the contents of a function (or a .m file) are statements and function definitions
func_or_statement: ( function_definition | statement );
func_or_statement_list: func_or_statement* -> ^(STATEMENT_LIST func_or_statement* );

// there are times when you can have a list of statements, but not function
// definitions -- for example, inside of an "if" block.
statement_list
	:  statement* -> ^(STATEMENT_LIST statement*)
	;

parameter_list
	: LPAREN ( ID COMMA? )* RPAREN -> ^(PARAMETER_LIST ID*)
	;

// Note: there is a functional difference between terminating a statement with a
// newline vs. a semicolon, so we have to remember the appropriate token in the AST.

statement
	:( lhs EQ ) => lhs EQ rhs nlosoc -> ^(ASSIGN lhs rhs nlosoc)
	| expression nlosoc -> ^( EXPR_STMT expression nlosoc )
	| if_statement
	| for_statement
	| while_statement
	| switch_statement
	| try_statement
	| return_statement
	| break_statement
	| continue_statement
	| clear_statement
	| global_statement
	| persistent_statement
	| SEMI -> ^(NULL_STMT) // a null statement
	;

nlosoc	: ( hidden_nl | SEMI | COMMA )^;

lhs: id_plus_indexers -> ^(LHS id_plus_indexers);
rhs: expression -> ^(RHS expression);

/* 
XXX CONTROL FLOW AMBIGUITIES:

The statement
    while a (5) end;
is parsed as
    while (a(5))
      [empty]
    end;
and not
    while a
      (5)
     end;
but I can't seem to get a non-ambiguous grammar to do that.
so, I enforce a newline or a comma after the expression to
delimit it from the body of the statements.  everywhere you
see a "nloc" below, you shouldn't really need one; this
grammar therefore parses only a subset of "true" matlab.
(Of course, *I* think that reasonable code wouldn't have
such ambiguities, don't you?  :) )

Similarly:	

The statement
    while 1 -5, end;
is parsed as
    while (1)
      -5,
    end;
while the statement
    while 1 - 5, end;
is parsed as
    while (1-5)
      [empty]
    end;

NOTE : THIS FEELS A LOT LIKE THE VECTOR AMBIGUITIES BELOW.
Perhaps solving one solves them both, since both are essentially
'space-delimited expressions' ambiguities.

*/
	
if_statement
	: IF expression nloc statement_list elseif_statement* else_statement? END -> ^(IF expression statement_list elseif_statement* else_statement?)
	;

elseif_statement
	: ELSEIF expression nloc statement_list -> ^(ELSEIF expression statement_list)
	;
	
else_statement
	: ELSE statement_list -> ^(ELSE statement_list)
	;
		
for_statement
	: FOR ID EQ expression nloc statement_list END -> ^(FOR ID expression statement_list)
	;
	
while_statement
	: WHILE expression nloc statement_list END -> ^(WHILE expression statement_list)
	;
	
switch_statement
	: SWITCH expression nloc case_statement* otherwise_statement? END -> ^(SWITCH expression case_statement* otherwise_statement?)
	;
	
case_statement
	: CASE expression nloc statement_list -> ^(CASE expression statement_list)
	;

otherwise_statement
	: OTHERWISE nloc statement_list -> ^(OTHERWISE statement_list)
	;
	
try_statement
	: TRY statement_list catch_statement? END -> ^(TRY statement_list catch_statement?)
	;
	
catch_statement
        : CATCH ID? nlosoc statement_list -> ^(CATCH ID? statement_list)
	;
	
return_statement
	: RETURNS^ nlosoc
	;

break_statement
	: BREAK^ nlosoc
	;
	
continue_statement
	: CONTINUE^ nlosoc
	;

global_statement
	: GLOBAL (options {greedy=false;} : ID COMMA?)+ nlosoc -> ^(GLOBAL ID+ nlosoc)
	;

persistent_statement
	: PERSISTENT (options {greedy=false;} : ID COMMA?)+ nlosoc -> ^(PERSISTENT ID+ nlosoc)
	;

/*	
XXX CLEAR STATEMENT WILDCARDS

How can we fix wildcards in clear statements?
For example, "clear foo* bob" ought to be parsed as
  clear (foo*) (bob)
but this is hard since the '*' is lexed as its own
character, and we've discarded the fact that there is
whitespace between '*' and 'bob', but not between
'foo' and '*'.  This means we can't tell that it's supposed
to be part of the foo identifier.

Options	for solving:
1) parser-context sensitive lexing?  (I don't think this is right)
2) poke around in the hidden channel to find out where the whitespace is.
   this is probably the best solution, but seems like a pain.
   
*/


clear_statement
	: CLEAR (options {greedy=false;} : ID COMMA?)* nlosoc -> ^(CLEAR ID* nlosoc)
	;

//
// ===============================
//

// a precedence hierarchy for parsing expressions

// these are groups of operators that have equivalent precedences
g1	: ( NEQ | DOUBLE_EQ | GRTE | GRT | LSTE | LST );
g2	: ( PLUS | MINUS );
g3	: ( LEFTDIV | RIGHTDIV | TIMES | EL_LEFTDIV | EL_RIGHTDIV | EL_TIMES );
g4	: ( EXP | EL_EXP );


/*

XXX MATRIX TRANSPOSE PROBLEM.

The single quote operator is problematic because of things like this:	

aa' + foo('some string here')+bb'

Right now, the operator is placed in the correct place in the grammar,
and the grammar checks out just fine, but you get lexing errors if you
try to use it.

*/

postfix_operator
	: ( CCT | EL_CCT );
prefix_operator
	: ( PLUS | MINUS | NEG );

// the hierarchy is defined from LOWEST to HIGHEST priority.

expression : e0 -> ^(EXPRESSION e0);

e0	: e1;

e1	: e2 (LOG_OR^ e2)*;
e2	: e3 (LOG_AND^ e3)*;
e3	: e4 (BIN_OR^ e4)*;
e4	: e5 (BIN_AND^ e5)*;
e5	: e6 (g1^ e6)*;
e6	: e7 (COLON^ e7)*;
e7	: e8 (g2^ e8)*;
e8	: e9 (g3^ e9)*;
e9	: prefix_operator^ e9 | e10;
e10	: e11 (g4^ e11)*; // note: in matlab, exponentiation is left-associative
e11	: unary_expression postfix_operator^?;

unary_expression
	: base_expression -> ^(base_expression)
	| LPAREN expression RPAREN -> ^( PARENS expression )
	;

base_expression
	: id_plus_indexers
	| INT
	| FLOAT
	| STRING
	| anon_func_handle
	| cell
	| matrix
	;

/*
XXX ANONYMOUS EXPRESSION AMBIGUITIES

This generates a ton of warnings, but it does the right thing.
At least, I think it does.

We want anonymous expressions to be "greedy".

The statement
    a+@()x+y
parses as
   a+( @()x+y )
and not as
   a+ ( @()x ) +y
but I can't seem to make the right behavior explicit.  The way that
ANTLR is disabling the alternatives seems to result in the right
behavior, though.

If you comment out the second alternative here, the grammar should
check out perfectly clean.
*/

anon_func_handle
	: AT ID -> ^(AT ID)
	| AT parameter_list (options{greedy=true;}: expression ) -> ^(AT parameter_list expression)
	;

// this captures things like foo.(bar){3,4}.baz
id_plus_indexers
	:	( i1=ID -> ^(ID_NODE $i1) )
		( DOT ( i2=ID -> ^($id_plus_indexers ^(FIELDACCESS $i2))
		      | LPAREN expression RPAREN -> ^($id_plus_indexers ^(DYNFIELDACCESS expression))
		      ) 
		| LPAREN fpl1=function_parameter_list? RPAREN -> ^($id_plus_indexers ^(APPLY $fpl1))
		| LBRACE fpl2=function_parameter_list RBRACE -> ^($id_plus_indexers ^(CELLACCESS $fpl2))
		)*
	;

// also permits the use of the colon as an "expression"
function_parameter_list
	: function_parameter ( COMMA function_parameter )* -> ^(FUNCTION_PARAMETER_LIST function_parameter*)
	;
	
function_parameter : expression	| COLON	;

matrix	: LSBRACE vector? ( nlos vector )* RSBRACE -> ^(MATRIX vector*);

cell	: LBRACE vector? ( nlos vector )* RBRACE -> ^(CELL vector*);

/*

XXX

I think the rule is the following

1) if a +/- does not have any space to the right, it's interpreted
   as a unary op
2) if there's no whitespace to the left of the operator, it's
   grouped to the left.
3) if there's whitespace to the right and left, it's treated as
   a binary op, and expressions are required on the right and left.

Vectors are pretty crazy.  Because you have a list of expressions
that can be separated by nothing but whitespace, all sorts of parsing
ambiguities start happening.  Whitespace becomes syntactically
meaningful in strange ways that are not totally context free.
For example,

[ a + b ]	is parsed as 	[ (a+b) ]
[ a+b ]		is parsed as 	[ (a+b) ]
[ a +b ]	is parsed as	[ (a) (+b) ]

A longer example:	

[ 1+  2+  3 ]   parses to   6		  
[ 1 + 2+  3 ]		    6		  
[ 1  +2+  3 ]		    1     5	  
[ 1+  2 + 3 ]		    6		  
[ 1 + 2 + 3 ]		    6		  
[ 1  +2 + 3 ]		    1     5	  
[ 1+  2  +3 ]		    3     3	  
[ 1 + 2  +3 ]		    3     3	  
[ 1  +2  +3 ]		    1     2     3

So, the weird thing is that we can't just look for the absence of a 
space between '+' and the next character to determine if it's a binary
or unary operator -- we also have to know if there's a space to the
left of it.

This is mostly a problem for prefix operators which can be ambiguous
in this context, but it also shows up in things like cell arrays
of anonymous function expressions.

size( { @()a+b } )	is	[ 1 1 ]
size( { @()a +b } )	is	[ 1 2 ]
size( { @()a + b } )	is	[ 1 1 ]

NOTE	:	 this feels like the control flow ambiguities above,
since both are essentially "whitespace-delimited expression" problems.

*/

// XXX The COMMA should really have a ? after it!!!
// vector	: ( expression COMMA? )+ -> ^(VECTOR expression+);
vector	:	expression ( COMMA expression )* -> ^(VECTOR expression+);

//
// ==================================================================
//
// LEXER RULES
//

//
// language keywords
//

BREAK	: 'break';
CASE	: 'case';
CATCH	: 'catch';
CONTINUE: 'continue';
ELSE	: 'else';
ELSEIF	: 'elseif';
END	: 'end';
FOR	: 'for';
FUNCTION: 'function';
GLOBAL	: 'global';
IF	: 'if';
OTHERWISE: 'otherwise';
PERSISTENT: 'persistent';
RETURNS	: 'return'; // not "RETURN" to avoid #define conflicts with readline.h
SWITCH	: 'switch';
TRY	: 'try';
VARARGIN: 'varargin';
WHILE	: 'while';
CLEAR	: 'clear';

ENDS	: END SEMI? ;

//
// operators and assignments
//

DOUBLE_EQ	: '==';

LOG_OR	: '||';
LOG_AND	: '&&';
LSTE	: '<=';
GRTE	: '>=';
NEQ	: '~=';

EL_TIMES	: '.*';
EL_LEFTDIV	: './';
EL_RIGHTDIV	: '.\\';
EL_EXP	: '.^';
EL_CCT	: '.\'';

EQ	: '=';

BIN_OR	: '|';
BIN_AND	: '&';

LST	: '<';
GRT	: '>';

COLON	: ':';

PLUS	: '+';
MINUS	: '-';
NEG	: '~';
TIMES	: '*';

LEFTDIV	: '/';
RIGHTDIV: '\\';

EXP	: '^';

CCT	: '\'';

//
// Other useful language snippets
//

SEMI	: ';';
LPAREN	: '(';
RPAREN	: ')';
LBRACE	: '{';
RBRACE	: '}';
LSBRACE	: '[';
RSBRACE	: ']';
AT	: '@';
DOT	: '.';
COMMA	: ',';

//
// comments
//

NL	: '\r'? '\n' { $channel = HIDDEN; }; // newline

// XXX I can't seem to get block comments to work.  The problem is that
// no matter what I do, the linecomment ends up "overriding" the block
// comment, and I get a lex error.  I've tried syntactic predicates,
// but they didn't help...
// If I comment out the LINECOMMENT rule, the BLOCKCOMMENT works fine.
// So, since I can only seem to have one or the other, I'm commenting
// out BLOCKCOMMENT for now.

//BLOCKCOMMENT
//        : '%{' (options{greedy=false;} : .)*  '%}' { $channel = HIDDEN; }
//	;

LINECOMMENT
	: '%' (options {greedy=false;} : .)* NL { $channel = HIDDEN; }
	;

// I think this is how to use syntactic predicates, but it doesn't seem to work.
//COMMENT
//	: ( '%{' ) => '%{' (options{greedy=false;}: .)*  '%}' { $channel = HIDDEN; }
//	| ( '%' (options{greedy=false;}: .)* NL ) { $channel = HIDDEN; }
//	;


THREEDOTS
	: ( '...' NL ) { $channel=HIDDEN;}
	;

//
// identifiers, strings, numbers, whitespace
//

ID	: ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')* ;

INT	: '0'..'9'+ ;

FLOAT
	: ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
	| '.' ('0'..'9')+ EXPONENT?
	| ('0'..'9')+ EXPONENT
	;

STRING
	: '\'' ( ESC_SEQ | ~('\\'|'\'') )* '\''
	;

WS
	: ( ' '
        | '\t'
        ) {$channel=HIDDEN;}
	;

fragment
EXPONENT
	: ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT
	: ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
	: '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
	| UNICODE_ESC
	| OCTAL_ESC
	;

fragment
OCTAL_ESC
	: '\\' ('0'..'3') ('0'..'7') ('0'..'7')
	| '\\' ('0'..'7') ('0'..'7')
	| '\\' ('0'..'7')
	;

fragment
UNICODE_ESC
	: '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
	;
