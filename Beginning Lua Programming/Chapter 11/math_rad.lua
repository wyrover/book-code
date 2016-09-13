math._sin = math.sin

-- Redefine math.sin to accept angle in degrees

math.sin = function(degrees)
  return math._sin(math.rad(degrees))
end

io.write("  x  sin(x)\n")
io.write("---  ------\n")
for Deg = 0, 360, 90 do
  io.write(string.format("%3d  %6.2f\n", Deg, math.sin(Deg)))
end
