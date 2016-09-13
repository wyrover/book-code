-- Turns $XYZ into the value of the global variable XYZ (XYZ
-- is any identifier):
function Interpolate(Str)
  return (string.gsub(Str, "%$([%a_][%w_]*)",
    function(Ident)
      return tostring(getfenv()[Ident])
    end))
end
