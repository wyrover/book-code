local function FncList(Tbl, Name)
  local List = {}
  for Key, Val in pairs(Tbl) do
    if type(Val) == "function" then
      List[#List + 1] = Key
    end
  end
  table.sort(List)
  io.write('<h1>', Name, ' library</h1>\n\n<p>')
  for J, Str in ipairs(List) do
    io.write(Str, " ")
  end
  io.write("</p>\n\n")
end

io.write('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"\n',
  '"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">\n\n',
  '<html>\n\n<head>\n\n<title>Lua Environment</title>',
  '\n\n</head>\n\n<body>\n\n')

local Namespace = {"coroutine", "debug", "io", "math", "os", "package",
  "string", "table"}

FncList(_G, "base")
for J, Tbl in ipairs(Namespace) do
  FncList(_G[Tbl], Tbl)
end
io.write('</body>\n\n</html>\n')
