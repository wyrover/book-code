local function Multitask()

  local function LclWork(Id, Count) -- Simulate some work
    for J = 1, Count do
      io.write(Id)
      io.flush()
      local Stop = os.time() + 1
      while os.time() < Stop do end
      coroutine.yield()
    end
  end

  local function LclContinue() -- Simulate check for user cancellation
    return math.random(12) > 1
  end

  local WorkA = coroutine.create(function () LclWork('A', 2) end)
  local WorkB = coroutine.create(function () LclWork('B', 4) end)
  math.randomseed(os.time())
  local A, B, Ok = true, true, true

  while (A or B) and Ok do
    Ok = LclContinue()
    if Ok and A then
      A = coroutine.resume(WorkA)
    end
    Ok = LclContinue()
    if Ok and B then
      B = coroutine.resume(WorkB)
    end
  end
  io.write(Ok and " done" or " cancel", "\n")

end

Multitask()
