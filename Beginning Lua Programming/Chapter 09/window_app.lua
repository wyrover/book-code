local Window = require("window")

-- Sample window message handler.

local function SampleWindow(Wnd, Msg)
  Wnd.Serial = (Wnd.Serial or 0) + 1
  io.write("Window ", Wnd.Id, ", message ", Msg, ", serial ", Wnd.Serial, "\n")
  if Msg == "ok" or Msg == "cancel" then
    io.write("Calling Window.Close on ", Wnd.Id, "\n")
    Window.Close(Msg)
    io.write("Called Window.Close on ", Wnd.Id, "\n")
  elseif Msg == "button" or Msg == "new" then
    local Time = os.date("%X")
    io.write("Calling Window.Show from ", Wnd.Id, " (", Time, ")\n")
    local Status = Window.Show(SampleWindow)
    io.write("Called Window.Show from ", Wnd.Id, ", child returned ",
      Status, " (", Time, ")\n")
  end
end

-- Main statements

io.write("Application: starting\n")
local Status = Window.Show(SampleWindow)
io.write("Window returned ", Status, "\n")
io.write("Application: ending\n")
