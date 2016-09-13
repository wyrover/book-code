#!/usr/local/bin/lua

local Cn = {}

Cn.HdrStr = [[Content-Type: text/html

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>

<head>

  <title>Calendar</title>

  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

  <link rel="stylesheet" href="/css/general.css" type="text/css" />

  <style type="text/css">

  table.data td {border-right : 1px solid; border-bottom : 1px solid;
  border-color:inherit; color:#404040; background-color:#FFFFFF;
  vertical-align:top; width:14%;}

  table.data td.today {background-color:#f0f0ff;}

  table.data td.name {font-weight:bold; text-align:center; color:#505060;
  background-color:#f0f0ff;}

  table.data td.edge {width:15%;}

  </style>

</head>

<body>

]]

Cn.FtrStr = "</body>\n\n</html>"

local DaysInMonth = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }

-- For the specified month and year, this function returns the day of week of
-- the first day in the month, and the number of days in the month.

local function Calendar(Year, Month)
  assert(Year > 1900 and Year < 2100, "Year out of range")
  assert(Month >= 1 and Month <= 12, "Month out of range")
  local Rec = os.date("*t", os.time{year = Year, month = Month, day = 1})
  local DayMax = DaysInMonth[Rec.month]
  if DayMax == 0 then
    DayMax = math.mod(Rec.year, 4) == 0 and 29 or 28
  end
  return Rec.wday, DayMax
end

local Today = os.date("*t")
local Dow, DayMax = Calendar(Today.year, Today.month)

io.write(Cn.HdrStr)

io.write('<table class="data shade" border="0" cellspacing="0" ',
  'cellpadding="5" summary="" width="100%">\n',
  '<tbody>\n',
  '<tr><th colspan="7">', os.date('%B'), '</th></tr>\n',
  '<tr>\n')
local Base = 8 - Dow -- Map day to column
for Col = 1, 7 do
  local Wd = (Col == 1 or Col == 7) and ' edge' or ''
  io.write('<td class="name', Wd, '">', os.date("%a",
    os.time{year = Today.year, month = Today.month, day = Base + Col}),
    '</td>\n')
end
io.write('</tr>\n')
local Day = 2 - Dow -- Map day to column
while Day <= DayMax do
  io.write('<tr>')
  for Col = 1, 7 do
    io.write(Today.day == Day and '<td class="today">' or '<td>',
      (Day >= 1 and Day <= DayMax) and Day or '&nbsp;', '</td>\n')
    Day = Day + 1
  end
  io.write("</tr>\n")
end
io.write('</tbody></table>\n')
io.write(Cn.FtrStr)
