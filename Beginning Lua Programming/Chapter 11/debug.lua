function Hook(Event, LineNum)
  -- Info for the currently running function (not counting this hook):
  local Info = debug.getinfo(2)
  if Event == "call" then
    io.write("'", Info.name or "???", "' is being called; ")
    if Info.nups == 0 then
      io.write("it has no upvalues.\n")
    else
      io.write("its upvalues are:\n")
      for I = 1, Info.nups do
        local Name, Val = debug.getupvalue(Info.func, I)
        io.write("  ", Name, "\t", tostring(Val), "\n")
      end
    end
  elseif Event == "return" or Event == "tail return" then
    io.write("'", Info.name or "???", "' is being returned from.\n")
  elseif Event == "line" then
    local LclCount = 0

    -- An iterator that returns the names and values of the current
    -- function's local variables:
    local function Locals()
      LclCount = LclCount + 1
      return debug.getlocal(3, LclCount)
    end

    local Lcls = {}
    for LclName, LclVal in Locals do
      -- Ignore temporary locals used by the Lua virtual machine:
      if string.sub(LclName, 1, 1) ~= "(" then
        Lcls[#Lcls + 1] = LclName .. " (" .. tostring(LclVal) .. ")"
      end
    end
    io.write("line ", LineNum, "; ")
    if #Lcls > 0 then
      -- This doesn't include upvalues:
      io.write("in-scope local(s): ", table.concat(Lcls, ", "),
        "\n")
    else
      io.write("no in-scope locals.\n")
    end
  end
end

local Fruit1, Fruit2 = "banana", "kiwi"

function FncB()
  return Fruit1 .. "/" .. Fruit2 -- Line 48.
end

function FncA(Fruit)
  local Fruits = FncB() -- Line 52.
  return Fruit .. "/" .. Fruits -- Line 53.
end

debug.sethook(Hook, "crl")
print("### " .. FncA("apple")) -- Line 57.
debug.sethook(nil) -- Line 58.
