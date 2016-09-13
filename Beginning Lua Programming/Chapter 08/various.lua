-- This file contains some of the functions and metatables
-- defined in Chapter 8.  They can be individually
-- cut-and-pasted into an interpreter session, or you can
-- run the whole file and access them from the interpreter
-- by using lua's -i option or the dofile function.

do
  local SpliceMeta -- SpliceMeta needs to be in
    -- SpliceMeta.__concat's scope.
  SpliceMeta = {
    -- Makes a new array by splicing two arrays together:
    __concat = function(ArrA, ArrB)
      assert(type(ArrA) == "table" and type(ArrB) == "table")
      local Ret = setmetatable({}, SpliceMeta)
      for I, Elem in ipairs(ArrA) do
        Ret[I] = Elem
      end
      local LenA = #ArrA
      for I, Elem in ipairs(ArrB) do
        Ret[LenA + I] = Elem
      end
      return Ret
    end}

  -- Takes an array, returns that array after giving it a
  -- metamethod that makes it "spliceable" with the
  -- concatenation operator:
  function MakeSpliceArr(Arr)
    return setmetatable(Arr, SpliceMeta)
  end
end

do
  -- Returns Val after (if it's a string) lowercasing it:
  local function Lower(Val)
    if type(Val) == "string" then
      Val = string.lower(Val)
    end
    return Val
  end

  local Meta = {
    __newindex = function(Tbl, Key, Val)
      rawset(Tbl, Lower(Key), Val)
    end,
    __index = function(Tbl, Key)
      return rawget(Tbl, Lower(Key))
    end}

  -- Returns a new table with a metatable that makes its
  -- keys case-insensitive:
  function MakeIgnoreCase()
    return setmetatable({}, Meta)
  end
end

CallMeta = {
  -- Are all of the arguments true-valued keys in Tbl?
  __call = function(Tbl, ...)
    for I = 1, select("#", ...) do
      if not Tbl[select(I, ...)] then
        return false -- NON-STRUCTURED EXIT: FOUND A KEY
          -- WITH A NON-TRUE VALUE.
      end
    end
    return true
  end}
