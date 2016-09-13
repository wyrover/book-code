local Mantissa, Exp = math.frexp(math.pi)
io.write("Pi: ", Mantissa, " * 2 ^ ", Exp, " = ",
  math.ldexp(Mantissa, Exp), "\n")
