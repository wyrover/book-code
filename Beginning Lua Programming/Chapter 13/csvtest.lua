require "csv"

local Str = 'Natty Bumppo,"Natty Bumppo, Pathfinder","Natty ""Hawkeye"" Bumppo"'

local SubStr, Pos

io.write("--- csv.parse ---\n")
Pos = 1
io.write(Str, "\n")
for J = 1, 10 do
  if Pos > 0 then
    SubStr, Pos = csv.parse(Str, Pos)
    io.write(string.format("Pos %3d, field [%s], escaped [%s]\n", Pos, SubStr,
      csv.escape(SubStr)))
  end
end

io.write("--- csv.cells ---\n")
for CellStr in csv.cells(Str) do
  io.write(CellStr, "\n")
end
