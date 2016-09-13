local socket = require("socket")
require "show"

local Cn = {}

Cn.Host = "localhost"

Cn.Port = 80

Cn.MimeList = {
  css = "text/css",
  gif = "image/gif",
  htm = "text/html",
  html = "text/html",
  png = "image/png",
  txt = "text/plain" }

local function LclHdrRead(ClSck)
  local Hdr = {}
  local LineStr, ErrStr
  LineStr, ErrStr = ClSck:receive()
  if LineStr then
    -- "GET /page.html HTTP/1.1" -> "GET", "page.html"
    Hdr.Cmd, Hdr.Path = string.match(LineStr, "^(%S+)%s+%/(%S*)")
    while LineStr do
      LineStr, ErrStr = ClSck:receive()
      if LineStr then
        if LineStr ~= "" then
          local Key, Val = string.match(LineStr, "^(.-)%:%s*(.*)$")
          Hdr[string.lower(Key)] = Val
        else
          LineStr = nil -- End loop at first blank line
        end
      end
    end
  end
  if (not Hdr.Path) or (Hdr.Path == "") then
    Hdr.Path = "index.html"
  end
  return Hdr
end

local function LclSend(Client, BodyStr, MimeStr, CodeStr)
  local SendStr =
    'HTTP/1.1 ' .. (CodeStr or '200 OK') .. '\r\n' ..
    'Date: ' .. os.date() .. '\r\n' ..
    'Server: webserver.lua/0.1\r\n' ..
    'Content-Length: ' .. string.len(BodyStr) .. '\r\n' ..
    'Content-Type: ' .. (MimeStr or 'text/html') .. '\r\n\r\n' .. BodyStr
  Client:send(SendStr)
  Client:shutdown() -- We're finished with this transaction
end

local function LclSendFile(Client, FileStr)
  local Hnd = io.open(FileStr, "rb")
  if Hnd then
    local Str = Hnd:read("*all")
    if Str then
      local ExtStr = string.lower(string.match(FileStr, "%P+$"))
      local MimeStr = Cn.MimeList[ExtStr] or "application/octet-stream"
      LclSend(Client, Str, MimeStr)
    else
      LclSend(Client, 'Error reading file.', 'text/plain',
        '500 Internal Server Error')
    end
    Hnd:close()
  else
    LclSend(Client, 'Error opening file.', 'text/plain', '404 Not Found')
  end
end

local Addr, Port, Server, Client, Hdr, Loop

Server = socket.bind(Cn.Host, Cn.Port)
if Server then
  Addr, Port = Server:getsockname()
  if Addr and Port then
    io.write("Waiting for connection from client on ", Addr, ":", Port, "\n")
    local PortStr = Port == 80 and "" or (":" .. Port)
    io.write('To end server, request "http://', Cn.Host, PortStr,
      '/quit" from browser\n')
    Loop = true
    while Loop do
      Client = Server:accept()
      if Client then
        io.write("Got client request\n")
        Addr, Port = Client:getpeername()
        if Addr and Port then
          io.write("Connected to ", Addr, ":", Port, "\n")
          Hdr = LclHdrRead(Client)
          ObjectShow(Hdr, "Hdr")
          if not string.find(Hdr.Path, "..", 1, true) then
            if Hdr.Path == "quit" then
              LclSend(Client, "Shutdown", "text/plain")
              Loop = false
            else
              LclSendFile(Client, Hdr.Path)
            end
          else
            LclSend(Client, 'Unauthorized', 'text/plain', '401 Unauthorized')
          end
        else
          io.write("Could not retrieve client address\n")
        end
        Client:close()
      else
        io.write("Error connecting to client\n")
      end
    end
    io.write("Ending server loop\n")
  else
    io.write("Could not retrieve server address\n")
  end
  Server:close()
else
  io.write("Error creating server socket\n")
end
