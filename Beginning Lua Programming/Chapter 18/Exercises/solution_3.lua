-- barchart.lua

function Barchart(TitleStr, Tbl)
  local Count = table.getn(Tbl) -- Lua 5.0
  if Count > 0 then
    local ScrWd, ScrHt, ScrDepth = screen.mode()
    local ScrBase = ScrWd / 160
    screen.clear()
    gui.title(TitleStr)
    local X, Top = screen.pos()
    local BarWd, GapHrz, GapVrt = 3, 3, 4 * ScrBase
    if Count * (BarWd + GapHrz) - GapHrz <= ScrWd then
      BarWd = math.floor((ScrWd + GapHrz - (Count * GapHrz)) / Count)
      local Lf = math.floor((ScrWd + GapHrz - Count * (BarWd + GapHrz)) / 2)
      local Max = Tbl[1]
      for J, Val in ipairs(Tbl) do
        if Val > Max then
          Max = Val
        end
      end
      if Max > 0 then
        local Mlt = (ScrHt - Top - GapVrt - GapVrt) / Max
        local Clr = screen.rgb(128, 128, 128)
        X = Lf
        for J, Val in ipairs(Tbl) do
          local Ht = math.floor(Val * Mlt)
          screen.box(X, ScrHt - GapVrt - Ht, BarWd, Ht, Clr)
          X = X + BarWd + GapHrz
        end
        gui.event()
      else
        gui.alert("Values must be positive")
      end
    else
      gui.alert("Too many elements")
    end
  else
    gui.alert("Table is empty")
  end
end

Barchart("Example", {12, 34, 23, 25, 67, 45, 23, 7, 5, 54, 48, 50})
