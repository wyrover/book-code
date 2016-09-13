local http = require("socket.http")
require("show")

local ResFileStr = "lascii85.tar.gz"
local PathStr = "http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/5.0/"
local ResHnd, ErrStr = io.open(ResFileStr, "wb")
if ResHnd then
  local Req = {
    url = PathStr .. ResFileStr,
    sink = ltn12.sink.file(ResHnd),
    -- proxy = "http://proxy.example.com:8888"
  }
  local Response = {http.request(Req)}
  ObjectShow(Response, "Response")
else
  io.write("Error opening ", ResFileStr, " for writing\n")
end
