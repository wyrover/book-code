local socket = require("socket")

local Addr = arg[1] or "127.0.0.1"
local Port = tonumber(arg[2] or 11250)
local SckHnd, ErrStr, Str, ClAddr, ClPort, SrvAddr, SrvPort
SckHnd, ErrStr = socket.connect(Addr, Port, "127.0.0.1", 0)
if SckHnd then
  ClAddr, ClPort = SckHnd:getsockname()
  SrvAddr, SrvPort = SckHnd:getpeername()
  io.write("Connected with ", SrvAddr, " on port ", SrvPort, "\n")
  Str, ErrStr = SckHnd:receive()
  if Str then
    SckHnd:send(string.format("Greetings from %s:%d to %s:%d\r\n",
      ClAddr, ClPort, SrvAddr, SrvPort))
    io.write("Got [", Str, "] from server\n")
  else
    io.error("Error. ", ErrStr, "\n")
  end
  SckHnd:close()
else
  io.write("Connecting socket. ", ErrStr, "\n")
end
