Tbl = {[0] = "zero", "one", "two", Id = 100, Name = "Alexander Botts",
  "three", [10] = "ten"}

io.write("--- ipairs ---\n")
for J, Val in ipairs(Tbl) do
  io.write(J, ": ", tostring(Val), "\n")
end

io.write("--- pairs ---\n")
for Key, Val in pairs(Tbl) do
  io.write(tostring(Key), ": ", tostring(Val), "\n")
end

io.write("--- next ---\n")
local Key, Val = next(Tbl)
while Key do
  io.write(tostring(Key), ": ", tostring(Val), "\n")
  Key, Val = next(Tbl, Key)
end
