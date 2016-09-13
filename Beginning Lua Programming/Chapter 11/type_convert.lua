BinStr = "101010"
Num = tonumber(BinStr, 2)
DecStr = tostring(Num)
io.write(BinStr, " (", type(BinStr), "), ", Num, " (", type(Num), "), ",
  DecStr, " (", type(DecStr), ")\n")
