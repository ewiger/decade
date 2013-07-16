% function matlab_ast_print( ast )
%
% Accepts an AST as returned by mparser and walks it, printing out
% the contents of each node in such a way that the original source
% code is regenerated.
%
%
% Copyright(c) 2011 David Wingate
% 
% This file is part of the mparser package, which is licensed under
% the MIT license.  See the file COPYING for details.
%


%
% ==========================================================================
%

% We pass around an "f" function, which is used to emit the
% appropriate strings.  This could be replaced with any other emitter,
% for example something that wrote the contents out to a file, instead
% of to stdout.

function matlab_ast_print( ast )
  f = @printer;
  indent = 0;
  recurse_on_node( ast, f, indent );
end

function printer( x, literal )
  if ( nargin > 1 && literal )
    % we don't want to interpret this string - just print it verbatim
    fprintf( 1, '%s', x );
  else
    % normal fprintf
    fprintf( 1, x );
  end;
end


%
% ==========================================================================
%

% This is the main function.

function recurse_on_node( n, f, indent )

  % put some base case logic here.
  % this simplifies some of the code later.
  if ( isempty( n ) )
    return;
  end
  
  if ( isstr( n ) )
    f( n );
    return;
  end
  
  if ( iscell( n ) )
    for i=1:length( n )
      recurse_on_node( n{i}, f, indent );
    end
    return;
  end

  %
  %  The main recursion
  %
  
  switch( n.type )

   case 'program'
    print_program( n, f, indent );

   case 'function_definition'
    print_function_definition( n, f, indent );

   case 'while_statement'
    print_while_statement( n, f, indent );
    
   case 'for_statement'
    print_for_statement( n, f, indent );
    
   case 'if_statement'
    print_if_statement( n, f, indent );
    
   case 'elseif'
    print_elseif( n, f, indent );
   
   case 'switch_statement'
    print_switch_statement( n, f, indent );

   case 'try_statement'
    print_try_statement( n, f, indent );

   case 'catch_statement'
    print_catch_statement( n, f, indent );
    
   case 'expression_stmt'
    print_expression_stmt( n, f, indent );
    
   case 'assignment_stmt'
    print_assignment_stmt( n, f, indent );
    
   case 'case'
    print_case( n, f, indent );

   case 'binary_op'
    print_binary_op( n, f, indent );
   
   case 'unary_op'
    print_unary_op( n, f, indent );
   
   case 'constant'
    print_constant( n, f, indent );

   case 'id'
    print_id( n, f, indent );
    
   case 'function_handle'
    print_function_handle( n, f, indent );
    
   case 'cell'
    print_cell( n, f, indent );
    
   case 'matrix'
    print_matrix( n, f, indent );
    
   case 'vector'
    print_vector( n, f, indent );
    
   case 'anon_function'
    print_anon_function( n, f, indent );
    
   case 'fieldaccess'
    print_fieldaccess( n, f, indent );
    
   case 'dynfieldaccess'
    print_dynfieldaccess( n, f, indent );
    
   case 'apply'
    print_apply( n, f, indent );
    
   case 'cellapply'
    print_cellapply( n, f, indent );

   case 'continue'
    print_continue( n, f, indent );

   case 'break'
    print_break( n, f, indent );
   
   case 'return'
    print_return( n, f, indent );

   case 'parens'
    print_parens( n, f, indent );
   
   case 'global'
    print_global( n, f, indent );

   case 'persistent'
    print_persistent( n, f, indent );

   case 'clear'
    print_clear( n, f, indent );

  end

end

%
% ==========================================================================
%

% Some utility functions

% prints out 2*n spaces to give pretty indenting.
function print_indent( f, indent )
  f( repmat( ' ', 1, indent*2 ) );
end

% prints a separated list of items.
function print_sep_list( n, f, indent, sep )
  if ( length(n) == 0 )
    return;
  end
  for i=1:length( n )-1
    recurse_on_node( n{i}, f, indent );
    f( sep );
  end
  recurse_on_node( n{end}, f, indent );
end

%
% ==========================================================================
%

%
% Functions for each node type.
%

function print_program( n, f, indent )
  recurse_on_node( n.statements, f, indent );
end

%
% ==========================================================================
%

function print_function_definition( n, f, indent )
  print_indent( f, indent );
  f( 'function ' );

  num_ret_args = length(n.return_params);
  if (  num_ret_args > 0 )
    f( '[ ' );
    for i=1:num_ret_args-1
      f( n.return_params{i} );
      f( ', ' );
    end
    f( n.return_params{end} );
    f( ' ] = ' );
  end

  f( n.name );
  
  num_input_args = length( n.input_params );
  if ( num_input_args > 0 )
    f( '( ' );
    for i=1:num_input_args-1
      f( n.input_params{i} );
      f( ', ' );
    end
    f( n.input_params{end} );
    f( ' )' );
  end
  print_indent( f, indent );
  f( '\n' );
  
  recurse_on_node( n.statements, f, indent + 1 );

  print_indent( f, indent );
  f( 'end\n' );

end

%
% ==========================================================================
%

function print_while_statement( n, f, indent )
  print_indent( f, indent );
  f( 'while ' );

  recurse_on_node( n.condition, f, indent );  
  
  f( '\n' );

  recurse_on_node( n.statements, f, indent + 1 );

  print_indent( f, indent );
  f( 'end\n' );  
end

%
% ==========================================================================
%

function print_for_statement( n, f, indent )
  print_indent( f, indent );
  f( 'for ' );

  f( n.iterator );
  f( ' = ' );
  
  recurse_on_node( n.values, f, indent );  
  
  f( '\n' );

  recurse_on_node( n.statements, f, indent + 1 );

  print_indent( f, indent );
  f( 'end\n' );  
end

%
% ==========================================================================
%

function print_if_statement( n, f, indent )
  print_indent( f, indent );
  f( 'if ' );

  recurse_on_node( n.condition, f, indent );
  
  f( '\n' );

  recurse_on_node( n.statements, f, indent + 1 );

  recurse_on_node( n.elseifs, f, indent ); % note - same indent
  
  if ( ~isempty( n.else ) )
    print_indent( f, indent );
    f( 'else\n' );
    recurse_on_node( n.else, f, indent + 1 );
  end

  print_indent( f, indent );
  f( 'end\n' );  
end

%
% ==========================================================================
%

function print_switch_statement( n, f, indent )
  print_indent( f, indent );
  f( 'switch ' );

  recurse_on_node( n.expression, f, indent );  
  
  f( '\n' );

  recurse_on_node( n.cases, f, indent ); % note - same indent
  
  if ( ~isempty( n.otherwise ) )
    print_indent( f, indent );
    f( 'otherwise\n' );
    recurse_on_node( n.otherwise, f, indent + 1 );
  end

  print_indent( f, indent );
  f( 'end\n' );  
end

%
% ==========================================================================
%

function print_try_statement( n, f, indent )
  print_indent( f, indent );
  f( 'try\n' );
  
  recurse_on_node( n.statements, f, indent );  
  
  if ( ~isempty( n.catch ) )
    recurse_on_node( n.catch, f, indent ); % note - same indent
  end

  print_indent( f, indent );
  f( 'end\n' );  
end

function print_catch_statement( n, f, indent )
  print_indent( f, indent );
  f( 'catch ' );
  if ( ~isempty( n.id ) )
    f( n.id )
  end
  f( '\n' );

  recurse_on_node( n.statements, f, indent + 1 );

  % parent will print the "end" for us
end

%
% ==========================================================================
%

function print_expression_stmt( n, f, indent )
  print_indent( f, indent );

  recurse_on_node( n.expression, f, indent );

  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );

end

%
% ==========================================================================
%

function print_assignment_stmt( n, f, indent )
  print_indent( f, indent );

  recurse_on_node( n.lhs, f, indent );
  f( ' = ' );
  recurse_on_node( n.rhs, f, indent );

  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );

end

%
% ==========================================================================
%

function print_elseif( n, f, indent )
  print_indent( f, indent );
  f( 'elseif ' );
  recurse_on_node( n.expression, f, indent );
  f( '\n' );

  recurse_on_node( n.statements, f, indent + 1 );
  % the parent will handle printing out an "end" for us.
end

%
% ==========================================================================
%

function print_case( n, f, indent )
  print_indent( f, indent );
  f( 'case ' );
  recurse_on_node( n.expression, f, indent );
  f( '\n' );
  recurse_on_node( n.statements, f, indent + 1 );
  % the parent switch will handle printing out an "end" for us.
end

%
% ==========================================================================
%

function print_binary_op( n, f, indent )
  recurse_on_node( n.lhs, f, indent );
  f( ' ' );
  f( n.op );
  f( ' ' );
  recurse_on_node( n.rhs, f, indent );
end
  
%
% ==========================================================================
%

function print_unary_op( n, f, indent )
  switch( n.op )
   
   case { '+', '-', '~' }
    f( n.op );
    recurse_on_node( n.lhs, f, indent );
   
   % this is .' -- element-wise transpose
   % this is ' -- transpose   
   case { '.''', '''' }
    recurse_on_node( n.lhs, f, indent );   
    f( n.op );    

   otherwise
    error( 'unknown unary op!' );
    
  end
  
end

%
% ==========================================================================
%

function print_parens( n, f, indent )
  f( '( ' );
  recurse_on_node( n.expression, f, indent );
  f( ' )' );
end

%
% ==========================================================================
%

function print_constant( n, f, indent )
  % possible types of constants include "string" and "float".
  % but we don't care, because strings are represented with their
  % quote marks and all escaping, so we just spit it out...
  f( n.value, 1 );
end

%
% ==========================================================================
%

function print_id( n, f, indent )
  f( n.id );  % should always be a string.  just spit it out.
  for i=1:length( n.indexers )
    recurse_on_node( n.indexers{i}, f, indent );
  end
end

%
% ==========================================================================
%

function print_function_handle( n, f, indent )
  f( '@' );
  f( n.function_id );
end

%
% ==========================================================================
%

function print_cell( n, f, indent )
  f( '{' );
  print_sep_list( n.vectors, f, indent, '; ' );
  f( '}' );
end

%
% ==========================================================================
%

function print_matrix( n, f, indent )
  f( '[' );
  print_sep_list( n.vectors, f, indent, '; ' );
  f( ']' );
end

%
% ==========================================================================
%

function print_vector( n, f, indent )
  print_sep_list( n.expressions, f, indent, ', ' );
end

%
% ==========================================================================
%

function print_anon_function( n, f, indent )
  f( '@( ' );
  print_sep_list( n.parameters, f, indent, ', ' );
  f( ' )' );
  recurse_on_node( n.expression, f, indent );
  f( ';' );
end

%
% ==========================================================================
%

function print_fieldaccess( n, f, indent )
  f( '.' )
  f( n.id );
end

%
% ==========================================================================
%

function print_dynfieldaccess( n, f, indent )
  f( '.( ' )
  recurse_on_node( n.expression, f, indent );
  f( ' )' );
end

%
% ==========================================================================
%

function print_apply( n, f, indent )
  f( '( ' );
  print_sep_list( n.parameter_list, f, indent, ', ' );
  f( ' )' );
end

%
% ==========================================================================
%

function print_cellapply( n, f, indent )
  f( '{ ' );
  recurse_on_node( n.parameter_list, f, indent );
  f( ' }' );
end

%
% ==========================================================================
%

function print_continue( n, f, indent )
  print_indent( f, indent );
  f( 'continue' );
  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );
end

function print_break( n, f, indent )
  print_indent( f, indent );
  f( 'break' );
  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );
end

function print_return( n, f, indent )
  print_indent( f, indent );
  f( 'return' );
  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );
end

function print_global( n, f, indent )
  print_indent( f, indent );
  f( 'global ' );
  print_sep_list( n.ids, f, indent, ', ' );
  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );
end

function print_persistent( n, f, indent )
  print_indent( f, indent );
  f( 'persistent ' );
  print_sep_list( n.ids, f, indent, ', ' );
  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );
end

function print_clear( n, f, indent )
  print_indent( f, indent );
  f( 'clear ' );
  print_sep_list( n.ids, f, indent, ', ' );
  if ( n.semicolon )
    f( ';' );
  end
  f( '\n' );
end
