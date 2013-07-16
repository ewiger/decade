function test_cell

  a = { 1, 2, 3 };
  a = { a; b; c };
  a = { a+b, c+d; foo, 'bar' };

end
