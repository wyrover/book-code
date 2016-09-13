Tbl = {"This is line 4", "This line is out of place", "This is line 2"}
table.insert(Tbl, "This is line 1")
table.remove(Tbl, 2)
table.insert(Tbl, "This is line 3")
table.insert(Tbl, "This is line " .. table.maxn(Tbl) + 1)
table.sort(Tbl)
io.write(table.concat(Tbl, "\n"), "\n")
