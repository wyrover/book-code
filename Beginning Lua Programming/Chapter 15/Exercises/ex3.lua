#!/usr/local/bin/lua

local Cgi = require("cgi")

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

local function CalendarShow(Year, Month)

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

  local Dow, DayMax = Calendar(Year, Month)

  io.write(Cn.HdrStr)

  io.write('<table class="data shade" border="0" cellspacing="0" ',
    'cellpadding="5" summary="" width="100%">\n',
    '<tbody>\n',
    '<tr><th colspan="7">', os.date('%B %Y', os.time{year = Year,
      month = Month, day = 1}), '</th></tr>\n',
    '<tr>\n')
  local Base = 8 - Dow
  for Col = 1, 7 do
    local Wd = (Col == 1 or Col == 7) and ' edge' or ''
    io.write('<td class="name', Wd, '">', os.date("%a",
      os.time{year = Year, month = Month, day = Base + Col}),
      '</td>\n')
  end
  io.write('</tr>\n')
  local Day = 2 - Dow
  while Day <= DayMax do
    io.write('<tr>')
    for Col = 1, 7 do
      io.write('<td>', (Day >= 1 and Day <= DayMax) and Day or '&nbsp;',
        '</td>\n')
      Day = Day + 1
    end
    io.write("</tr>\n")
  end
  io.write('</tbody></table>\n')
  io.write(Cn.FtrStr)

end

local function CalendarRequest(ScriptStr)
  io.write(Cn.HdrStr, '<h1>Calendar Request</h1>\n',
    '<form action="', ScriptStr, '" method="get">\n',
    '<table class="shade" summary=""><tbody>\n',
    '<tr>\n',
    '<td class="label">Month</td>\n',
    '<td>\n',
    '<select name="Month" size="1">\n')
  local Today = os.date("*t")
  for J = 1, 12 do
    local AttrStr = Today.month == J and ' selected="selected"' or ""
    local MonthStr = os.date("%B", os.time{year=2000, month=J, day = 1})
    io.write('<option', AttrStr, ' value="', J, '">', MonthStr, '</option>\n')
  end
  io.write('</select>\n',
    '</td>\n',
    '<td class="label">Year</td>\n',
    '<td>\n',
    '<select name="Year" size="1">\n')
  for J = Today.year - 12, Today.year + 12 do
    local AttrStr = Today.year == J and ' selected="selected"' or ""
    io.write('<option', AttrStr, ' value="', J, '">', J, '</option>\n')
  end
  io.write('</select>\n',
    '</td>\n',
    '<td><input type="submit" value="Submit" /></td>\n',
    '</tr>\n',
    '</tbody></table>\n',
    '</form>\n',
    Cn.FtrStr)
end

-- If UserMonth and UserYear were passed in and are valid, present the
-- requested calendar, otherwise present a request form.

local Prm = Cgi.Params()
local UserMonth, UserYear
local ShowForm = true
UserMonth = tonumber(Prm.Get.Month)
if UserMonth and UserMonth >= 1 and UserMonth <= 12 then
  UserYear = tonumber(Prm.Get.Year)
  if UserYear and UserYear >= 1970 and UserYear < 2038 then
    ShowForm = false
  end
end

if ShowForm then
  if Prm.SCRIPT_NAME then
    CalendarRequest(Prm.SCRIPT_NAME)
  else
    io.write(Cn.HdrStr, "<p>Error: could not determine name of script.</p>\n",
      Cn.FtrStr)
  end
else
  CalendarShow(UserYear, UserMonth)
end
