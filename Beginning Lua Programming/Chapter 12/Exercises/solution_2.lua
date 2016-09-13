require "pack"

local PngFileStr = arg[1] or "logo.png"

local function LclFileGet(FileStr)
  local Str
  local Hnd, ErrStr = io.open(FileStr, "rb")
  if Hnd then
    Str = Hnd:read("*all")
    Hnd:close()
  end
  return Str, ErrStr
end

local function printf(FrmtStr, ...)
  io.write(string.format(FrmtStr, ...))
end

local PngStr, ErrStr = LclFileGet(PngFileStr)
if PngStr then
  local PngSize = string.len(PngStr)
  local DataLen, DataStr, HdrStr, Crc
  -- Documented PNG signature
  local SignatureStr = string.char(137, 80, 78, 71, 13, 10, 26, 10)
  local Pos = string.len(SignatureStr)
  if SignatureStr == string.sub(PngStr, 1, Pos) then
    Pos = Pos + 1
    printf("Header Length  CRC\n")
    printf("------ ------  ----------\n")
    while Pos and Pos < PngSize do
      Pos, DataLen, HdrStr = string.unpack(PngStr, ">LA4", Pos)
      if Pos then
        -- print("unpack_1", Pos, DataLen, HdrStr)
        if DataLen > 0 then
          Pos, DataStr, Crc = string.unpack(PngStr, ">A" .. DataLen .. "L", Pos)
        else
          Pos, Crc = string.unpack(PngStr, ">L", Pos)
          DataStr = ""
        end
        -- print("unpack_2", Frmt, Pos, string.len(DataStr), Crc)
        if Pos then
          printf("%s %8u  0x%08X\n", HdrStr, DataLen, Crc)
          if HdrStr == "IEND" then
            Pos = nil -- End of loop
          end
        end
      end
    end
  else
    io.write("Error: ", PngFileStr, " does not have expected signature\n")
  end
else
  io.write("Error: ", ErrStr, "\n")
end
