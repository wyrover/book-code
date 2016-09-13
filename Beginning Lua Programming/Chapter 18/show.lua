local function LclVisit(Tbl, Val, Key, Lvl, Seen)
  Seen = Seen or {}
  Lvl = Lvl or 0
  local VtpStr = type(Val)
  local Vqt = VtpStr == "string" and '"' or ''
  local Kqt = type(Key) == "string" and '"' or ''
  local ValStr = tostring(Val)
  ValStr = string.gsub(ValStr, "table:%s*.*$", "(tbl)")
  ValStr = string.gsub(ValStr, "function:%s*.*$", "(fnc)")
  table.insert(Tbl, string.rep('    ', Lvl) ..
    tostring(Key) .. " " ..  Vqt .. ValStr .. Vqt)
  if VtpStr == "table" then
    if not Seen[Val] then
      Seen[Val] = true
      local SrtTbl = {}
      for K, V in Val do
        table.insert(SrtTbl, K)
      end
      table.sort(SrtTbl)
      for J, K in ipairs(SrtTbl) do
        LclVisit(Tbl, Val[K], K, Lvl + 1, Seen)
      end
    end
  end
end

function ObjectRender(Val, Key)
  local Tbl = {}
  LclVisit(Tbl, Val, Key, 0)
  return Tbl
end

local function Metrics()
  local Scr = {}
  Scr.Wd, Scr.Ht, Scr.Depth, Scr.Color = screen.mode()
  local Base = Scr.Wd / 160
  Scr.DlgBrd = Base * 2 -- Border
  Scr.ScrollWd = Base * 7
  Scr.ChWd, Scr.ChHt = screen.textsize("X") -- Character size
  Scr.Top = math.floor(Scr.ChHt * 3 / 2)
  return Scr
end

function ObjectShow(Obj, TitleStr)
  local Tbl = ObjectRender(Obj, TitleStr)
  local Scr = Metrics()
  local Wd, Ht = Scr.Wd - 2 * Scr.DlgBrd,
    Scr.Ht - 2 * Scr.DlgBrd
  gui.dialog(0, 0, Wd, Ht, TitleStr)
  local ChWd, ChHt = screen.font(0)
  local Cols = math.floor((Wd - Scr.ScrollWd) / ChWd) - 1
  local Rows = math.floor((Ht - 4 * ChHt) / ChHt)
  gui.control{type = "field", lines = Rows,
    columns = Cols, limit = 30000, editable = false,
    underlined = false, text = table.concat(Tbl, "\n")}
  gui.nl()
  gui.tab()
  local IdOk = gui.control{type = "button", text = "OK"}
  local Loop = true
  while Loop do
    local Evt, Id = gui.event()
    if Evt == ctlSelect then
       if Id == IdOk then
         Loop = false
       end
     elseif Evt == appStop then
       Loop = false
     end
  end
  gui.destroy()
end
