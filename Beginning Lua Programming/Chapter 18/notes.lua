-- Returns a table in which keys are names and values are associated notes.

local function ListInitialize()
  -- Simulate reading data from a database. This can be replaced with actual
  -- Palm OS database access.
  return {
    Dante = "1265-1321",
    Petrarch = "1304-1374",
    Leonardo = "1452-1519",
    Copernicus = "1473-1543",
    Michelangelo = "1475-1564",
    Cervantes = "1547-1616",
    Galileo = "1564-1642",
    Shakespeare = "1564-1616",
    Donne = "1572-1631",
    Milton = "1608-1674"}
end

-- Stores data from associative data table

local function ListFinalize(List)
  -- Store modified data here.
end

-- Place a button at position X, return button ID

local function LclButton(X, Str)
  local Id
  screen.moveto(X)
  Id = gui.control{type = "button", text = Str}
  gui.nl()
  return Id
end

-- Initialize main form

local function LclForm(Lcl)
  local X, Y
  Lcl.ScrWd, Lcl.ScrHt = screen.mode()
  Lcl.ScrUnit = Lcl.ScrWd / 160
  Lcl.DlgBrd = Lcl.ScrUnit * 2
  screen.clear()
  gui.title(Lcl.TitleStr)
  screen.moveto(1)
  Lcl.ListRowCount = 8
  Lcl.CtlList = gui.control{type = "list", lines = Lcl.ListRowCount,
    columns = 21, selected = 4, list = Lcl.ShowList}
  X, Y = screen.pos()
  gui.nl()
  screen.moveto(1)
  Lcl.CtlNote = gui.control{type = "field", lines = 4, columns = 28,
    limit = 30000, editable = false, underlined = false}
  X = X - Lcl.ScrUnit
  screen.moveto(X, Y)
  Lcl.CtlNew = LclButton(X, "New")
  Lcl.CtlEdit = LclButton(X, "Edit")
  Lcl.CtlDel = LclButton(X, "Delete")
  gui.setfocus(Lcl.CtlList)
end

-- Raises record editor dialog. If NameStr is nil, it will
-- be editable, otherwise noneditable

local function RecDlg(Lcl, NameStr, NoteStr)
  local CtlName, CtlNote, CtlOk, CtlCancel,
    Name, Note
  local New = NameStr == nil
  local Border = 2 * Lcl.DlgBrd
  gui.dialog(0, 0, Lcl.ScrWd - Border, Lcl.ScrHt - Border,
    New and "New record" or "Edit record")
  gui.control{type = "label", text = "Name"}
  gui.nl()
  CtlName = gui.control{type = "field", lines = 1,
    columns = 27, limit = 27, text = NameStr or "",
    editable = New},
  gui.nl()
  gui.control{type = "label", text = "Note"}
  gui.nl()
  CtlNote = gui.control{type = "field", lines = 6,
    columns = 27, limit = 1024, text = NoteStr or ""},
  gui.nl()
  CtlOk = gui.control{type = "button", text = "OK"}
  CtlCancel = gui.control{type = "button", text = "Cancel"}
  gui.setfocus(New and CtlName or CtlNote)
  local Loop = true
  while Loop do
    local Evt, Id = gui.event()
    if Evt == appStop then
      Loop = false
    elseif Evt == ctlSelect then
      Loop = false
      if Id == CtlOk then
        Name = gui.gettext(CtlName)
        if string.len(Name) > 0 then
          Note = gui.gettext(CtlNote)
        else
          Name = nil
        end
      end
    end
  end
  gui.destroy()
  return Name, Note -- May be nil
end

-- Returns currently selected name

local function RecName(Lcl)
  local Name, Pos
  Pos = gui.getstate(Lcl.CtlList)
  if Pos then
    Name = Lcl.ShowList[Pos]
  end
  return Name
end

-- Updates note field in main form based on currently selected record

local function NoteRefresh(Lcl)
  local Note, Name
  Name = RecName(Lcl)
  if Name then
    Note = Lcl.DataList[Name]
  end
  gui.settext(Lcl.CtlNote, Note or "")
end

-- Refreshes selection list on main form. Pos specifies new selection. Nil: no
-- change; number: new selection by position; string: new position by name.

local function ListRefresh(Lcl, Pos)
  local J = 1
  for Key in pairs(Lcl.DataList) do
    Lcl.ShowList[J] = Key
    J = J + 1
  end
  Lcl.ShowList[J] = nil
  table.sort(Lcl.ShowList)
  gui.settext(Lcl.CtlList, Lcl.ShowList)
  local PosCat = type(Pos)
  if PosCat == "number" then
    gui.setstate(Lcl.CtlList, Pos)
  elseif PosCat == "string" then
    local J, Count, NewPos = 1, table.getn(Lcl.ShowList)
    while J <= Count and not NewPos do
      if Lcl.ShowList[J] == Pos then
        NewPos = J
      else
        J = J + 1
      end
    end
    if NewPos then
      gui.setstate(Lcl.CtlList, NewPos)
    end
  end
  NoteRefresh(Lcl)
end

-- Handles addition of new record

local function RecNew(Lcl)
  local Name, Note = RecDlg(Lcl)
  if Name then
    if Lcl.DataList[Name] then
      gui.alert("Duplicate name: " .. Name)
    else
      Lcl.DataList[Name] = Note
      ListRefresh(Lcl, Name)
      Lcl.Modified = true
    end
  end
end

-- Handles modification of existing record

local function RecEdit(Lcl)
  local Name, Note
  Name = RecName(Lcl)
  if Name then
    Name, Note = RecDlg(Lcl, Name, Lcl.DataList[Name])
    if Name then
      Lcl.DataList[Name] = Note
      ListRefresh(Lcl)
      Lcl.Modified = true
    end
  end
end

-- Shift the currently selected record with hardware button

local function RecScroll(Lcl, Key)
  local Pos = gui.getstate(Lcl.CtlList)
  local NewPos
  local Count = table.getn(Lcl.ShowList) -- Lua 5.0
  if Key == hardKeyDown then -- Down
    NewPos = Pos + 1
  elseif Key == hardKeyUp then -- Up
    NewPos = Pos - 1
  elseif Key == hardKey1 then -- Top
    NewPos = 1
  elseif Key == hardKey2 then -- Page up
    NewPos = Pos - Lcl.ListRowCount
  elseif Key == hardKey3 then -- Page down
    NewPos = Pos + Lcl.ListRowCount
  elseif Key == hardKey4 then -- Bottom
    NewPos = Count
  end
  if NewPos then
    if NewPos < 1 then
      NewPos = 1
    elseif NewPos > Count then
      NewPos = Count
    end
    if NewPos ~= Pos then
      gui.setstate(Lcl.CtlList, NewPos)
      NoteRefresh(Lcl)
    end
  end
end

-- Handles deletion of currently selected record

local function RecDel(Lcl)
  local Name = RecName(Lcl)
  if Name then
    Lcl.DataList[Name] = nil
    ListRefresh(Lcl)
    Lcl.Modified = true
  end
end

-- Main initialization and event loop

local Lcl = {TitleStr = "Notes"}
screen.clear()
Lcl.DataList = ListInitialize()
Lcl.ShowList = {}
LclForm(Lcl)
ListRefresh(Lcl, 1)
local Loop = true
while Loop do
  local Evt, Id = gui.event()
  if Evt == appStop then
    Loop = false
  elseif Evt == ctlSelect then
    if Id == Lcl.CtlNew then
      RecNew(Lcl)
    elseif Id == Lcl.CtlEdit then
      RecEdit(Lcl)
    elseif Id == Lcl.CtlDel then
      RecDel(Lcl)
    end
  elseif Evt == lstSelect then
    NoteRefresh(Lcl)
  elseif Evt == keyDown then
    RecScroll(Lcl, Id)
  end
end
if Lcl.Modified then
  ListFinalize(Lcl.DataList)
end
