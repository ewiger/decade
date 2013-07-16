function test_cntrl

  for i=1:10
    if ( foo )
      break;
    else
      return;
    end
    continue;
  end
  
  if ( frank )
    continue;
  end

end