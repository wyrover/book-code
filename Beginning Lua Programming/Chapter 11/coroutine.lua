local function Average()
  local Co = coroutine.running()
  Val = coroutine.yield(tostring(Co) .. " is " .. coroutine.status(Co))
  local Count, Sum = 1, Val
  while true do
    Sum = Sum + coroutine.yield(Sum, Sum / Count)
    Count = Count + 1
  end
end

local CoAve = coroutine.create(Average)
local Code, Str = coroutine.resume(CoAve)
if Code then
  io.write(Str, "\n")
  local Ave = coroutine.wrap(Average)
  io.write(Ave(), "\n\n")
  io.write("Val A  Sum A  Ave A    Val B  Sum B  Ave B\n")
  io.write("-----  -----  -----    -----  -----  -----\n")
  math.random() -- On some systems that shall remain nameless, the
    -- first call to math.random is easy enough to predict that it
    -- destroys the illusion of randomness; throw it away.
  for J = 1, 6 do
    local Code, ValA, SumA, AveA, ValB, SumB, AveB
    ValA = math.random(0, 100)
    Code, SumA, AveA = coroutine.resume(CoAve, ValA)
    if Code then
      ValB = math.random(0, 100)
      SumB, AveB = Ave(ValB)
      io.write(string.format("%5d  %5d  %5.2f    %5d  %5d  %5.2f\n",
        ValA, SumA, AveA, ValB, SumB, AveB))
    end
  end
end
