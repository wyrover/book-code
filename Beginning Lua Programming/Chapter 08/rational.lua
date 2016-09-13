-- A basic rational number implementation.

-- Returns A and B's greatest common divisor:
local function GetGcd(A, B)
  local Remainder = A % B
  return Remainder == 0 and B or GetGcd(B, Remainder)
end

-- Metamethods:

local function Add(A, B)
  return MakeRat(A.Numer * B.Denom + B.Numer * A.Denom,
    A.Denom * B.Denom)
end

local function Sub(A, B)
  return A + -B
end

local function Mul(A, B)
  return MakeRat(A.Numer * B.Numer, A.Denom * B.Denom)
end

local function Div(A, B)
  assert(B.Numer ~= 0, "Divison by zero")
  return A * MakeRat(B.Denom, B.Numer)
end

local function Unm(A)
  return MakeRat(-A.Numer, A.Denom)
end

local function Eq(A, B)
  -- This and Lt work because MakeRat always makes sure the
  -- denominator is positive and reduces to lowest terms:
  return A.Numer == B.Numer and A.Denom == B.Denom
end

local function Lt(A, B)
  local Diff = A - B
  return Diff.Numer < 0
end

local function ToString(Rat)
  return Rat.Numer .. "/" .. Rat.Denom
end

local RatMeta = {
  __add = Add,
  __sub = Sub,
  __mul = Mul,
  __div = Div,
  __unm = Unm,
  __eq = Eq,
  __lt = Lt,
  __tostring = ToString,
}

-- The three global functions supplied by this library:

-- Instantiates a rational number:
function MakeRat(Numer, Denom)
  Numer, Denom = tonumber(Numer), tonumber(Denom)
  assert(Denom ~= 0, "Denominator must be nonzero")
  assert(Numer == math.floor(Numer) and Denom == math.floor(Denom),
    "Numerator and denominator must be integers")
  -- Make sure the denominator is positive:
  if Denom < 0 then
    Numer, Denom = -Numer, -Denom
  end
  -- Reduce the fraction to its lowest terms:
  local Gcd = GetGcd(Numer, Denom)
  local Rat = {
    Numer = Numer / Gcd,
    Denom = Denom / Gcd}
  return setmetatable(Rat, RatMeta)
end

-- Instantiates a rational number from a string of the form
-- "numerator/denominator":
function r(Str)
  local Numer, Denom = string.match(Str, "^(%-?%d+)%/(%-?%d+)$")
  assert(Numer, "Couldn't parse rational number")
  return MakeRat(Numer, Denom)
end

-- Converts a rational to a (floating-point) number:
function RatToNumber(Rat)
  return Rat.Numer / Rat.Denom
end
