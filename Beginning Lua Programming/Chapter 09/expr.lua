local Token, Pos

local function LclToken(ExpStr)
  local Len, PosStart, PosEnd, Allow
  Allow = "factor"
  Len = string.len(ExpStr)
  Pos = 1
  while Pos <= Len do
    if Allow == "factor" then
      PosStart, PosEnd, Token = string.find(ExpStr,
        "^%s*([%+%-]?%d+%.?%d*)%s*", Pos)
      if PosStart then
        Token = tonumber(Token)
      else
        PosStart, PosEnd, Token = string.find(ExpStr,
          "^%s*(%()%s*", Pos)
        if not PosStart then
          error("expected number or '(' at position " .. Pos)
        end
      end
    else -- "op"
      PosStart, PosEnd, Token = string.find(ExpStr,
        "^%s*([%)%+%-%*%/])%s*", Pos)
      if not PosStart then
        error("expected operator at position " .. Pos)
      end
    end
    Allow = coroutine.yield()
    Pos = PosEnd + 1
  end
  Token = "end"
  coroutine.yield()
  error("end of expression overreached")
end

local LclExpression

local function LclFactor()
  local Val = 0
  if type(Token) == "number" then
    Val = Token
    LclToken("op")
  elseif Token == "(" then
    LclToken("factor")
    Val = LclExpression()
    if Token == ")" then
      LclToken("op")
    else
      error("missing ')' at position " .. Pos)
    end
  else
    error("expecting number or '(' at position " .. Pos)
  end
  return Val
end

local function LclTerm()
  local Val = LclFactor()
  while Token == '*' or Token == '/' do
    if Token == '*' then
      LclToken("factor")
      Val = Val * LclFactor()
    else
      LclToken("factor")
      Val = Val / LclFactor()
    end
  end
  return Val
end

function LclExpression()
  local Val = LclTerm()
  while Token == '+' or Token == '-' do
    if Token == '+' then
      LclToken("factor")
      Val = Val + LclTerm()
    else
      LclToken("factor")
      Val = Val - LclTerm()
    end
  end
  return Val
end

local function LclEvaluate(ExpStr)
  LclToken = coroutine.wrap(LclToken)
  LclToken(ExpStr)
  local Val = LclExpression()
  if Token ~= "end" then
    error("unexpected token at position " .. Pos)
  end
  return Val
end

-- Evaluates the specified expression. If successful, returns result of
-- expression. If the expression can't be evaluated, (nil, error message, error
-- position) is returned.

function Evaluate(ExpStr)
  local ErrPos
  local Code, Val = pcall(LclEvaluate, ExpStr)
  if Code then
    Code, Val = Val, nil
  else
    Code, ErrPos = nil, Pos
  end
  return Code, Val, ErrPos
end

local ExpStr = arg[1] or "1 + 1"
local Result, ErrStr, ErrPos = Evaluate(ExpStr)
io.write("Expression: ", ExpStr, "\n")
if Result then
  io.write("Result: ", Result, "\n")
else
  io.write(string.rep(' ', ErrPos + 11), "^\n")
  io.write("Error: ", ErrStr, "\n")
end
