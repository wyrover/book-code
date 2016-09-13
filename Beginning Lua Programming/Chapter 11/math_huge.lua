NumListA = {2, 5, 3}
NumListB = {5, 3, 2}
local Max = -math.huge
for I, NumA in ipairs(NumListA) do
  local NumB = NumListB[I]
  Max = math.max(Max, NumA * NumB)
end
io.write("The maximum product was ", Max, ".\n")
