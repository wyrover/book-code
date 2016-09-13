#!/usr/local/bin/lua

local Cgi = require("cgi")
require("show")

local Cn = {}

Cn.HdrStr = [[Content-Type: text/html

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>

<head>

  <title>CGI Environment</title>

  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

  <link rel="stylesheet" href="/css/general.css" type="text/css" />

</head>

<body>

]]

Cn.FtrStr = "</body>\n\n</html>"

local Prm = Cgi.Params()
local List = {}
ObjectDescribe(List, Prm, "CGI parameters")
io.write(Cn.HdrStr, '<p class="hdr shade">CGI Environment</p>\n')
for J, Rec in ipairs(List) do
  local Key, Val = string.match(Rec[2], '^%[%"?(.-)%"?%]%s+(.+)$')
  if Key then
    io.write('<div style="margin-left:', Rec[1] * 0.5, 'cm;">',
      '<b>', Cgi.Escape(Key), '</b> ', Cgi.Escape(Val), '</div>\n')
  else
    io.write(Rec[2], '<br />\n')
  end
end
io.write(Cn.FtrStr)
