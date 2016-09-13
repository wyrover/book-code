module(..., package.seeall)

function Quote(Str)
  return string.format("%q", Str)
end
