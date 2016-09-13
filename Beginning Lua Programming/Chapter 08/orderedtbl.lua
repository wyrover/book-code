-- "Ordered tables" -- they remember the order in which
-- things are put into them.

local RealTbls = {} -- Keys: proxy tables; values: the real
  -- tables.
local NumToKeys = {} -- Keys: proxy tables; values: arrays
  -- of real tables' keys, in order.
local KeyToNums = {} -- Keys: proxy tables; values: tables
  -- mapping real tables' keys to their (order) numbers.
-- Prevent leakage by giving all of the above tables weak
-- keys (this will be explained in Chapter 10):
local ModeMeta = {__mode = "k"}
setmetatable(RealTbls, ModeMeta)
setmetatable(NumToKeys, ModeMeta)
setmetatable(KeyToNums, ModeMeta)

-- Does the bookkeeping necessary to add a key and its order
-- to the real table (or update/delete a key):
local function __newindex(Proxy, Key, Val)
  local RealTbl = RealTbls[Proxy]
  local NumToKey = NumToKeys[Proxy]
  local KeyToNum = KeyToNums[Proxy]
  if RealTbl[Key] == nil then
    -- This is a new key.  Only add it if the value's
    -- non-nil:
    if Val ~= nil then
      -- Record the value:
      RealTbl[Key] = Val
      -- Record the order:
      NumToKey[#NumToKey + 1] = Key
      KeyToNum[Key] = #NumToKey
    end
  else
    -- This is an already existing key.
    if Val ~= nil then
      -- Record the new value:
      RealTbl[Key] = Val
    else
      -- Delete it:
      RealTbl[Key] = nil
      local Num = KeyToNum[Key]
      KeyToNum[Key] = nil
      -- table.remove will shift down anything in NumToKey
      -- that's higher than Num, but it needs to be done by
      -- hand for KeyToNum:
      if Num < #NumToKey then
        for SomeKey, SomeNum in pairs(KeyToNum) do
          if SomeNum > Num then
            KeyToNum[SomeKey] = SomeNum - 1
          end
        end
      end
      table.remove(NumToKey, Num)
    end
  end
end

-- Returns Key's value in the real table:
local function __index(Proxy, Key)
  return RealTbls[Proxy][Key]
end

-- An iterator that iterates through all the real table's
-- key-value pairs in the correct order:
local function __next(Proxy, PrevKey)
  assert(type(Proxy) == "table", "bad argument to 'next'")
  local RealTbl = RealTbls[Proxy]
  local NumToKey = NumToKeys[Proxy]
  local KeyToNum = KeyToNums[Proxy]
  -- This will be 0 only if PrevKey is the seed value nil:
  local PrevNum = KeyToNum[PrevKey] or 0
  local Key = NumToKey[PrevNum + 1]
  return Key, RealTbl[Key]
end

local RealIpairs = ipairs

-- Returns an ipairs iterator for the real table:
local function __ipairs(Proxy)
  return RealIpairs(RealTbls[Proxy])
end

-- The metatable:
local OrderMeta = {__newindex = __newindex, __index = __index,
  __next = __next, __ipairs = __ipairs}

local RealNext = next

-- A metatable-aware replacement for next:
function next(Tbl, Key)
  local Meta = getmetatable(Tbl)
  if Meta and Meta.__next then
    return Meta.__next(Tbl, Key)
  else
    return RealNext(Tbl, Key)
  end
end

-- A metatable-aware replacement for pairs:
function pairs(Tbl)
  -- The real pairs only needs to be replaced because it
  -- returns its own copy of next unaffected by our
  -- replacement.
  assert(type(Tbl) == "table", "bad argument to 'pairs'")
  return next, Tbl, nil
end

-- A metatable-aware replacement for ipairs:
function ipairs(Tbl)
  local Meta = getmetatable(Tbl)
  if Meta and Meta.__ipairs then
    return Meta.__ipairs(Tbl)
  else
    return RealIpairs(Tbl)
  end
end

-- Returns a table that remembers the order in which keys
-- are added to it:
function MakeOrderedTbl()
  local RealTbl, Proxy = {}, {}
  RealTbls[Proxy] = RealTbl
  -- The following two tables are two complementary ways of
  -- recording the order that keys are added in:
  NumToKeys[Proxy] = {}
  KeyToNums[Proxy] = {}
  return setmetatable(Proxy, OrderMeta)
end
