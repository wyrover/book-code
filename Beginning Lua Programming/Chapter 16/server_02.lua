local socket = require("socket")

local Cn = {HostPort = 3072, HostAddr = "*"}

local function ClientSession(SckHnd)
  local Loop, Str, ErrStr = true
  local Rcv = {}
  while Loop do
    coroutine.yield(Str)
    Str, ErrStr = SckHnd:receive()
    if Str then
      Loop = Str ~= ".quit" and Str ~= ".shutdown"
      if Loop then
        for J = 1, #Str do
          Rcv[string.byte(Str, J, J)] = true
        end
        local SendStr = ""
        for J = 33, 255 do
          if Rcv[J] then
            SendStr = SendStr .. string.char(J)
          end
        end
        SckHnd:send(SendStr .. "\r\n")
      end
    else
      io.write("Error: ", ErrStr, "\n")
    end
  end
  return Str
end

local SckHnd = socket.bind(Cn.HostAddr, Cn.HostPort)
if SckHnd then
  local SckList = {} -- Array of sockets, beginning with accepting socket
  local CoList = {}  -- Table of coroutines keyed by socket
  local Loop = true
  -- Prevent this socket from blocking for too long in call to accept
  SckHnd:settimeout(250)
  SckList[1] = SckHnd
  while Loop do
    io.write('Waiting for connection or data from\n')
    for J, Hnd in ipairs(SckList) do
      io.write('  ', J, ': ', tostring(Hnd), ', ', tostring(CoList[Hnd]), '\n')
    end
    local ReadTbl, WriteTbl, ErrStr = socket.select(SckList)
    for K, SckHnd in ipairs(ReadTbl) do
      if SckHnd == SckList[1] then -- Server socket
        local ClientHnd, ErrStr = SckHnd:accept()
        if ClientHnd then
          local NewPos = #SckList + 1
          SckList[NewPos] = ClientHnd
          CoList[ClientHnd] = coroutine.wrap(ClientSession)
          CoList[ClientHnd](ClientHnd)
        elseif ErrStr ~= "timeout" then
          io.write(ErrStr, "\n")
          Loop = false
        end
      else -- Client connection
        local Cmd = CoList[SckHnd]()
        if ".quit" == Cmd then
          CoList[SckHnd] = nil
          SckHnd:close()
          local L, Pos = #SckList
          while L > 1 do
            if SckHnd == SckList[L] then
              table.remove(SckList, L)
              L = 1 -- Terminate search
            else
              L = L - 1
            end
          end
        elseif ".shutdown" == Cmd then
          io.write("Shutting down server\n")
          Loop = false
        end
      end
    end
  end
  for J, SckHnd in ipairs(SckList) do
    SckHnd:close()
  end
else
  io.write("Error creating server socket\n")
end
