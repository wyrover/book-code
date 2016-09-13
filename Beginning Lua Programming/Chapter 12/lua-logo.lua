require "gd"

local Lcl = {}

local function LclWriteCtr(Hnd, X, Y, Clr, FontStr, PtSize, TextStr)
  local Ext = {gd.stringFT(nil, Clr, FontStr, PtSize, 0, 0, 0, TextStr)}
  Hnd:stringFT(Clr, FontStr, PtSize, 0, X - (Ext[3] - Ext[1]) / 2,
    Y + (Ext[4] - Ext[6]) / 2, TextStr)
end

Lcl.Size = 256
--Lcl.FontStr = "c:/windows/fonts/arial.ttf" -- Windows
Lcl.FontStr = "vera " -- Linux and friends
Lcl.Scale = 1 - math.sqrt(2) / 2
Lcl.RadiusLg = Lcl.Size / 3
Lcl.DiameterLg = 2 * Lcl.RadiusLg
Lcl.RadiusSm = Lcl.Scale * Lcl.RadiusLg
Lcl.DiameterSm = 2 * Lcl.RadiusSm
Lcl.CenterX = (Lcl.Size - Lcl.RadiusSm / 2) / 2
Lcl.CenterY = (Lcl.Size + Lcl.RadiusSm / 2) / 2

Hnd = gd.createTrueColor(Lcl.Size, Lcl.Size)
Lcl.White = Hnd:colorAllocate(255, 255, 255)
Lcl.Blue = Hnd:colorAllocate(32, 32, 128)
Lcl.Gray = Hnd:colorAllocate(192, 192, 192)
Lcl.Black = Hnd:colorAllocate(0, 0, 0)
Hnd:setAntiAliased(Lcl.Blue)
Hnd:filledRectangle(0, 0, Lcl.Size - 1, Lcl.Size - 1, Lcl.White)
-- Planet
Hnd:filledArc(Lcl.CenterX, Lcl.CenterY, Lcl.DiameterLg, Lcl.DiameterLg,
  0, 360, gd.ANTI_ALIASED, gd.ARC)
-- Moon
Hnd:filledArc(Lcl.CenterX + Lcl.RadiusLg, Lcl.CenterY - Lcl.RadiusLg,
  Lcl.DiameterSm, Lcl.DiameterSm, 0, 360, gd.ANTI_ALIASED, gd.ARC)
-- Moonshadow
Hnd:setAntiAliased(Lcl.White)
Hnd:filledArc(Lcl.CenterX + Lcl.RadiusLg - Lcl.DiameterSm,
  Lcl.CenterY - Lcl.RadiusLg + Lcl.DiameterSm,
  Lcl.DiameterSm, Lcl.DiameterSm, 0, 360, gd.ANTI_ALIASED, gd.ARC)
-- Orbit
Hnd:setAntiAliased(Lcl.Gray)
Hnd:arc(Lcl.CenterX, Lcl.CenterY,
  Lcl.DiameterLg + Lcl.DiameterSm, Lcl.DiameterLg + Lcl.DiameterSm, 0, 300,
  gd.ANTI_ALIASED, gd.ARC)
Hnd:arc(Lcl.CenterX, Lcl.CenterY,
  Lcl.DiameterLg + Lcl.DiameterSm, Lcl.DiameterLg + Lcl.DiameterSm, 330, 360,
  gd.ANTI_ALIASED, gd.ARC)
-- Text
LclWriteCtr(Hnd, Lcl.CenterX, Lcl.CenterY + Lcl.RadiusSm, Lcl.White,
  Lcl.FontStr, 48, "Lua")
Hnd:png("logo.png")
Hnd:gif("logo.gif")
Hnd:jpeg("logo.jpg", 90)
os.execute("start logo.png")
