io.write("Radians  Degrees\n")
io.write("-------  -------\n")
for Step = 0, 2, 0.25 do
  local Degree = math.deg(math.pi * Step)
  io.write(string.format("%4.2f pi  %7.1f\n", Step, Degree))
end
