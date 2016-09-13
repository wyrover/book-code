Fnc, ErrStr = package.loadlib("./luabogo.so", "luaopen_bogo")
if Fnc then
  Fnc("bogo")
else
  io.write(ErrStr, "\n")
end
