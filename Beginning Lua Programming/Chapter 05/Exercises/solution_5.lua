-- Trims leading whitespace:
function TrimLeft(Str)
  return (string.gsub(Str, "^%s+", ""))
end
