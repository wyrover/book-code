local socket = require("socket")

local Cn = {SrvPort = 23032, SrvAddr = "localhost"}

local SckHnd = socket.connect(Cn.SrvAddr, Cn.SrvPort)
if SckHnd then
  local RcvStr, ErrStr = SckHnd:receive()
  if RcvStr then
    io.write(RcvStr, "\n")
    local SendList = {"One", "Two", "Three", ""}
    for J, Str in ipairs(SendList) do
      SckHnd:send(Str .. "\r\n")
      local RcvStr, ErrStr = SckHnd:receive()
      if RcvStr then
        io.write(RcvStr, "\n")
      elseif ErrStr ~= "closed" then
        io.write("Error: ", ErrStr, "\n")
      end
    end
  else
    io.write("Error: ", ErrStr, "\n")
  end
  SckHnd:close()
else
  io.write("Error creating client socket\n")
end
