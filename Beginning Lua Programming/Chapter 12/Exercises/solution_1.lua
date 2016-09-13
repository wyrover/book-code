require "sqlite3"

local Cn = {}
Cn.DbStr = "test.db"
Cn.InitStr = [[
  BEGIN TRANSACTION;
  DROP TABLE IF EXISTS T1;
  CREATE TABLE T1(A, B, C);
  INSERT INTO T1 VALUES (12, 91, 40);
  INSERT INTO T1 VALUES (27, 79,  5);
  INSERT INTO T1 VALUES (32, 66, 53);
  END TRANSACTION;
]]

local DbHnd, ErrStr = sqlite3.open(Cn.DbStr)
if DbHnd then
  if DbHnd:exec(Cn.InitStr) then
    DbHnd:set_function("format", -1, string.format)
    print(DbHnd:first_cols("SELECT FORMAT('%05d %05d', 12, 34)"))
    for Row in DbHnd:rows("SELECT FORMAT('A %05d, B %05d, C %05d', A, B, C) " ..
      "AS Rec FROM T1") do
      print(Row.Rec)
    end
  else
    io.write("Error initializing ", Cn.DbStr, "\n")
  end
  DbHnd:close()
else
  io.write("Error opening ", Cn.DbStr, ": ", tostring(ErrStr), "\n")
end
