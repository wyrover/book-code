local Str = "/var/local/www"
local PosStart, PosEnd, MatchStr = 1
while PosStart do
  PosStart, PosEnd, MatchStr = string.find(Str, "(%w+)", PosStart)
  if PosStart then
    io.write(MatchStr, "\n")
    PosStart = PosEnd + 1
  end
end
