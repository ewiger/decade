% function pnp( x )
%
% "Parse-and-print"
%
% A simple script that wraps the mparser MEX file.  It parses a
% file, checks for parse errors, and then prints out the parsed
% file to the screen.
%
%
% Copyright(c) 2011 David Wingate
% 
% This file is part of the mparser package, which is licensed under
% the MIT license.  See the file COPYING for details.
%

function pnp( x )
  [r,a] = mparser( x, 1 );
  if ( r == 0 )
    matlab_ast_print( a );
  else
    % parse error
    a
  end;
  
end
