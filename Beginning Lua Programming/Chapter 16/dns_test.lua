local socket = require("socket")
require("show")

local Ip = {socket.dns.toip("www.lua.org")}

ObjectShow(Ip, "toip")
