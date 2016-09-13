local function Rational(Num, Den)
  local Whole, Frac = math.modf(Num / Den)
  io.write(Num, " / ", Den, " = ", Whole, " + ", Frac, "\n")
  io.write(Num, " / ", Den, " = ", Whole, " + ",
    math.fmod(Num, Den), " / ", Den, "\n")
end

Rational(5, 3)
Rational(-5, 3)
Rational(5.2, math.pi)
