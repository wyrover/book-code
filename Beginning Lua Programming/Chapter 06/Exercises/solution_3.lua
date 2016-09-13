local function zpcall(Fnc, Err, ...)
  local Args = {...}
  local function Relay()
    return Fnc(unpack(Args))
  end
  return xpcall(Relay, Err)
end

-- Test

local function Fnc(A, B, C)
  print("Here in Fnc.", A, B, C)
  print(A + B + C)
end

print(zpcall(Fnc, debug.traceback, 1, nil, 3))
