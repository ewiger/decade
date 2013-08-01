function test_gp(x)
  global foo bar;
  global foo, bar
  persistent foo, bar;
  persistent foo bar
  clear foo bar;
  clear foo, bar, baz
end
