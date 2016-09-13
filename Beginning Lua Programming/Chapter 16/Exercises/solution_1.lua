local ltn12 = require("ltn12")

local function Transform(Str, ...)
  -- Source is specified string
  local Src = ltn12.source.string(Str)
  -- Chain all specified filters into one
  local Filter = ltn12.filter.chain(...)
  -- Send all data chunks to table
  local Snk, Tbl = ltn12.sink.table()
  -- Filter chunks before delivering to sink
  Snk = ltn12.sink.chain(Filter, Snk)
  -- Open the valve
  local Code, ErrStr = ltn12.pump.all(Src, Snk)
  return Code and table.concat(Tbl) or nil, ErrStr
end

-- Return the rot13 cipher of the character Ch

local function Rot13Char(Ch)
  local Val = string.byte(Ch)
  return string.char(Val < 65 and Val or
    Val < 78 and Val + 13 or
    Val < 91 and Val - 13 or
    Val < 97 and Val or
    Val < 110 and Val + 13 or
    Val < 123 and Val - 13 or Val)
end

-- Filter that returns a rot13 cipher of the string Str

local function Rot13(Str)
  return Str and string.gsub(Str, "(%a)", Rot13Char)
end

local Str = "The quick brown fox jumps over the lazy dog"
local CnvStr = Transform(Str, Rot13)
io.write(Str, "\n", CnvStr, "\n",
  Str == Transform(CnvStr, Rot13) and "OK" or "Not OK", "\n",
  Str == Transform(Str, Rot13, Rot13) and "OK" or "Not OK", "\n")
