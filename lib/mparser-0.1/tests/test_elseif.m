function test_elseif
  if bob == 42
    a = 5;
  end
  
  if ( bob == 42 )
    fprintf( 1, 'hello, %s\n', bob );
  elseif ( bob == 43 )
    fprintf( 'goodbye, world\n' );
  elseif bob == 44
    fprintf( 'goodbye, world\n' );
  else
    b = @frank;
  end
end
