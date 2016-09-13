require "sqlite3"

local Cn = {}
Cn.DbStr = "test.db"
Cn.InitStr = [[
  BEGIN TRANSACTION;
  DROP TABLE IF EXISTS T3;
  CREATE TABLE T3(A);
  INSERT INTO T3 VALUES (12);
  INSERT INTO T3 VALUES (27);
  INSERT INTO T3 VALUES (32);
  INSERT INTO T3 VALUES (91);
  INSERT INTO T3 VALUES (79);
  INSERT INTO T3 VALUES (66);
  INSERT INTO T3 VALUES (40);
  INSERT INTO T3 VALUES ( 5);
  INSERT INTO T3 VALUES (53);
  END TRANSACTION;
]]

local function LclStdDev()
  local Sum, List = 0, {}

  local function LclStep(Val)
    if type(Val) == "number" then
      table.insert(List, Val)
      Sum = Sum + Val
    end
  end

  local function LclFinal(Count)
    local StdDev = 0
    Count = #List
    if Count > 1 then
      local SumDevSq = 0
      local Mean = Sum / Count
      for J, Val in ipairs(List) do
        local Dev = Val - Mean
        SumDevSq = SumDevSq + Dev * Dev
      end
      StdDev = math.sqrt(SumDevSq / (Count - 1))
    end
    return StdDev
  end

  return LclStep, LclFinal
end

local DbHnd, ErrStr = sqlite3.open(Cn.DbStr)
if DbHnd then
  if DbHnd:exec(Cn.InitStr) then
    DbHnd:set_aggregate("stddev", 1, LclStdDev)
    print(DbHnd:first_cols("SELECT STDDEV(A) FROM T3"))
  else
    io.write("Error initializing ", Cn.DbStr, "\n")
  end
  DbHnd:close()
else
  io.write("Error opening ", Cn.DbStr, ": ", tostring(ErrStr), "\n")
end
