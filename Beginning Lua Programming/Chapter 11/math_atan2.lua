for Y = -3, 3 do
  for X = -3, 3 do
    io.write(string.format("%5.2f ", math.atan2(Y, X) / math.pi))
  end
  io.write("\n")
end
