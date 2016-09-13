local mime = require("mime")
local ltn12 = require("ltn12")

-- This function receives one string and one or more high level filters. On
-- success it returns the filtered string, otherwise nil followed by an error
-- message.

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

local function Test(Str, EncodingStr)
  local CodeStr, StatStr
  CodeStr = Transform(Str, mime.encode(EncodingStr)) or ""
  StatStr = Str == Transform(CodeStr, mime.decode(EncodingStr)) and
    "OK" or "Not OK"
  io.write(string.format("%-18s [%s] %s\n", EncodingStr, CodeStr, StatStr))
end

local Str = "Gabriel Garc\237a M\225rquez"
io.write(Str, "\n")
Test(Str, "base64")
Test(Str, "quoted-printable")
