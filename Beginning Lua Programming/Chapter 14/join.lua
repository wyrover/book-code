-- A demonstration of a simple relational database with join
-- functionality entirely in Lua.

-- Example database tables:
Cust = {
  {Id = "C001", NameLast = "Bumppo", NameFirst = "Natty"},
  {Id = "C002", NameLast = "Finn", NameFirst = "Huckleberry"},
  {Id = "C003", NameLast = "Darcy", NameFirst = "Fitzwilliam"},
  {Id = "C004", NameLast = "Bennet", NameFirst = "Elizabeth"},
  {Id = "C005", NameLast = "Marner", NameFirst = "Silas"},
}
Product = {
  {Id = "P001", DescStr = "whatchamacallit"},
  {Id = "P002", DescStr = "gizmo"},
  {Id = "P003", DescStr = "gewgaw"},
  {Id = "P004", DescStr = "thingamajig"},
  {Id = "P005", DescStr = "widget"},
  {Id = "P006", DescStr = "doodad"},
  {Id = "P007", DescStr = "whatsit"},
}
Order = {
  {Id = "O001", CustId = "C003", ProductId = "P002", Count = 52},
  {Id = "O002", CustId = "C003", ProductId = "P004", Count = 87},
  {Id = "O003", CustId = "C004", ProductId = "P001", Count = 12},
  {Id = "O004", CustId = "C004", ProductId = "P003", Count = 8},
  {Id = "O005", CustId = "C004", ProductId = "P005", Count = 20},
  {Id = "O006", CustId = "C002", ProductId = "P004", Count = 2},
}

-- Returns a new database table composed of the selected columns of
-- Tbl1 and Tbl2 where WhereFnc is true:
function Join(Tbl1, Tbl1Select, Tbl2, Tbl2Select, WhereFnc)
  local Ret = {}
  -- For each pairing of rows:
  for _, Tbl1Row in ipairs(Tbl1) do
    for _, Tbl2Row in ipairs(Tbl2) do
      if WhereFnc(Tbl1Row, Tbl2Row) then
        -- WhereFnc is true, so include the selected fields of this
        -- pairing in the result:
        local RetRow = {}
        for _, Tbl1Col in ipairs(Tbl1Select) do
          RetRow[Tbl1Col] = Tbl1Row[Tbl1Col]
        end
        for _, Tbl2Col in ipairs(Tbl2Select) do
          RetRow[Tbl2Col] = Tbl2Row[Tbl2Col]
        end
        Ret[#Ret + 1] = RetRow
      end
    end
  end
  return Ret
end

-- Space-padding helper function for ShowDbTbl:
local function ShowField(Str, Width, Numeric)
  local Pad = string.rep(" ", Width - string.len(Str))
  if Numeric then
    io.write(Pad, Str, "  ")
  else
    io.write(Str, Pad, "  ")
  end
end

-- Displays a database table (an array of associative rows):
function ShowDbTbl(Tbl)
  local ColList = {}
  -- Get the width of each column name and type of each column:
  for ColStr, Val in pairs(Tbl[1]) do
    ColList[#ColList + 1] = {Name = ColStr,
      Width = string.len(ColStr), Numeric = type(Val) == "number"}
  end
  -- Get the maximum width of each column:
  for _, Row in ipairs(Tbl) do
    for _, Col in ipairs(ColList) do
      Col.Width = math.max(string.len(Row[Col.Name]), Col.Width)
    end
  end
  -- Display a column header:
  for _, Col in ipairs(ColList) do
    ShowField(Col.Name, Col.Width, Col.Numeric)
  end
  io.write("\n")
  for _, Col in ipairs(ColList) do
    ShowField(string.rep("-", Col.Width), Col.Width, false)
  end
  io.write("\n")
  -- Display the rows:
  for _, Row in ipairs(Tbl) do
    for _, Col in ipairs(ColList) do
      ShowField(Row[Col.Name], Col.Width, Col.Numeric)
    end
    io.write("\n")
  end
end

-- Demonstration:
CustOrder = Join(Cust, {"NameLast"},
  Order, {"ProductId", "Count"},
  function(Cust, Order) return Cust.Id == Order.CustId end)
print("*** Cust joined to Order ***")
ShowDbTbl(CustOrder)
print()
CustOrderProduct = Join(CustOrder, {"NameLast", "Count"},
  Product, {"DescStr"},
  function(CustOrder, Product)
    return CustOrder.ProductId == Product.Id
  end)
print("*** Cust joined to Order joined to Product ***")
ShowDbTbl(CustOrderProduct)
print()
Bennet = Join(CustOrder, {"Count"},
  Product, {"DescStr"},
  function(CustOrder, Product)
    return CustOrder.ProductId == Product.Id
      and CustOrder.NameLast == "Bennet"
  end)
print("*** All orders by customer 'Bennet' ***")
ShowDbTbl(Bennet)
