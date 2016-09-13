local function KbUsed(Seq)
  io.write(string.format("%2d %8.2f KB\n", Seq, collectgarbage("count")))
end
KbUsed(1)
Str = string.rep("*", 1e5)
KbUsed(2)
Str = nil
collectgarbage("collect")
KbUsed(3)
collectgarbage("collect")
KbUsed(4)
collectgarbage("collect")
KbUsed(5)
