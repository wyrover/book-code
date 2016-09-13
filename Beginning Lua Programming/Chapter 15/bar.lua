#!/usr/local/bin/lua

require "gd"
require "iomode"

io.modeset(io.stdin, "binary")
io.modeset(io.stdout, "binary")

local function LclPng(Str)

  local ValMax = 1 -- Prevent subsequent division by zero
  local ValList = {}
  local ValCount = 0
  local ClrList = {}
  local Factor, Ht, Wd, BarWd, Gap, Hnd, PngStr, ErrStr

  Str = "_" .. string.gsub(string.lower(Str), "_", "__") .. "_"

  local function LclPrm(Prefix, Default)
    local PatStr = "_" .. Prefix .. "(%d+)_"
    local Pos, Pos, NumStr = string.find(Str, PatStr)
    return Pos and tonumber(NumStr) or Default
  end

  Ht = LclPrm("h", 200)
  BarWd = LclPrm("b", 12)
  Gap = LclPrm("g", 4)
  for ValStr in string.gmatch(Str, "_([%.%d%:]+)_") do
    local Pos, NumStr, ClrStr, Val
    Pos, Pos, NumStr, ClrStr = string.find(ValStr, "([%d%.]+)%:(%d+)")
    Val = tonumber(Pos and NumStr or ValStr)
    if Val > ValMax then
      ValMax = Val
    end
    table.insert(ValList, { Val, tonumber(ClrStr) })
    ValCount = ValCount + 1
  end
  if ValCount > 0 then
    Wd = ValCount * (Gap + BarWd) + Gap + 1
    Factor = (Ht - Gap) / ValMax
    Hnd = gd.createPalette(Wd, Ht)
    if Hnd then
      -- Background is first color allocated
      ClrList.White = Hnd:colorAllocate(255, 255, 255)
      ClrList.Gray = Hnd:colorAllocate(150, 150, 150)
      ClrList.Black = Hnd:colorAllocate(0, 0, 0)
      -- Allocate colors specified in query string
      for Mode, RdStr, GrStr, BlStr in string.gmatch(Str,
        "_([cfl])(%x%x)(%x%x)(%x%x)_") do
        local Clr = Hnd:colorAllocate(tonumber(RdStr, 16),
          tonumber(GrStr, 16), tonumber(BlStr, 16))
        if Mode == 'c' then
          table.insert(ClrList, Clr)
        elseif Mode == 'l' then
          ClrList.Line = Clr
        else
          ClrList.Fill = Clr
        end
      end
      local ClrDef = ClrList[1] or ClrList.Gray
      ClrList.Line = ClrList.Line or ClrList.Gray
      -- By default, background color is the first color allocated. If a fill
      -- color was specified, fill in the image now.
      if ClrList.Fill then
        Hnd:filledRectangle(1, 1, Wd, Ht, ClrList.Fill)
      end
      local X = Gap
      for J = 1, ValCount do
        local Y = Factor * ValList[J][1]
        local ClrPos = ValList[J][2] or 0
        local Clr = ClrList[ClrPos] or ClrDef
        Hnd:filledRectangle(X, Ht - Y, X + BarWd,
          Ht, ClrList.Line)
        Hnd:filledRectangle(X + 1, Ht + 1 - Y, X + BarWd - 1,
          Ht, Clr)
        X = X + Gap + BarWd
      end
      -- Create border around image
      Hnd:line(0, 0, 0, Ht, ClrList.Line)
      Hnd:line(0, 0, Wd, 0, ClrList.Line)
      Hnd:line(Wd - 1, 0, Wd - 1, Ht - 1, ClrList.Line)
      Hnd:line(0, Ht - 1, Wd - 1, Ht - 1, ClrList.Line)
      -- Load image into Lua string
      PngStr = gd.pngStr(Hnd)
      if not PngStr then
        ErrStr = "Error generating PNG image."
      end
      Hnd = nil
    else
      ErrStr = "Error creating palette-based PNG image."
    end
  else
    ErrStr = "No values retrieved from query string."
  end
  return PngStr, ErrStr
end

local Http, PngStr, ErrStr, QueryStr

Http = os.getenv("SERVER_SOFTWARE")
QueryStr = arg[1] or os.getenv("QUERY_STRING")
if QueryStr then
  PngStr, ErrStr = LclPng(QueryStr)
else
  ErrStr = "Error retrieving query string."
end
if PngStr then
  if Http then
    io.write('Date: ', os.date(), '\r\n')
    io.write('X-Scripting-Engine: ', _VERSION, '\r\n')
    io.write('X-Graphics-Library: ', gd.VERSION, '\r\n')
    io.write("Content-Transfer-Encoding: binary\r\n")
    io.write('Content-Length: ', string.len(PngStr), '\r\n')
    io.write("Content-Type: image/png\r\n\r\n");
  end
  io.write(PngStr)
else
  io.write(Http and 'Content-Type: text/plain\r\n\r\n' or '', ErrStr, "\r\n")
end
