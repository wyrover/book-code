--[[ This file contains some of the functions defined in
Chapter 3.  They can be individually cut-and-pasted into an
interpreter session, or you can run the whole file and
access its functions from the interpreter by using (from the
shell):

  lua -i various.lua

or (from within the interpreter):

  dofile("various.lua")

Both these techniques are explained in Chapter 3. ]]

-- Prints a greeting:
function Greet(Name)
  print("Hello, " .. Name .. ".")
end

-- A different function that prints a greeting (this
-- overwrites the above Greet function):
function Greet(Name)
  if Name == "Joe" then
    MsgStr = "Whaddya know, Joe?"
  else
    MsgStr = "Hello, " .. Name .. "."
  end
  print(MsgStr)
end

-- Returns the average of Num1 and Num2:
function Average(Num1, Num2)
  return (Num1 + Num2) / 2
end

-- Returns true if Str has an even number of characters;
-- otherwise returns false:
function EvenLen(Str)
  if #Str % 2 == 0 then
    return true
  else
    return false
  end
  print("This will never get printed!")
end

-- Does nothing (and returns nothing):
function DoNothing()
end

-- Returns its three arguments:
function ReturnArgs(Arg1, Arg2, Arg3)
  return Arg1, Arg2, Arg3
end

-- Variables for ScopeTest:
Arg, NotShadowed = "Outside 1", "Outside 2"

-- Prints its one argument and the value of NotShadowed:
function ScopeTest(Arg)
  print(Arg, NotShadowed)
end

-- Nested local scopes:
function ScopeTest2(LclA)
  print("LclA is " .. LclA)
  for LclB = 1, 5 do
    print("LclB is " .. LclB)
    LclA = LclA + 1
  end
  print("LclA is now " .. LclA)
end

-- The local keyword:
function ScopeTest3(Lcl)
  for I = 1, 5 do
    Lcl = Lcl .. "a"
    print(Lcl)
    local Lcl = ""
    Lcl = Lcl .. "z"
    print(Lcl)
  end
  print("The loop is done.")
  print(Lcl)
end

-- Returns true if Str has an even number of characters;
-- otherwise returns false.  (This is the structured version
-- of the same-named function given earlier.)
function EvenLen(Str)
  local Ret
  if #Str % 2 == 0 then
    Ret = true
  else
    Ret = false
  end
  return Ret
end

-- Returns Val's length, or false if Val isn't a string:
function Len(Val)
  return type(Val) == "string" and #Val
end

-- Returns the factorial of N (recursively):
function Fact(N)
  local Ret
  if N == 0 then
    -- Base case:
    Ret = 1
  else
    -- Recursive case:
    Ret = N * Fact(N - 1)
  end
  return Ret
end

-- Returns the factorial of N (iteratively):
function Fact(N)
  local Ret = 1
  for I = 1, N do
    Ret = Ret * I
  end
  return Ret
end

-- Returns the factorial of N (tail-recursively).  Calls
-- itself with two arguments, but when you call it, you need
-- supply only one argument (like the other Fact functions).
function Fact(N, Acc)
  -- Initialize the accumulator to 1:
  Acc = Acc or 1
  if N == 0 then
    -- Base case:
    return Acc
  else
    -- Recursive case:
    return Fact(N - 1, N * Acc)
  end
end

-- Give the print function another name, so it'll still be
-- accessible:
RealPrint = print

-- Prints a message and its one argument:
function FakePrint(Val)
  RealPrint("Inside FakePrint:", Val)
end

-- Returns a do-nothing function:
function MakeDoNothing()
  return function() end
end

-- Returns a function that tests whether a number is
-- less than N:
function MakeLessThan(N)
  return function(X)
    return X < N
  end
end

-- Returns two functions: a function that gets N's value,
-- and a function that increments N by its argument.
function MakeGetAndInc(N)
  -- Returns N:
  local function Get()
    return N
  end

  -- Increments N by M:
  local function Inc(M)
    N = N + M
  end

  return Get, Inc
end
