-- Converts a LuaSQL cursor to an array of associative tables:
function CursorToTbl(Cursor)
  local Ret = {}

  -- A wrapper for Cursor:fetch that lets it return a table
  -- without being given one:
  local function RowsIter()
    local Row = {}
    return Cursor:fetch(Row, "a") -- alphanumeric/associative
  end

  -- Get each row:
  for Row in RowsIter do
    table.insert(Ret, Row)
  end
  Cursor:close()
  return Ret
end
