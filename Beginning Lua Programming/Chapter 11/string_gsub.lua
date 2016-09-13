-- Generate list of month names in current locale
local Dt = os.date("*t")
Dt.day = 1
local MonthList = {}
for Month = 1, 12 do
  Dt.month = Month
  MonthList[Month] = os.date("%B", os.time(Dt))
end

function ExpandDate(Month, Day, Sep)
  return (MonthList[tonumber(Month)] or "---") .. " " ..
    tonumber(Day) .. (Sep == "," and ", " or "")
end

local Str = "0117,0211,0818"
io.write("Before: ", Str, "\n")
Str = string.gsub(Str, "(%d%d)(%d%d)(%,?)", ExpandDate)
io.write("After:  ", Str, "\n")
