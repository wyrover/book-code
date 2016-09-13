function JoinPairs(AList, BList)
  local function Iter()
    local Pos = 1
    local A, B = AList[Pos], BList[Pos]
    while A ~= nil and B ~= nil do
      coroutine.yield(A, B)
      Pos = Pos + 1
      A, B = AList[Pos], BList[Pos]
    end
  end
  return coroutine.wrap(Iter)
end
