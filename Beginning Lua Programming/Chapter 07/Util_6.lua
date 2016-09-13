local _G = _G

module(...)

function Quote(Str)
  return _G.string.format("%q", Str)
end
