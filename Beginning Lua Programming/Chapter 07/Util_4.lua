local Util = {}

function Util.Quote(Str)
  return string.format("%q", Str)
end

return Util
