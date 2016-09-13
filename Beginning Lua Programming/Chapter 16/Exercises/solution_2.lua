local function LclBodyPrint(PopHnd, Pos)
  local Str, Tbl
  Str, Tbl = LclTransact(PopHnd, "retr " .. Pos, true)
  if Str then
    io.write(Str, "\n")
    local Show
    for K, Str in ipairs(Tbl) do
      if Show then
        io.write(Str, "\n")
      elseif Str == "" then
        Show = true
      end
    end
  else
    io.write('Error: ', Tbl, "\n") -- Tbl is error message
  end
end
