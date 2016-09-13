-- Simple SDL graphics demo.  Usage: luacheia5 test.lua
-- (This is the initial version, saved as movearound.1.lua.  The
-- final version is available as movearound.lua.)

assert(cheia.load("SDL"))
assert(cheia.load("bit"))

-- Returns a "screen" (a canvas within a window upon which the
-- sprites will be drawn):
local function MakeScreen()
  local ErrStr
  local Screen = SDL.SDL_SetVideoMode(640, 480, 8,
    bit.bor(SDL.SDL_SWSURFACE, SDL.SDL_ANYFORMAT))
  if not Screen then
    ErrStr = debug.traceback(SDL.SDL_GetError())
  end
  return Screen, ErrStr
end

-- Returns a sprite object; ImgName is the filename of a .bmp file:
local function MakeSprite(Screen, ImgName)
  local Sprite, ErrStr
  local Img = SDL.SDL_LoadBMP(ImgName)
  if Img then
    local Background = SDL.SDL_MapRGB(Screen.format, 0, 0, 0)
    -- Current X and Y positions:
    local CurX, CurY = 0, 0
    -- Tables to be (re)used as rectangle arguments to
    -- SDL_FillRect and SDL_BlitSurface:
    local FillRect = {x = 0, y = 0, w = Img.w, h = Img.h}
    local BlitRect = {x = 0, y = 0, w = Img.w, h = Img.h}
    -- The sprite object:
    Sprite = {}

    -- Moves the sprite; X and Y default to the current position if
    -- they are nil:
    function Sprite:Move(X, Y)
      local Succ, ErrStr
      X, Y = math.floor(X or CurX), math.floor(Y or CurY)
      -- Erase the sprite at its current position:
      FillRect.x, FillRect.y = CurX, CurY
      if SDL.SDL_FillRect(Screen, FillRect, Background) == 0 then
        -- Write it to its new position:
        BlitRect.x, BlitRect.y = X, Y
        if SDL.SDL_BlitSurface(Img, nil, Screen, BlitRect) == 0 then
          CurX, CurY = X, Y
          Succ = true
        else
          Succ, ErrStr = false, debug.traceback(SDL.SDL_GetError())
        end
      else
        Succ, ErrStr = false, debug.traceback(SDL.SDL_GetError())
      end
      return Succ, ErrStr
    end

    -- Give the sprite its initial position:
    local Succ
    Succ, ErrStr = Sprite:Move()
    if not Succ then
      Sprite = nil
    end
  else
    ErrStr = debug.traceback(SDL.SDL_GetError())
  end
  return Sprite, ErrStr
end

-- Returns a function that gets pending events (all it returns in
-- this version is whether to quit):
local function MakeGetEvents()
  -- An SDL_Event structure -- this is how SDL.SDL_PollEvent returns
  -- any event it finds in the event queue:
  local Event = SDL.SDL_Event_new()

  -- Returns whether to quit:
  return function()
    local Quit = false
    -- Consume pending events:
    while not Quit and SDL.SDL_PollEvent(Event) == 1 do
      -- Was the window closed?
      if Event.type == SDL.SDL_QUIT then
        Quit = true -- Break the loop.
      -- Was "q" pressed?
      elseif Event.type == SDL.SDL_KEYDOWN
        and Event.key.keysym.sym == SDL.SDLK_q
      then
        Quit = true -- Break the loop.
      end
    end
    return Quit
  end
end

local function MainLoop(Screen, UserSprite)
  local Succ, ErrStr = true
  -- Put the sprite 32 pixels from the left side, and 0 pixels
  -- from the top, of the screen (window):
  Succ, ErrStr = UserSprite:Move(32, 0)
  if Succ then
    -- Update the entire screen:
    SDL.SDL_UpdateRect(Screen, 0, 0, 0, 0)
    local GetEvents = MakeGetEvents()
    -- GetEvents returns true if the user quits; loop until that
    -- happens:
    while not GetEvents() do end
  end
  return Succ, ErrStr
end

local function Main()
  local ErrStr
  local InitRes = SDL.SDL_Init(SDL.SDL_INIT_VIDEO)
  if InitRes == 0 then
    local Screen
    Screen, ErrStr = MakeScreen()
    if Screen then
      -- The window title is someone misleading in this initial
      -- version:
      SDL.SDL_WM_SetCaption("Move Around", "Move Around");
      SDL.SDL_EnableKeyRepeat(SDL.SDL_DEFAULT_REPEAT_DELAY,
        SDL.SDL_DEFAULT_REPEAT_INTERVAL)
      -- Create a sprite (eventually to be controlled by the user):
      local UserSprite
      UserSprite, ErrStr = MakeSprite(Screen, "goodie.bmp")
      if UserSprite then
        io.write("In the new window, press 'q' (or close the",
          " window) to quit.\n")
        io.flush()
        local _
        _, ErrStr = MainLoop(Screen, UserSprite)
      end
    end
    SDL.SDL_Quit()
  else
    ErrStr = debug.traceback(SDL.SDL_GetError())
  end
  return not ErrStr, ErrStr
end

local Succ, ErrStr = Main()
if not Succ then
  io.stderr:write(ErrStr, "\n")
end
