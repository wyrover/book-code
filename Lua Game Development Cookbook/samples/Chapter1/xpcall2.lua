local function f1(a, b)
  assert((a == 1), "The first parameter must be equal to 1")
  print(b)
  return a+1
end

local function xpcall2(fn, ...)
  local arg = {...}
  return xpcall(
    -- function wrapper to pass function arguments
    function(...)
      return fn(unpack(arg))
    end,
    -- error function
    function(msg)
      return debug.traceback(msg, 3)
    end
  )
end

print(xpcall2(f1, 2, 'a'))
