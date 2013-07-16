function test_expr
  (a && c ) || (~a | b );
  a+5;
  a(42).foo.(bob)
  a('42').foo.('bob')
  'hello' + 'world'
  a{'bob'}{42}

  % XXX transpose operator doesn't work yet due to a lexing problem in
  % the ANTLR grammar.  See comments in matlab.g
  % a'
  
  % element-wise transpose
  B = A.';
  B = A.' + 'foo';
  B = 'foo' + A.' + 'bar';

  A( ~isfinite(A) ) = 42;
  ~( ~A || ~B )
  
  C = +5;
  C = -5;
  C = A^B^C;
  C = A^(B+C);
  C = A^B+C;
  C = +++++5;
  C = -----5;
  
  A | B;
  A || B | C .^ D;
  
  foom
  foom( fpl )
  foom{ fpl }
  foom.id
  foom.( expr )
  foom{ fpl }.(expr).(expr)
  
  foom{ fpl }.(expr){ fpl }
  
  foom{ 42 }( 4 )

end
