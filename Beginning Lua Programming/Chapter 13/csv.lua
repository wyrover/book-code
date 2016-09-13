require "csvparse"

-- Return a string which has been properly quoted for inclusion in a
-- comma-separated value file.

function csv.escape(str)
  local wrap = ""
  str = tostring(str)
  if string.find(str, '"') then
    str = string.gsub(str, '"', '""')
    wrap = '"'
  end
  if string.find(str, ',') then
    wrap = '"'
  end
  return wrap .. str .. wrap
end

-- Iterator to allow traversal of CSV cells

function csv.cells(str)
  local pos = 1
  local function nextcell()
    local cellstr
    if pos > 0 then
      cellstr, pos = csv.parse(str, pos)
    else
      cellstr = nil
    end
    return cellstr
  end
  return nextcell
end
