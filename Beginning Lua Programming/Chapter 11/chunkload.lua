local FileStr = "test.lua"

for J = 1, 2 do
  local LuaStr
  if J == 1 then
    io.write("=== Source chunk ===\n")
    LuaStr = "print('source', ...)"
  else
    io.write("=== Compiled chunk ===\n")
    LuaStr = string.dump(function(...) print('compiled', ...) end)
  end
  local Hnd = io.open(FileStr, "wb")
  if Hnd then
    Hnd:write(LuaStr)
    Hnd:close()
    io.write("[dofile]     ")
    dofile(FileStr)
    io.write("[loadfile]   ")
    local Fnc, ErrStr = loadfile(FileStr)
    if Fnc then
      Fnc(1, 2, 3)
      io.write("[loadstring] ")
      Fnc, ErrStr = loadstring(LuaStr)
      if Fnc then
        Fnc(1, 2, 3)
        io.write("[load]       ")
        local RetStr = LuaStr
        local function Loader()
          local Ret = RetStr
          RetStr = nil
          return Ret
        end
        Fnc, ErrStr = load(Loader)
        if Fnc then
          Fnc(1, 2, 3)
        else
          io.write(ErrStr, "\n")
        end
      else
        io.write(ErrStr, "\n")
      end
    else
      io.write(ErrStr, "\n")
    end
  else
    io.write("Error opening ", FileStr, " for writing\n")
  end
end
os.remove(FileStr)
