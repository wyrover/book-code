-- "String print" -- returns a string of the form output by
-- print:
function Sprint(...)
  local Args = {...}
  -- Using select and a numeric for makes sure that no nils
  -- are missed:
  local ArgCount = select("#", ...)
  for I = 1, ArgCount do
    Args[I] = tostring(Args[I])
  end
  return table.concat(Args, "\t") .. "\n"
end
