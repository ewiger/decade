function test_switch

  switch ( bob )
   case 42
    a = 17;
    b = 19;
   case { 'jim', 'frank' }
    b = 12;
    a = 'foo';
   otherwise
    g = @randn;
    h = @rand;
  end

end
