-- lua_expr.lua

screen.clear()
gui.title("Lua expression")
local X, Y
local Wd, Ht = screen.mode()
gui.label("Lua expression")
gui.nl()
local CtlExpr = gui.control{type = "field", lines = 1, columns = 24,
  limit = 256, editable = true, underlined = true}
local CtlGo = gui.control{type = "button", text = "Go"}
gui.nl()
gui.label("Result")
gui.nl()
local CtlResult = gui.control{type = "field", lines = 1, columns = 28,
  limit = 256, editable = false, underlined = false}
gui.setfocus(CtlExpr)
local Loop = true
while Loop do
  local Evt, Id = gui.event()
  if Evt == appStop then
    Loop = false
  elseif Evt == ctlSelect then
    if Id == CtlGo then
      local Fnc
      local Str = gui.gettext(CtlExpr)
      if string.len(Str) > 0 then
        Fnc, Str = loadstring("return tostring(" .. Str .. ")")
        if Fnc then
          local Status
          Status, Str = pcall(Fnc)
        end
        gui.settext(CtlResult, tostring(Str))
      end
    end
  end
end
