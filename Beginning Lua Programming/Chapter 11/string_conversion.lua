local Str = "Lua"
io.write(string.format("Normal: '%s', reversed: '%s', " ..
  "lower: '%s', upper: '%s'\n\n",
  Str, string.reverse(Str), string.lower(Str), string.upper(Str)))
for J, TblStr in ipairs{"coroutine", "debug", "io", "math",
  "os", "package", "string", "table"} do
  local Count = 0
  for FldStr in pairs(_G[TblStr]) do
    Count = Count + 1
  end
  io.write(string.format("%s%s%d\n", TblStr, string.rep(".",
    14 - string.len(TblStr) - string.len(Count)), Count))
end
