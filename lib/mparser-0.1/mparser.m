%
% function [retval,ast] = mparser( string [, is_filename] )
%
% A mex file which interfaces to an ANTLR matlab grammar.  The
% function accepts a string and returns an AST represented as a
% deeply nested set of structures.
%
% If the "is_filename" flag is set (to anything) the string is
% interpreted as a filename and the contents of that file are
% parsed.
%
% Returns:
%
% The "retval" return value is the number of parse errors encountered
% in the string or file.  If it is zero, the "ast" contains the
% parse tree.  If "retval" is non-zero, "ast" is actually a
% structure containing information about the parse error.
%
%
% Copyright(c) 2011 David Wingate
% 
% This file is part of the mparser package, which is licensed under
% the MIT license.  See the file COPYING for details.
%
