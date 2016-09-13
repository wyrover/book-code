FileStr = os.tmpname()
Hnd = io.open(FileStr, "w")
if Hnd then
  io.write("Opened temporary file ", FileStr, " for writing\n")
  Hnd:write("Line 1\nLine 2\nLine 3\n")
  Hnd:close()
  for Str in io.lines(FileStr) do
    io.write(Str, "\n")
  end
  os.remove(FileStr)
else
  io.write("Error opening ", FileStr, " for writing\n")
end
