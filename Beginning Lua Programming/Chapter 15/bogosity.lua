#!/usr/local/bin/lua

local Cn = {}

Cn.HdrStr = [[Content-Type: text/html

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>

<head>

  <title>Bogosity Index</title>

  <link rel="stylesheet" href="/css/general.css" type="text/css" />

</head>

<body>

]]

Cn.FtrStr = "</body>\n\n</html>"

-- This function returns an indexed table and bar chart specifier. The table
-- contains DayCount (default 31) simulated data records. Each record is an
-- indexed table of the form {date, value}. The chart specifier is a string
-- with each value separated by an underscore.

local function LclDataRetrieve(DayCount)
  DayCount = DayCount or 31
  local Lo, Hi, SecPerDay = -0.3, 2.3, 60 * 60 * 24
  local Tm = os.time() - DayCount * SecPerDay
  local ValList, RecList = {}, {}
  for J = Lo, Hi, (Hi - Lo) / DayCount do
    local DtStr = string.gsub(os.date("%A %d %B", Tm), " 0", " ")
    Tm = Tm + SecPerDay
    local Val = 100 + math.floor(300 * math.sin(J) + math.random(0, 60))
    table.insert(RecList, {DtStr, Val})
    table.insert(ValList, Val)
  end
  return RecList, table.concat(ValList, "_")
end

local Out = io.write
local CnColCount = 3
local List, UrlStr = LclDataRetrieve()
local RowCount = #List
local Split = math.ceil(RowCount / CnColCount)

Out(Cn.HdrStr, '<h1>Recent Bogon Flux Averages</h1>\n',
  '<div><img src="/cgi-bin/bar.lua?h50_cD0D0F0_l9090B0_ff6f6ff_', UrlStr,
  '" alt="Bogosity chart" /></div>\n',
  '<p class="note">Average bogon flux values (10<span class="super">9</span> ',
  'bogons / m<span class="super">2</span>) from ', List[1][1],
  ' to ', List[RowCount][1], '</p>\n',
  '<table summary=""><tbody><tr>\n')
local Row = 1
for Col = 1, CnColCount do
  Out('<td><table class="data shade" border="0" cellspacing="0" ',
  'cellpadding="5" summary="">\n',
  '<tbody>\n',
  '<tr><th>Date</th><th>Bogon Flux</th></tr>\n')
  for J = 1, Split do
    local Dt, Val
    local Rec = List[Row]
    if Rec then
      Dt, Val = Rec[1], Rec[2]
      Row = Row + 1
    else
      Dt, Val = "&nbsp;", "&nbsp;"
    end
    Out('<tr><td>', Dt, '</td><td class="right">', Val, '</td></tr>\n')
  end
  Out('</tbody>\n</table></td>\n')
end
Out('</tr></tbody></table>\n', Cn.FtrStr)
