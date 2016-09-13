local Arg = string.lower(arg[1] or "")
local Read = string.match(Arg, "r")
local Mode = string.match(Arg, "b") and "binary" or "text"

require "iomode"

io.modeset(io.stdout, Mode)
io.modeset(io.stdin, Mode)

if Read then
  local Str = io.read("*all")
  for J = 1, #Str do
    local Val = string.byte(Str, J, J)
    if Val >= 32 then
      io.write("'", string.sub(Str, J, J), "' ")
    else
      io.write(string.format("0x%02x ", Val))
    end
  end
  io.write("\n")
else -- Write
  io.write("1\0132\0103\0264")
end
