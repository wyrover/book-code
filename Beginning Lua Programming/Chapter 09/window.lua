local Window, Lcl = {}, {}

-- Queue of posted event messages

Lcl.MsgQueue = {}

-- Window structure
--   Parent: window object of parent
--   Id: string identifier, e.g. 1.2.5 for 5th child of 2nd child of window 1
--   Co: coroutine(Fnc) blocks until terminating event
--   ChildCount: number of active children
--   ChildSerial: used for naming new children
--   ChildList: associative array keyed by child window objects
--   Close: assigned the terminating message (usually "cancel" or "ok")
--   User: table passed to handler to hold data that must persist from event to
--     event

-- List of window structures keyed by Id

Lcl.WindowList = {}

-- Currently active window

Lcl.WindowActive = nil

-- Value of message to indicate that associated window should be unblocked

Lcl.MsgReturn = "\000"

-- Display all active windows

function Lcl.Show()
  local List = {}
  for Id, Wnd in pairs(Lcl.WindowList) do
    table.insert(List, Wnd.Close and (Id .. " (pending closure)") or Id)
  end
  table.sort(List)
  for J, Id in ipairs(List) do
    io.write(Id, "\n")
  end
end

-- This function is called when an event occurs that could result in the return
-- of a window call.

function Lcl.Destroy(Wnd)
  if Wnd.Close and Wnd.ChildCount == 0 then
    io.write("Unblocking window ", Wnd.Id, "\n")
    table.insert(Lcl.MsgQueue, {Wnd, Lcl.MsgReturn})
  end
end

-- Show some help text

function Lcl.Help()
  io.write("Type 'show' to see all active windows\n")
  io.write("Type 'window_id msg' to send message to window\n")
  io.write("Standard messages are 'create', 'ok' and 'cancel'\n")
end

-- Simulate the generation of a window event. For a windowed application, this
-- would typically originate with the graphical shell (Windows: GetMessage,
-- XLib: XNextEvent. No coroutine yielding occurs here, so this can be a
-- binding to C.

function Lcl.EventGet()
  local Wnd, Msg
  if Lcl.MsgQueue[1] then -- If event is queued, retrieve the first one in
    local Rec = table.remove(Lcl.MsgQueue, 1)
    Wnd, Msg = Rec[1], Rec[2]
  else -- Wait for event from user
    while not Msg do
      io.write("Cmd> ")
      local Str = io.read()
      Str = string.gsub(Str, "^ *(.-) *$", "%1")
      Str = string.lower(Str)
      if Str == "help" or Str == "?" then
        Lcl.Help()
      elseif Str == "show" then
        Lcl.Show()
      else -- Pass message along to designated window
        local IdStr, MsgStr = string.match(Str, "(%S+)%s+(%S+)")
        if IdStr then
          Wnd = Lcl.WindowList[IdStr]
          if Wnd then
            if not Wnd.Close then
              Msg = MsgStr
            else
              io.write("Window ", IdStr, " is inactive\n")
            end
          else
            io.write("Unknown window: ", IdStr, "\n")
          end
        else
          io.write("Expecting 'help', 'show' or 'window_id msg'\n")
        end
      end
    end
  end
  return Wnd, Msg
end

-- Main event loop. All coroutines are resumed from this function and yield
-- back to it.

function Lcl.EventLoop()
  local Wnd, Msg
  local Loop = true
  while Loop do
    Wnd, Msg = Lcl.EventGet()
    if Wnd then
      Lcl.WindowActive = Wnd
      if Msg == Lcl.MsgReturn then
        -- Resume blocking window call
        if Wnd.Co then
          coroutine.resume(Wnd.Co, Wnd.Close)
        else
          Loop = false
          Msg = Wnd.Close
        end
      else
        -- Non-terminating message was received. Notify window in new coroutine
        -- rather than direct function call because a new blocking window may
        -- be raised.
        local Co = coroutine.create(Wnd.Fnc)
        coroutine.resume(Co, Wnd.User, Msg)
      end
    end
  end
  return Msg
end

function Window.Show(Fnc)
  local Parent = Lcl.WindowActive -- Nil for first window shown
  local Msg, Id
  if Parent then
    Parent.ChildSerial = Parent.ChildSerial + 1
    Id = Parent.Id .. "." .. Parent.ChildSerial
  else -- First window
    Lcl.Help()
    Id = "1"
  end
  local Co = coroutine.running()
  local Wnd = {Parent = Parent, Co = Co, Id = Id, Fnc = Fnc,
    ChildCount = 0, ChildSerial = 0, ChildList = {}, User = {Id = Id}}
  io.write("Creating window ", Wnd.Id, "\n")
  table.insert(Lcl.MsgQueue, {Wnd, "create"})
  Lcl.WindowList[Id] = Wnd
  if Parent then
    assert(Co)
    Parent.ChildList[Wnd] = true
    Parent.ChildCount = Parent.ChildCount + 1
    -- We're running in a coroutine; yield back to event loop. The current
    -- coroutine will not be resumed until the newly created window and all of
    -- its descendent windows have been destroyed. This happens when a
    -- Lcl.MsgReturn is posted for this window.
    Msg = coroutine.yield()
    Parent.ChildCount = Parent.ChildCount - 1
    Parent.ChildList[Wnd] = nil
    -- Close parent if it's in pending state
    Lcl.Destroy(Parent)
  else
    assert(not Co)
    -- We're running in main thread; call event loop and don't return
    -- until the loop ends
    Msg = Lcl.EventLoop()
  end
  Lcl.WindowList[Id] = nil
  return Msg
end

function Window.Close(Msg)
  local Wnd = Lcl.WindowActive
  Wnd.Close = Msg or "destroy"
  Lcl.Destroy(Wnd)
end

return Window
