-- Rotates self N elements to the left:
function Methods:RotateL(N)
  N = N or 1
  if #self > 0 then
    self.Pos = OneMod(self.Pos + N, #self)
  end
end
