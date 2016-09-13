-- This file contains some of the functions defined in
-- Chapter 10.  They can be individually cut-and-pasted into
-- an interpreter session, or you can run the whole file and
-- access them from the interpreter by using lua's -i option
-- or the dofile function.

-- Garbage-collection example:
function GarbageTest(Len)
  -- Clean up any garbage:
  collectgarbage("collect")
  for I = 0, 9 do
    -- Create a Len-character string and put it in a variable
    -- that will go out of scope at the end of this iteration:
    local Junk = string.rep(I, Len)
    -- Print a line of a bar chart showing memory usage:
    print(string.rep("#", collectgarbage("count"))) -- This use
      -- of string.rep increases memory usage a bit too, but
      -- that can be ignored.
  end
end

-- Out-of-scope variable example:
I = "global"

function Inner()
  local I = "inside Inner, outside loop"
  for I = 1, 10 do
    if I == 5 then
      debug.debug()
    end
    print("I (inside loop): " .. I)
  end
end

function Outer()
  local I = "inside Outer"
  Inner()
end

-- Upvalue example:
do
  local N = 0

  -- Returns a number one higher than the last every time
  -- it's called:
  function Counter()
    N = N + 1
    return N
  end
end

-- Tail-call example:
do -- local scope for Count.
  local Count = 5

  -- Keeps tail-calling itself until Count is 0:
  function Tail()
    if Count == 0 then
      return
    else
      Count = Count - 1
      return Tail()
    end
  end
end
