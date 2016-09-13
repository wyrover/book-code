local ServerStr = "localhost"
local Port = 23032
local CmdStr = "nc " .. ServerStr .. " " .. Port
-- local CmdStr = "plink -raw -P " .. Port .. " " .. ServerStr
local SendFileStr = "send.txt"

local Hnd = io.open(SendFileStr, "w")
if Hnd then
  Hnd:write("One\nTwo\nThree\n\n")
  Hnd:close()
  Hnd = io.popen(CmdStr .. " < " .. SendFileStr)
  if Hnd then
    for Str in Hnd:lines() do
      io.write(Str, "\n")
    end
    Hnd:close()
  else
    io.write("Error executing '", CmdStr, "'\n")
  end
else
  io.write("Error opening ", SendFileStr, " for writing\n")
end
