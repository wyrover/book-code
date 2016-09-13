-- This script outputs all unique lines of a file, sorted.
-- It does no error checking!
--
-- Usage:
--
--   lua sortuniq.lua INFILE OUTFILE
--
-- If OUTFILE is not given, standard output will be used.  If
-- no arguments are given, standard input and standard output
-- will be used.

-- Like pairs, but loops in order by key.  (Unlike the
-- version in Chapter 4, this only handles all-string or
-- all-numeric keys.)
function SortedPairs(Tbl)
  local Sorted = {} -- A (soon to be) sorted array of Tbl's keys.
  for Key in pairs(Tbl) do
    Sorted[#Sorted + 1] = Key
  end
  table.sort(Sorted)
  local I = 0

  -- The iterator itself:
  return function()
    I = I + 1
    local Key = Sorted[I]
    return Key, Tbl[Key]
  end
end

function Main(InFilename, OutFilename)
  -- Make the lines of the input file (standard input if no
  -- name was given) keys of a table:
  local Lines = {}
  local Iter = InFilename and io.lines(InFilename) or io.lines()
  for Line in Iter do
    Lines[Line] = true
  end
  -- Get a handle to the output file (standard output if no
  -- name was given):
  local OutHnd = OutFilename
    and io.open(OutFilename, "w")
    or io.stdout
  -- Write each line in Lines to the output file, in order:
  for Line in SortedPairs(Lines) do
    OutHnd:write(Line, "\n")
  end
  OutHnd:close()
end

Main(...)
