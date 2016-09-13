local function FileCopyLineNum(SrcFileStr, DstFileStr)
  local SrcHnd, DstHnd, ErrStr, Line
  SrcHnd, ErrStr = io.open(SrcFileStr, "r")
  if SrcHnd then
    DstHnd, ErrStr = io.open(DstFileStr, "w")
    if DstHnd then
      Line = 0
      for Str in SrcHnd:lines() do
        Line = Line + 1
        DstHnd:write(Line, " ", Str, "\n")
      end
      if Line == 0 then
        ErrStr = SrcFileStr .. ": File is empty"
        Line = nil
      end
      DstHnd:close()
    end
    SrcHnd:close()
  end
  return Line, ErrStr
end

local Count, ErrStr = FileCopyLineNum("index.html", "index.lst")
io.write(Count and ("OK: count " .. Count) or ErrStr, "\n")
