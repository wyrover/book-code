-- address.lua

screen.clear()
local DbStr = "db:/AddressDB"
local ErrStr
local Hnd, Count = io.open(DbStr, "rb")
local List = {}
if Hnd then
  if Count > 0 then
    for J = 0, Count - 1 do
      local RecSize = Hnd:openrec(J)
      if RecSize then
        if RecSize > 0 then
          local RecStr = Hnd:read("*a")
          if RecStr then
            local Rec = bin.unpack("BBBBBBBBBSSS", RecStr)
            table.insert(List, (Rec[11] or "") .. " " .. (Rec[10] or "") ..
              " " .. (Rec[12] or ""))
          end
        end
        Hnd:closerec()
      end
    end
  else
    ErrStr = "The address book contains no records"
  end
  Hnd:close()
else
  ErrStr = "Error opening the address book"
end
if not ErrStr then
  if next(List) then -- At least one item?
    gui.title("Address")
    gui.list(10, 30, List)
    gui.nl()
    local CtlOk = gui.button("OK")
    local Evt, Id
    repeat
      Evt, Id = gui.event()
    until Evt == appStop or (Evt == ctlSelect and Id == CtlOk)
  end
else
  print(ErrStr)
  repeat until gui.event() == penDown
end
