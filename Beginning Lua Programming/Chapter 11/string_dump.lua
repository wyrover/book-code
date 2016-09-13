Str = string.dump(function(...) print(...) end)
print(string.gsub(Str, "%A+", "..."))
loadstring(Str)(1, 2, 3)
