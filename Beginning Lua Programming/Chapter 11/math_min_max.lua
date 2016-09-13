local function Range(...)
  io.write("Count: ", select("#", ...), ", Min: ", math.min(...),
    ", Max: ", math.max(...), "\n")
end

Range(3, 1, 72, 5.4, math.pi)
