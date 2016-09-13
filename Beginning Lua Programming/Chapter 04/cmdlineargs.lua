-- This script lists (by number) all arguments given to it
-- on the command line.

local Count = select("#", ...)
if Count > 0 then
  print("Command-line arguments:")
  for I = 1, Count do
    print(I, (select(I, ...))) -- The parentheses adjust
      -- select to one value.
  end
else
  print("No command-line arguments given.")
end
