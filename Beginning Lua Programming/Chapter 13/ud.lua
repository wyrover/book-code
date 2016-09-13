package.cpath = "./?.so;./?.dll"
require "ud_example"

local HndA = ud_example.open(1)
local HndB = ud_example.open(2)
do -- local block
  local HndC = ud_example.open(3)
  io.write(tostring(HndA), ", ", tostring(HndB), ", ", tostring(HndC), "\n")
  HndA:set(4)
  HndA:set(1)
  HndA:close()
  io.write("End of local block\n")
end
collectgarbage("collect")
io.write("End of script\n")
