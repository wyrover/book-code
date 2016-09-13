FileStr = "test.txt"
Hnd, ErrStr = io.open(FileStr, "w")
if Hnd then
  Hnd:setvbuf("full", 1024)
  for K = 1, 4 do
    if K == 3 then
      Pos = Hnd:seek("cur", 0)
      Hnd:write("Special line\n")
    end
    Hnd:write(string.format("Line %02d\n", K))
  end
  Hnd:flush()
  Hnd:close()
  Hnd, ErrStr = io.open(FileStr, "r")
  if Hnd then
    for Str in Hnd:lines() do
      io.write(Str, "\n")
    end
    Hnd:seek("set", Pos)
    Str = Hnd:read("*l")
    io.write(Str, "\n")
    Hnd:close()
  else
    io.write(ErrStr)
  end
else
  io.write(ErrStr)
end
