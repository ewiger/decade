function test_for

  for i=1:5
    a(i) = i;
  end
  
  for i=foo(b)
    a=a+1;
  end
  
  for i=1:5
  end
  
end
