local socket = require("socket")

local Cn = {SrvPort = 3072, SrvAddr = "localhost"}

local SckHnd = socket.connect(Cn.SrvAddr, Cn.SrvPort)
if SckHnd then
  local Loop = true
  local CnnSrvStr, CnnSrvPort = SckHnd:getpeername()
  local CnnNameStr = socket.dns.tohostname(CnnSrvStr)
  io.write(string.format("Connected to %s (%s) on port %d.\n",
    CnnSrvStr, CnnNameStr, CnnSrvPort))
  io.write('Issue .quit to end connection, .shutdown to terminate server.\n')
  while Loop do
    local Str, ErrStr
    io.write("Send: ")
    Str = io.read() or ".quit"
    SckHnd:send(Str .. "\r\n")
    Str, ErrStr = SckHnd:receive()
    if Str then
      io.write("Received: ", Str, "\n")
    else
      Loop = false
      if ErrStr == "closed" then
        io.write("Closing connection to server\n")
      else
        io.write("Error: ", ErrStr, "\n")
      end
    end
  end
  SckHnd:close()
else
  io.write("Error creating client socket\n")
end
