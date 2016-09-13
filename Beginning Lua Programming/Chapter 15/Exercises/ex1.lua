#!/usr/local/bin/lua

local Cgi = require("cgi")

local Cn = {}

Cn.HdrStr = [[Content-Type: text/html

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>
<head>
  <title>Greeting</title>
  <link rel="stylesheet" href="/css/general.css" type="text/css" />
</head>
<body>

]]

Cn.FtrStr = "</body>\n\n</html>"

local Rec = os.date("*t")
local Hr = Rec.hour
local Str
if Hr >= 3 then
  if Hr >= 12 then
    if Hr >= 18 then
      Str = "Good evening"
    else
      Str = "Good afternoon"
    end
  else
    Str = "Good morning"
  end
else
  Str = "Good night"
end
local Prm = Cgi.Params()
Str = Str .. ", " .. (Prm.Get.Name or "Unknown user") .. "!"
io.write(Cn.HdrStr, '<p class="hdr shade">', Cgi.Escape(Str), '</p>', Cn.FtrStr)
