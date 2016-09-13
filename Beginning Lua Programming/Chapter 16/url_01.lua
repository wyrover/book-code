local url = require("socket.url")
require("show")

local UrlStr = "http://natty:pathfinder@www.example.net:8888" ..
  "/susquehanna;loc?date=1793#title"
ObjectShow(url.parse(UrlStr), "URL")
