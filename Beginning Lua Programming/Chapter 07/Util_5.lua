local format = string.format

module(...)

function Quote(Str)
  return format("%q", Str)
end
