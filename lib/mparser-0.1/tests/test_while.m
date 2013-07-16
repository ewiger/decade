function test_while
  while ( a == b )
    s1;
    s2;
  end

  while a == b
    s3;
    s4;
  end
  
  while 1, a=b; end
end
