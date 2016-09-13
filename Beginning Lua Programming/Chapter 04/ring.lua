-- A ring data structure:

-- Returns X mod Y, but one-based: the return value will
-- never be less than 1 or greater than Y.  (Y is assumed to
-- be positive.)
local function OneMod(X, Y)
  return (X - 1) % Y + 1
end

-- A table in which to create the methods:
local Methods = {}

-- Inserts a new element into self:
function Methods:Push(Elem)
  table.insert(self, self.Pos, Elem)
end

-- Removes the current element from self; returns nil if
-- self is empty:
function Methods:Pop()
  local Ret
  if #self > 0 then
    Ret = table.remove(self, self.Pos)
    -- Keep self.Pos from pointing outside the array by
    -- wrapping it around:
    if self.Pos > #self then
      self.Pos = 1
    end
  end
  return Ret
end

-- Rotates self to the left:
function Methods:RotateL()
  if #self > 0 then
    self.Pos = OneMod(self.Pos + 1, #self)
  end
end

-- Rotates self to the right:
function Methods:RotateR()
  if #self > 0 then
    self.Pos = OneMod(self.Pos - 1, #self)
  end
end

-- Returns the ring's size:
function Methods:Size()
  return #self
end

-- Returns a string representation of self:
function Methods:ToString()
  -- Convert the parts of self to the left and to the right
  -- of self.Pos to strings:
  local LeftPart = table.concat(self, ", ", 1, self.Pos - 1)
  local RightPart = table.concat(self, ", ", self.Pos, #self)
  -- Only put a separator between them if neither is the
  -- empty string:
  local Sep
  if LeftPart == "" or RightPart == "" then
    Sep = ""
  else
    Sep = ", "
  end
  -- RightPart's first element is self.Pos, so put it first:
  return RightPart .. Sep .. LeftPart
end

-- Returns an iterator that iterates through all self's
-- elements.  (This is added later in the chapter.)
function Methods:Elems()
  local IterPos -- The position of the element the iterator
    -- needs to return

  return function()
    local Ret
    if IterPos then
      if IterPos ~= self.Pos then
        Ret = self[IterPos]
      else
        -- Back at the beginning; do nothing (which ends the
        -- loop by returning nil).
      end
    else
      -- At the beginning: initialize IterPos:
      IterPos = self.Pos
      Ret = self[IterPos] -- If the ring is empty, this'll
        -- end the loop by returning nil.
    end
    IterPos = OneMod(IterPos + 1, #self)
    return Ret
  end
end

-- Instantiates a ring:
function MakeRing(Ring)
  -- Make an empty ring if an array of initial ring values
  -- wasn't passed in:
  Ring = Ring or {}
  -- Ring.Pos is the position of the current element of the
  -- ring; initialize it to 1 (all methods that expect
  -- there to be a current element first make sure the ring
  -- isn't empty):
  Ring.Pos = 1
  -- Give the ring methods and return it:
  for Name, Fnc in pairs(Methods) do
    Ring[Name] = Fnc
  end
  return Ring
end
