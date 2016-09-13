Tm = os.clock()
for J = 1, 1e6 do
  Tbl = {}
end
io.write("Loop: ", os.clock() - Tm, " sec\n")local Count = 1e7
local Tbl = {}
local Tm = os.clock()
for J = 1, Count do
  table.insert(Tbl, J)
end
io.write("table.insert: ", os.clock() - Tm, " sec\n")
Tbl = {}
Tm = os.clock()
for J = 1, Count do
  Tbl[#Tbl + 1] = J
end
io.write("Indexed insert: ", os.clock() - Tm, " sec\n")
