-- Returns a function that appends N dots to its argument:
function MakeDotter(N)
  local Dots = ""
  for I = 1, N do
    Dots = Dots .. "."
  end
  return function(Str)
    return Str .. Dots
  end
end
