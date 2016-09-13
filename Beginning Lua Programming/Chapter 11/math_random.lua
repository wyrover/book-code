local Seed = os.clock()
for J = 1, 2 do
  math.randomseed(Seed)
  math.random() -- On some systems that shall remain nameless, the
    -- first call to math.random is easy enough to predict that it
    -- destroys the illusion of randomness; throw it away.
  for J = 1, 10 do
    io.write(math.random(100, 200), " ")
  end
  io.write("\n")
end
