-- graph.lua

local Wd, Ht = screen.mode()
local Turn = 1
local Reduce = 0.98
local WalkLen = Wd
local Color = 0
local ColorInc = 1
local Min = 5

screen.clear()
screen.heading(90)
screen.moveto(0, 0)
repeat
  screen.color(screen.rgb(Color, Color, Color))
  Color = Color + ColorInc
  WalkLen = Reduce * WalkLen
  screen.turn(-(90 + Turn))
  screen.walk(WalkLen)
until WalkLen < Min
gui.event()
