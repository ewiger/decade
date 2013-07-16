function [ A, B ] = big_test( a, b, c )

% basic expressions
a+b
a+b+c
a-b-c
a | b || c && z + 42 .^ 3
-3^3
a^b^c
a+b*c
a*b+c
(a+b)*c.^foo

% function handles
@foo
@(x,y)x+y
a+@(x,y)x+y

% assignments
a = 42
a(42) = 15
a(15,1:20) = [ 1, 2, 3 ]

% indexing
a
a(42)
a(42,:)
a(42,1:10)
foo.bar(baz){42}.quux
foo.bar.baz.quux

% cells / matrices
[ 1, 2, 3 ]
{ @randn, @rand }
{ {1,2,3}, {4,5,6} }
% 
% XXX these don't work yet.
%[ 1+  2+  3 ]
%[ 1 + 2+  3 ]
%[ 1  +2+  3 ]
%[ 1+  2 + 3 ]
%[ 1 + 2 + 3 ]
%[ 1  +2 + 3 ]
%[ 1+  2  +3 ]
%[ 1 + 2  +3 ]
%[ 1  +2  +3 ]

% clear / return / break / continue
clear
clear foo
clear foo bar
% XXX these don't work yet
%clear foo*
%clear foo* bar*
%clear foo * bar
return
break
continue

% control flow

if ( flip )
  a = 42;
else
  bob = fred;
end

while ( 42 )
  try
    a(42) = 12;
  catch err
    disp( err )
  end
end

switch ( foo )
  case { 'bob', 'jim' }
    a = 42;
  case 42
    b = 42
  otherwise
    fprintf( stderr, 'crash\n' );
end

% XXX this doesn't work yet
%while a (5) end

% strings and matrix transpose
a.'
% XXX doesn't work yet
%a'
%aa' + foo('a+b')+bb'
a = 'foo'

% indexing

foo('bar')
foo.('bar')
foo = { 'bar', 'baz' }
% XXX doesn't work yet
%foo = { 'bar' 'baz' }

end
