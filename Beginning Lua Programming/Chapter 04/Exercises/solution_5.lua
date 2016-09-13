-- Like pairs, but iterates in order by key:
function SortedPairs(Tbl)
  local Sorted = {} -- A (soon to be) sorted array of Tbl's keys.
  for Key in pairs(Tbl) do
    Sorted[#Sorted + 1] = Key -- Same as table.insert.
  end
  table.sort(Sorted, CompAll)
  local I = 0

  -- The iterator itself:
  return function()
    I = I + 1
    local Key = Sorted[I]
    return Key, Tbl[Key]
  end
end
