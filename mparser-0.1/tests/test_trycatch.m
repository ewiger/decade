function test_trycatch

  A = rand(3);
  B = ones(5);

  try
    foo;
    bar;
  end
  
  try
    foo;
    bar;
  catch
  end
  
  try
    A;
    B;
  catch
    B;
    C;
  end

  try
    C = [A; B];
  catch err
  
      % Give more information for mismatch.
    if (strcmp(err.identifier,'MATLAB:catenate:dimensionMismatch'))

      msg = sprintf('%s', ...
                    'Dimension mismatch occured: First argument has ', ...
                    num2str(size(A,2)), ' columns while second has ', ...
                    num2str(size(B,2)), ' columns.');
      error('MATLAB:myCode:dimensions', msg);

      % Display any other errors as usual.
    else
      rethrow(err);
    end
  end  % end try/catch
end
