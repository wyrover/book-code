local function Fnc(A, B, C)
  A, B, C = tonumber(A), tonumber(B), tonumber(C)
  print(A and B and C and A + B + C
    or "Three numbers expected")
end

print("Position 1")
Fnc(1, "2", 3)
print("Position 2")
Fnc(1, "Lua", 3)
print("Position 3")
