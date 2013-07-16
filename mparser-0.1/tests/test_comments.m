function test_comments
  b = 42;  % a comment
  c = 43;  % another
  % this is a comment
  a = b + ...
      c;
  
  % this is illegal.
  % a = b + ... c;
  
  % XXX block comments are not yet supported!

%{
%
%This is a multiline comment.
%
%Hello, world.
%
%}
  
end