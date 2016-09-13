local ProgStr = [[
screen.clear()
print("Hello from a stream program!")
gui.event()
]]

local FileStr = "ExampleStream.lua"
local Hnd = io.open(FileStr, "w")
if Hnd then
  Hnd:write(ProgStr)
  Hnd:close()
  gui.alert(FileStr .. " successfully created")
else
  gui.alert("Error creating " .. FileStr)
end
