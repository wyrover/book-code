local ClrStr = "#C6EFF7"
local Red, Green, Blue = string.match(ClrStr, "%#(%x%x)(%x%x)(%x%x)")
if Red then
  io.write("Red: ", tonumber(Red, 16), ", Green: ", tonumber(Green, 16),
    ", Blue: ", tonumber(Blue, 16), "\n")
end
