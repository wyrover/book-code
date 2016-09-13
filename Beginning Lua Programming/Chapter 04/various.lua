--[[ This file contains some of the tables and functions
defined in Chapter 4.  They can be individually
cut-and-pasted into an interpreter session, or you can run
the whole file and access them from the interpreter by using
(from the shell):

  lua -i various.lua

or (from within the interpreter):

  dofile("various.lua")

Both these techniques are explained in this chapter. ]]

-- Two ways of writing the NameToInstr table:
NameToInstr = {["John"] = "rhythm guitar",
  ["Paul"] = "bass guitar",
  ["George"] = "lead guitar",
  ["Ringo"] = "drumkit"}
NameToInstr = {John = "rhythm guitar",
  Paul = "bass guitar",
  George = "lead guitar",
  Ringo = "drumkit"}

-- Two ways of writing the Months table:
Months = {[1] = "January", [2] = "February", [3] = "March",
  [4] = "April", [5] = "May", [6] = "June", [7] = "July",
  [8] = "August", [9] = "September", [10] = "October",
  [11] = "November", [12] = "December"}
Months = {"January", "February", "March", "April", "May",
  "June", "July", "August", "September", "October",
  "November", "December"}

-- Returns nothing:
function ReturnNothing()
end

-- Returns three (arbitrary) values:
function ReturnThreeVals()
  return "x", "y", "z"
end

-- Considered as an array, this would have gaps, but it's
-- obviously intended as an associative table:
AustenEvents = {[1775] = "born",
  [1811] = "Sense and Sensibility published",
  [1813] = "Pride and Prejudice published",
  [1814] = "Mansfield Park published",
  [1816] = "Emma published",
  [1817] = "died",
  [1818] = "Northanger Abbey and Persuasion published"}

-- Same info as NameToInstr, but ordered:
NamesAndInstrs = {
  {Name = "John", Instr = "rhythm guitar"},
  {Name = "Paul", Instr = "bass guitar"},
  {Name = "George", Instr = "lead guitar"},
  {Name = "Ringo", Instr = "drumkit"}}

-- Names of Baroque composers:
Names = {"Scarlatti", "Telemann", "Corelli", "Purcell",
  "Vivaldi", "Handel", "Bach"}

-- Returns a string showing an array's elements separated by
-- commas (and spaces):
function CommaSeparate(Arr)
  return table.concat(Arr, ", ")
end

-- Returns a table of two functions: a function that gets
-- N's value, and a function that increments N by its
-- argument.
function MakeGetAndInc(N)
  -- Returns N:
  local function Get()
    return N
  end

  -- Increments N by M:
  local function Inc(M)
    N = N + M
  end

  return {Get = Get, Inc = Inc}
end

-- The following version of MakeGetAndInc returns an object
-- that can be used with the colon syntax: "A:Get()"
do -- Local scope for Get and Inc.
  -- Returns Obj.N:
  local function Get(Obj)
    return Obj.N
  end

  -- Increments Obj.N by M:
  local function Inc(Obj, M)
    Obj.N = Obj.N + M
  end

  -- Creates an object:
  function MakeGetAndInc(N)
    return {N = N, Get = Get, Inc = Inc}
  end
end

-- The following version of MakeGetAndInc returns an object
-- that can be used with the colon syntax, and it uses the
-- colon syntax to define its methods:
do -- Local scope for T.
  -- A table in which to define Get and Inc:
  local T = {}

  -- Returns self.N:
  function T:Get()
    return self.N
  end

  -- Increments self.N by M:
  function T:Inc(M)
    self.N = self.N + M
  end

  -- Creates an object:
  function MakeGetAndInc(N)
    return {N = N, Get = T.Get, Inc = T.Inc}
  end
end

-- Returns the average of all its arguments:
function Average(...)
  local Ret, Count = 0, 0
  for _, Num in ipairs({...}) do
    Ret = Ret + Num
    Count = Count + 1
  end
  assert(Count > 0, "Attempted to average zero numbers")
  return Ret / Count
end

-- Returns a function that prints MakePrinter's arguments;
-- doesn't handle nils properly:
function MakePrinter(...)
  local Args = {...}
  return function()
    print(unpack(Args))
  end
end

-- Returns a function that prints MakePrinter's arguments;
-- does handle nils properly:
function MakePrinter(...)
  local Args = {...}
  local ArgCount = select("#", ...)
  return function()
    print(unpack(Args, 1, ArgCount))
  end
end

-- A wrapper for table.sort that takes keyword arguments:
function Sort(KeyArgs)
  local Arr = KeyArgs.Arr -- The array to be sorted.
  local CompFnc = KeyArgs.CompFnc -- Comparison function.
    or function(A, B) return A < B end -- Default.
  if KeyArgs.Reverse then
    -- Reverse the sense of the comparison function:
    local OrigCompFnc = CompFnc
    CompFnc = function(A, B)
      return OrigCompFnc(B, A)
    end
  end
  table.sort(Arr, CompFnc)
  return Arr
end

-- Creates a new table:
function CreateTbl()
  return {}
end

-- Makes a shallow copy of a table:
function ShallowCopy(Src)
  local Dest = {}
  for Key, Val in pairs(Src) do
    Dest[Key] = Val
  end
  return Dest
end

-- Makes a deep copy of a table.  Doesn't properly handle
-- duplicate subtables.
function DeepCopy(Src)
  local Dest = {}
  for Key, Val in pairs(Src) do
    Key = type(Key) == "table" and DeepCopy(Key) or Key
    Val = type(Val) == "table" and DeepCopy(Val) or Val
    Dest[Key] = Val
  end
  return Dest
end

-- Makes a deep copy of a table.  This version of DeepCopy
-- properly handles duplicate subtables, including cycles.
-- (The Seen argument is only for recursive calls.)
function DeepCopy(Src, Seen)
  local Dest
  if Seen then
    -- This will only set Dest if Src has been seen before:
    Dest = Seen[Src]
  else
    -- Top-level call; create the Seen table:
    Seen = {}
  end
  -- If Src is new, copy it into Dest:
  if not Dest then
    -- Make a fresh table and record it as seen:
    Dest = {}
    Seen[Src] = Dest
    for Key, Val in pairs(Src) do
      Key = type(Key) == "table" and DeepCopy(Key, Seen) or Key
      Val = type(Val) == "table" and DeepCopy(Val, Seen) or Val
      Dest[Key] = Val
    end
  end
  return Dest
end

-- The table illustrated in Figure 4-7:
Figure4_7 = {Person = {
    Living = {"Roberto Ierusalimschy", "Gary Larson"},
    Dead = {"Jane Austen", "Archimedes"}},
  Place = {"Rio de Janeiro", "The North Pole"}}

-- An iterator factory -- like ipairs, but goes through the
-- array in reverse order:
function ReverseIpairs(Arr)
  local I = #Arr

  local function Iter()
    local Ret1, Ret2
    if I > 0 then
      Ret1, Ret2 = I, Arr[I]
      I = I - 1
    end
    return Ret1, Ret2
  end

  return Iter
end

-- Stateless version of ReverseIpairs:
do -- Local scope for Iter.
  -- ReverseIpairs's iterator; Arr is the "invariant state",
  -- and I is the control variable's previous value:
  local function Iter(Arr, I)
    if I > 1 then
      I = I - 1
      return I, Arr[I] -- Violates structured programming
        -- (not a severe misdeed in such a small function).
    end
  end

  -- An iterator factory -- like ipairs, but goes through
  -- the array in reverse order:
  function ReverseIpairs(Arr)
    return Iter, Arr, #Arr + 1
  end
end

-- A workalike for the iterator returned by ipairs:
function IpairsIter(Arr, PrevI)
  local CurI = PrevI + 1
  Val = Arr[CurI]
  if Val ~= nil then
    return CurI, Val
  end
end

-- Returns an iterator that goes through all Len-long
-- subsequences of Arr:
function Subseqs(Arr, Len)
  local Pos = 0

  return function()
    Pos = Pos + 1
    if Pos + Len - 1 <= #Arr then
      return unpack(Arr, Pos, Pos + Len - 1)
    end
  end
end

-- Prints a greeting:
function Greet(Name)
  print("Hello, " .. Name .. ".")
end

-- Gives its caller an empty environment:
function PwnMe()
  setfenv(2, {})
end

-- Returns a do-nothing function:
function MakeDoNothing()
  return function() end
end
