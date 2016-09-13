local function Fnc(A, B, C)
  local Sum, ErrStr
  A, B, C = tonumber(A), tonumber(B), tonumber(C)
  if A and B and C then
    Sum = A + B + C
  else
    ErrStr = "Three numbers expected"
  end
  return Sum, ErrStr
end

print("Position 1")
local Sum, ErrStr = Fnc(1, "2", 3)
print(Sum or ErrStr)
print("Position 2")
local Sum, ErrStr = Fnc(1, nil, 3)
print(Sum or ErrStr)
print("Position 3")
