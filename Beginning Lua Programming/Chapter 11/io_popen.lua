Hnd, ErrStr = io.popen("lua -h")
if Hnd then
  for Str in Hnd:lines() do
    io.write(Str, "\n")
  end
  Hnd:close()
else
  io.write(ErrStr, "\n")
end
