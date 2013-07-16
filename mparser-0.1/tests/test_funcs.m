function R = test_funcs

  a = @randn;
  
  function z = foo( x, y )
    z = a + x + y;
  end

  R = @foo;
  
  function q = bar( a, z )
    hohum1;
    hohum2;
  end

  R = @(x,y)randn(x,y);
  R = @(x)foo;
  R = @bob;
  A{1} = @rand;
  
end

function [a,b] = q( a, b )
  % a comment
  a = 42;
  b = a+1;
end

function [a] = q2()
  q = r;
  function hohum
  end
end
