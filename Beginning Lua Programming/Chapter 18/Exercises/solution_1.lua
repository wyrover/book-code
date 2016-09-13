-- star.lua

screen.clear()
local Wd, Ht = screen.mode()
gui.title("Star")
local X, Y = screen.pos()
screen.moveto(Wd / 2, Y + Ht / 10)
screen.heading(252)
for J = 1, 5 do
  screen.walk(2 * Wd / 3)
  screen.turn(144)
end
gui.event()
