function CircleArea(Radius)
  return math.pi * Radius ^ 2
end

Radius = 5
io.write("radius: ", Radius, " feet; area: ",
  CircleArea(Radius), " square feet\n")
