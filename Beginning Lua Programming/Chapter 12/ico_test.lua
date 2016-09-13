require "pack"
require "show"

local function LclFileGet(FileStr)
  local Str
  local Hnd, ErrStr = io.open(FileStr, "rb")
  if Hnd then
    Str = Hnd:read("*all")
    Hnd:close()
  end
  return Str, ErrStr
end

local FileStr = arg[1] or "../lua-5.1.1/etc/lua.ico"
local IcoStr, ErrStr = LclFileGet(FileStr)
if IcoStr then
  local Ico = {}
  local Pos
  Pos, Ico.Reserved, Ico.Type, Ico.Count = string.unpack(IcoStr, "<H3")
  Ico.List = {}
  for J = 1, Ico.Count do
    local Item = {}
    Pos, Item.Width, Item.Height, Item.ColorCount, Item.Reserved,
      Item.Planes, Item.BitCount, Item.Size, Item.Offset =
      string.unpack(IcoStr, "<b4H2L2", Pos)
    table.insert(Ico.List, Item)
  end
  Pos, Ico.HdrSize, Ico.Width, Ico.Height, Ico.Planes, Ico.Bitcount,
    Ico.Compression, Ico.ImageSize, Ico.XPixelsPerM, Ico.YPixelsPerM,
    Ico.ColorsUsed, Ico.ColorsImportant = string.unpack(IcoStr, "<L3H2L6", Pos)
  ObjectShow(Ico, "Ico")
else
  io.write(ErrStr, "\n")
end
