require("luasql.mysql")

-- Executes all of the the SQL statements in List (stopping if one
-- fails):
function ExecSqlList(Conn, List)
  local Succ, ErrStr
  for _, SqlStr in ipairs(List) do
    Succ, ErrStr = Conn:execute(SqlStr)
    if not Succ then break end -- BREAK ON ERROR.
  end
  return Succ, ErrStr
end

-- Space-padding helper function for ShowCursor:
local function ShowField(Str, Width, Numeric)
  local Pad = string.rep(" ", Width - string.len(Str))
  if Numeric then
    io.write(Pad, Str, "  ")
  else
    io.write(Str, Pad, "  ")
  end
end

-- Displays all the rows in a database cursor, then closes the
-- cursor:
function ShowCursor(Cursor)
  ---- Get the name of each column and that name's width:
  local ColNames = Cursor:getcolnames()
  local ColWidths = {}
  for I, Name in ipairs(ColNames) do
    ColWidths[I] = string.len(Name)
  end
  -- Find out which columns hold numbers:
  local ColTypes = Cursor:getcoltypes()
  local ColIsNums = {}
  for I, Type in ipairs(ColTypes) do
    if string.sub(Type, 1, 6) == "number" then
      ColIsNums[I] = true
    else
      ColIsNums[I] = false
    end
  end

  -- A wrapper for Cursor:fetch that lets it return a table without
  -- being given one:
  local function RowsIter()
    local Row = {}
    return Cursor:fetch(Row)
  end

  -- Store all rows and the maximum widths of all columns:
  local Rows = {}
  for Row in RowsIter do
    table.insert(Rows, Row)
    for I, Field in ipairs(Row) do
      ColWidths[I] = math.max(ColWidths[I], string.len(Field))
    end
  end
  -- Display a column header:
  for I, ColName in ipairs(ColNames) do
    ShowField(ColName, ColWidths[I], ColIsNums[I])
  end
  io.write("\n")
  for I, ColWidth in ipairs(ColWidths) do
    ShowField(string.rep("-", ColWidth), ColWidth, false)
  end
  io.write("\n")
  -- Display the rows:
  for _, Row in ipairs(Rows) do
    for I, Field in ipairs(Row) do
      ShowField(Field, ColWidths[I], ColIsNums[I])
    end
    io.write("\n")
  end
  Cursor:close()
end

-- Creates the Cust, Product, and Ord tables:
function Create(Conn)
  return ExecSqlList(Conn, {[[
    CREATE TABLE Cust (
      Id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
      NameLast CHAR(50),
      NameFirst CHAR(50))]], [[
    CREATE TABLE Product (
      Id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
      DescStr CHAR(50))]], [[
    CREATE TABLE Ord (
      Id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
      CustId INT UNSIGNED,
      ProductId INT UNSIGNED,
      Count INT UNSIGNED)]]})
end

-- Puts rows into Cust, Product, and Ord:
function Populate(Conn)
  return ExecSqlList(Conn, {[[
    TRUNCATE TABLE Cust]], [[
    INSERT INTO Cust (NameLast, NameFirst) VALUES
      ('Bumppo', 'Natty'),
      ('Finn', 'Huckleberry'),
      ('Darcy', 'Fitzwilliam'),
      ('Bennet', 'Elizabeth'),
      ('Marner', 'Silas')]], [[
    TRUNCATE TABLE Product]], [[
    INSERT INTO Product (DescStr) VALUES
      ('whatchamacallit'), ('gizmo'), ('gewgaw'), ('thingamajig'),
      ('widget'), ('doodad'), ('whatsit')]], [[
    TRUNCATE TABLE Ord]], [[
    -- These CustIds and ProductIds are hardcoded, which means this
    -- INSERT would break if the previous INSERTs' auto-increment IDs
    -- didn't start at 1, but the TRUNCATEs make sure they do:
    INSERT INTO Ord (CustId, ProductId, Count) VALUES
      (3, 2, 52),
      (3, 4, 87),
      (4, 1, 12),
      (4, 3, 8),
      (4, 5, 20),
      (2, 4, 2)]]})
end

-- Does some sample SELECTs (joins) and displays the results:
function Test(Conn)
  local Cursor, ErrStr
  Cursor, ErrStr = Conn:execute([[
    SELECT NameLast, ProductId, Count
    FROM Cust, Ord
    WHERE Cust.Id = Ord.CustId]])
  if Cursor then
    print("*** Cust joined to Order ***")
    ShowCursor(Cursor)
    print()
    Cursor, ErrStr = Conn:execute([[
      SELECT NameLast, Count, DescStr
      FROM Cust, Ord, Product
      WHERE Cust.Id = Ord.CustId
        AND Ord.ProductId = Product.Id]])
    if Cursor then
      print("*** Cust joined to Order joined to Product ***")
      ShowCursor(Cursor)
      print()
      Cursor, ErrStr = Conn:execute([[
        SELECT Count, DescStr
        FROM Cust, Ord, Product
        WHERE Cust.Id = Ord.CustId
          AND Ord.ProductId = Product.Id
          AND Cust.NameLast = 'Bennet']])
      if Cursor then
        print("*** All orders by customer 'Bennet' ***")
        ShowCursor(Cursor)
      end
    end
  end
  return not ErrStr, ErrStr
end

-- Drops the Cust, Product, and Ord tables:
function Drop(Conn)
  return Conn:execute("DROP TABLE IF EXISTS Cust, Product, Ord")
end

-- Get what the LuaSQL documentation calls an "environment":
local MysqlEnv, ErrStr = luasql.mysql()
if MysqlEnv then
  -- Get a connection the "Lua" database as the user "Lua":
  local Conn
  Conn, ErrStr = MysqlEnv:connect("Lua", "Lua", "SmPlPaSs")
  if Conn then
    -- Obey the command given as a command-line argument:
    local Cmd = arg[1]
    local Succ
    if Cmd == "Create" then
      Succ, ErrStr = Create(Conn)
      if Succ then
        print("Successfully created Cust, Product, and Ord tables")
      end
    elseif Cmd == "Populate" then
      Succ, ErrStr = Populate(Conn)
      if Succ then
        print("Successfully populated Cust, Product, and Ord tables")
      end
    elseif Cmd == "Test" then
      Succ, ErrStr = Test(Conn)
    elseif Cmd == "Drop" then
      Succ, ErrStr = Drop(Conn)
      if Succ then
        print("Successfully dropped Cust, Product, and Ord tables")
      end
    else
      ErrStr = "A command-line argument of 'Create', 'Populate',"
        .. " 'Test', or 'Drop' is required"
    end
    Conn:close()
  end
  MysqlEnv:close()
end
if ErrStr then
  io.stderr:write(ErrStr, "\n")
end -- if
