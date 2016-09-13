io.write("Start of tests\n")
io.write("-- pcall --\n")
local BadLuaStr = "print('Intentional error ahead', ...) error('crash')"
Fnc, ErrStr = loadstring(BadLuaStr, "Demonstration")
if Fnc then
  local Code, ErrStr = pcall(Fnc, "pcall")
  if not CodeErrStr then
    io.write("[pcall error] ", ErrStr, "\n")
  end
  io.write("-- xpcall --\n")
  local Code, ErrStr = xpcall(Fnc, debug.traceback)
  if not CodeErrStr then
    io.write("[xpcall error] ", ErrStr, "\n")
  end
else
  io.write("[loadstring error] ", ErrStr, "\n")
end
io.write("End of tests\n")
