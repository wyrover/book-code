local A, B, C

local function Status(Str)
  io.write(string.format("%-8s A is %-10s C is %-10s (%s)\n",
    Str, coroutine.status(A), coroutine.status(C),
    tostring(coroutine.running() or "main thread")))
end

function A()
  Status("A")
end

function B()
  Status("B")
end

function C()
  Status("C 1")
  coroutine.resume(A)
  Status("C 2")
  B()
  Status("C 3")
  coroutine.yield()
  Status("C 4")
end

A = coroutine.create(A)
B = coroutine.wrap(B)
C = coroutine.create(C)
Status("Main 1")
coroutine.resume(C)
Status("Main 2")
coroutine.resume(C)
Status("Main 3")
