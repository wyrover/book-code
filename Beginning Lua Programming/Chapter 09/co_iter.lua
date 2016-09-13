local function ReverseIPairs(Arr)
  local function Iter()
    for J = #Arr, 1, -1 do
      coroutine.yield(J, Arr[J])
    end
  end
  return coroutine.wrap(Iter)
end

for J, Str in ReverseIPairs({"one", "two", "three"}) do
  print(J, Str)
end
