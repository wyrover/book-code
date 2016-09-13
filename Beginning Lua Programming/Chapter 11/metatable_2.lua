local function WindowCreate(TitleStr)

  local function WindowTextGet()
    -- Simulate GUI call
    io.write("  Retrieving ", TitleStr, "\n")
    return TitleStr
  end

  local function WindowTextSet(Str)
    -- Simulate GUI call
    local OldTitleStr = TitleStr
    io.write("  Setting ", Str, "\n")
    TitleStr = Str
    return OldTitleStr
  end

  local function MtIndex(Tbl, Key)
    return Key == "Title" and WindowTextGet() or Tbl[Key]
  end

  local function MtNewIndex(Tbl, Key, Val)
    if Key == "Title" then
      WindowTextSet(Val)
    else
      Tbl[Key] = Val
    end
  end

  local Wnd, MtTbl = {}, {__index = MtIndex, __newindex = MtNewIndex}

  setmetatable(Wnd, MtTbl)

  return Wnd

end

local Wnd = WindowCreate("Title 1")
local Str = Wnd.Title
io.write("1 ", Str, "\n")
Wnd.Title = "Title 2"
Str = Wnd.Title
io.write("2 ", Str, "\n")
rawset(Wnd, "Title", "Title 3")
Str = rawget(Wnd, "Title")
io.write("3 ", Str, "\n")
Wnd.Title = "Title 4" -- No metamethod
Str = Wnd.Title -- No metamethod
io.write("4 ", Str, "\n")
