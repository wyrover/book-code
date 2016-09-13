function Recurse(Count, Indent)
  Indent = Indent or ""
  if Count > 0 then
    io.write(Indent, "do\n")
    Recurse(Count - 1, Indent .. " ")
    io.write(Indent, "end\n")
  end
end

Recurse(299)
