-- Reads lines, evaluates them as expressions, prints the
-- results:
function ExprInterp()
  io.write("expression> ")
  local Line = io.read()
  while Line and Line ~= "quit" do
    -- Convert the expression to a statement, convert the
    -- statement to a function, call the function, and print
    -- its results:
    Line = "return " .. Line
    local Fnc = loadstring(Line) -- Missing error check.
    print(Fnc()) -- Missing error check.
    io.write("expression> ")
    Line = io.read()
  end
end
