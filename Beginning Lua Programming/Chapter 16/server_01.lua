local socket = require("socket")

local Addr = arg[1] or "127.0.0.1"
local Port = tonumber(arg[2] or 11250)
local Str, Len, SckHnd, ClientHnd, ErrStr, BindAddr, BindPort, ClAddr, ClPort
SckHnd, ErrStr = socket.bind(Addr, Port)
if SckHnd then
  BindAddr, BindPort = SckHnd:getsockname()
  io.write("Listening on ", BindAddr, ", port ", BindPort, "\n")
  ClientHnd, ErrStr = SckHnd:accept()
  if ClientHnd then
    ClAddr, ClPort = ClientHnd:getpeername()
    io.write("Connection from ", ClAddr, ", port ", ClPort, "\n")
    Str = string.format("Greetings from %s:%d to %s:%d\r\n",
      BindAddr, BindPort, ClAddr, ClPort)
    Len, ErrStr = ClientHnd:send(Str)
    if Len then
      Str, ErrStr = ClientHnd:receive()
      if Str then
        io.write("Received from client: [", Str, "]\n")
      else
        io.write("Receive error: ", ErrStr, "\r\n")
      end
      ClientHnd:shutdown("both")
    else
      io.write("Send error: ", ErrStr, "\n")
    end
    ClientHnd:close()
  else
    io.write("Client connection. ", ErrStr, "\n")
  end
  SckHnd:close()
else
  io.write("Listening socket. ", ErrStr, "\n")
end
