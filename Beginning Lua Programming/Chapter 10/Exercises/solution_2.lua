-- Gets the value of the named variable from its calling
-- scope:
function GetVar(Name)
  local CallerInfo = debug.getinfo(2, "uf")
  local Caller, Nups = CallerInfo.func, CallerInfo.nups
  local Found, FoundVal
  -- Look at the variables local to the caller; the last hit
  -- (if any) will be the desired one:
  local LclIndex = 1
  local Loop = true
  while Loop do
    local MaybeName, MaybeVal = debug.getlocal(2, LclIndex)
    if MaybeName then
      if MaybeName == Name then
        -- Found one hit (of possibly several):
        Found, FoundVal = true, MaybeVal
      end
      LclIndex = LclIndex + 1
    else
      -- No more locals; break the loop:
      Loop = false
    end
  end
  -- If it wasn't found as a local, search the caller's
  -- upvalues:
  if not Found then
    for UpIndex = 1, Nups do
      local MaybeName, MaybeVal = debug.getupvalue(Caller,
        UpIndex)
      if MaybeName == Name then
        -- FOUND IT! -- RECORD THE VALUE AND BREAK THE LOOP:
        Found, FoundVal = true, MaybeVal
        break
      end
    end
  end
  -- If it wasn't found as either a local or an upvalue, get
  -- it as a global (from the caller's environment):
  if not Found then
    FoundVal = getfenv(2)[Name]
  end
  return FoundVal
end

-- Call this function to run the test code given in the exercise:
function Test()
  Gl = "global"
  Up = "upvalue"
  L = "local"
  OL = "outer local"
  IL = "inner local"
  A, B, C, D, E = Gl, Gl, Gl, Gl, Gl

  function OuterFnc()
    local A, B, C, D = Up, Up, Up, Up

    local function InnerFnc(A)
      local A = IL
      local B = L
      local _ = C -- Without this, C, like D, would not be an
        -- upvalue of InnerFnc.
      for _, Name in ipairs({"A", "B", "C", "D", "E"}) do
        print(Name, GetVar(Name))
      end
    end

    InnerFnc(OL)
  end

  OuterFnc()
end
