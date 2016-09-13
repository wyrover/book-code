-- Simple SDL graphics demo.  Usage: luacheia5 test.lua
-- (This is the final version.  The initial version is available as
-- movearound.1.lua.)

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

-- Returns N, or (if N is lower than Min), a number as much above Min
-- as N is below it, or (if N is higher than Max), a number as much
-- below Max as N is above it; also returns a second value telling
-- whether had to return a number other than N:
local function ComputeBounce(N, Min, Max)
  local Fix = false
  if N > Max then
    N = Max - (N - Max)
    Fix = true
  elseif N < Min then
    N = Min + (Min - N)
    Fix = true
  end
  return N, Fix
end

-- Returns N, or Min if N is lower than Min, or Max, if N is higher
-- than Max:
local function Between(N, Min, Max)
  return N > Max and Max or N < Min and Min or N
end

-- Returns 1, 0, or -1 depending on whether N is positive, zero, or
-- negative:
local function Sign(N)
  if N > 0 then
    N = 1
  elseif N < 0 then
    N = -1
  end
  return N
end

-- Returns a sprite object; ImgName is the filename of a .bmp file:
local function MakeSprite(Screen, ImgName)
  local Sprite, ErrStr
  local Img = SDL.SDL_LoadBMP(ImgName)
  if Img then
    local Background = SDL.SDL_MapRGB(Screen.format, 0, 0, 0)
    -- Current X and Y positions:
    local CurX, CurY = 0, 0
    -- Current velocities along the X and Y axes, in pixels per tick
    -- (minimum -1, maximum 1):
    local VelX, VelY = 0, 0
    -- Velocity increment (the smallest unit by which velocity along
    -- a given axis changes):
    local VelInc = 1 / 32
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

    -- Call this once for every tick:
    function Sprite:Tick(Ticks)
      local Succ, ErrStr = true
      local NewX
      if VelX ~= 0 and math.mod(Ticks, 1 / VelX) < 1 then
        NewX = CurX + Sign(VelX)
        -- Make sure it doesn't go off the edges:
        local Fix
        NewX, Fix = ComputeBounce(NewX, 0,
          self:ScreenWidth() - self:Width())
        if Fix then VelX = -VelX end
      end
      local NewY
      if VelY ~= 0 and math.mod(Ticks, 1 / VelY) < 1 then
        NewY = CurY + Sign(VelY)
        -- Make sure it doesn't go off the edges:
        local Fix
        NewY, Fix = ComputeBounce(NewY, 0,
          self:ScreenHeight() - self:Height())
        if Fix then VelY = -VelY end
      end
      if NewX or NewY then
        Succ, ErrStr = self:Move(NewX, NewY)
      end
      return Succ, ErrStr
    end

    -- Accelerates the sprite along the X axis (negative values of
    -- Accel accelerate to the left):
    function Sprite:AccelX(Accel)
      if Accel ~= 0 then
        VelX = Between(VelX + Accel * VelInc, -1, 1)
      end
    end

    -- Accelerates the sprite along the Y axis (negative values of
    -- Accel accelerate upward):
    function Sprite:AccelY(Accel)
      if Accel ~= 0 then
        VelY = Between(VelY + Accel * VelInc, -1, 1)
      end
    end

    -- Stops the sprite cold:
    function Sprite:Stop()
      VelX, VelY = 0, 0
    end

    -- More methods:
    function Sprite:X() return CurX end
    function Sprite:Y() return CurY end
    function Sprite:Width() return Img.w end
    function Sprite:Height() return Img.h end
    function Sprite:ScreenWidth() return Screen.w end
    function Sprite:ScreenHeight() return Screen.h end

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

-- Returns a function that gets pending events:
local function MakeGetEvents()
  -- An SDL_Event structure -- this is how SDL.SDL_PollEvent returns
  -- any event it finds in the event queue:
  local Event = SDL.SDL_Event_new()

  -- Returns how much to accelerate to the right, how much to
  -- accelerate downward, and whether to stop or quit:
  return function()
    local AccelX, AccelY, OtherEvent = 0, 0
    -- Consume pending events:
    while not OtherEvent and SDL.SDL_PollEvent(Event) == 1 do
      -- Was the window closed?
      if Event.type == SDL.SDL_QUIT then
        OtherEvent = "Quit" -- Break the loop.
      elseif Event.type == SDL.SDL_KEYDOWN then
        local Key = Event.key.keysym.sym
        -- Was "q" pressed?
        if Key == SDL.SDLK_q then
          OtherEvent = "Quit" -- Break the loop.
        -- If an arrow key was pressed, accelerate the sprite in that
        -- direction:
        elseif Key == SDL.SDLK_LEFT then
          AccelX = AccelX - 1
        elseif Key == SDL.SDLK_RIGHT then
          AccelX = AccelX + 1
        elseif Key == SDL.SDLK_UP then
          AccelY = AccelY - 1
        elseif Key == SDL.SDLK_DOWN then
          AccelY = AccelY + 1
        elseif Key == SDL.SDLK_SPACE then
          OtherEvent = "Stop" -- Break the loop.
        else
          -- Some other key; ignore it.
        end
      end
    end
    return AccelX, AccelY, OtherEvent
  end
end

-- Moves the user sprite and automatic sprite the appropriate
-- distance for the given amount of ticks:
local function Step(UserSprite, AutoSprite, Ticks)
  local Succ, ErrStr = true
  -- Move the sprites:
  Succ, ErrStr = UserSprite:Tick(Ticks)
  if Succ then
    Succ, ErrStr = AutoSprite:Tick(Ticks)
  end
  return Succ, ErrStr
end

-- Are Sprite1 and Sprite2 overlapping?
local function Overlap(Sprite1, Sprite2)
  local Ret = false
  -- This views the two sprites as rectangle-shaped, even if they
  -- look like other shapes.
  local DiffX = math.abs(Sprite1:X() - Sprite2:X())
  local Width = (Sprite1:Width() + Sprite2:Width()) / 2
  if DiffX < Width then
    local DiffY = math.abs(Sprite1:Y() - Sprite2:Y())
    local Height = (Sprite1:Height() + Sprite2:Height()) / 2
    if DiffY < Height then
      Ret = true
    end
  end
  return Ret
end

local function MainLoop(Screen, UserSprite, AutoSprite)
  local Succ, ErrStr = true
  local GetEvents = MakeGetEvents()
  -- Loop until the user quits (or an error occurs):
  local PrevTicks = SDL.SDL_GetTicks();
  local OtherEvent
  while Succ and OtherEvent ~= "Quit" do
    -- Get any and all pending events:
    local AccelX, AccelY
    AccelX, AccelY, OtherEvent = GetEvents()
    -- Update the sprite's velocity:
    UserSprite:AccelX(AccelX)
    UserSprite:AccelY(AccelY)
    -- Get the current number of elapsed ticks -- if it's the same as
    -- the previous number, keep checking until it's different:
    local Ticks = SDL.SDL_GetTicks();
    while Ticks == PrevTicks do
      Ticks = SDL.SDL_GetTicks();
    end
    -- Call Step once for each tick that has happened since
    -- PrevTicks:
    for I = PrevTicks + 1, Ticks do
      Succ, ErrStr = Step(UserSprite, AutoSprite, I)
      if not Succ then break end -- BREAK ON ERROR.
    end
    PrevTicks = Ticks
    if OtherEvent == "Stop" then
      -- Screeching halt:
      UserSprite:Stop()
    end
    -- Are the two sprites overlapping?
    if Overlap(UserSprite, AutoSprite) then
      io.write("GAME OVER (collision)\n")
      break -- EXIT THE MAIN LOOP.
    end
    -- Update the entire screen:
    SDL.SDL_UpdateRect(Screen, 0, 0, 0, 0)
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
      -- Set the window title; enable key repeat:
      SDL.SDL_WM_SetCaption("Move Around", "Move Around");
      SDL.SDL_EnableKeyRepeat(SDL.SDL_DEFAULT_REPEAT_DELAY,
        SDL.SDL_DEFAULT_REPEAT_INTERVAL)
      -- Create a sprite to be controlled by the user and another to
      -- be controlled by the program:
      local UserSprite
      UserSprite, ErrStr = MakeSprite(Screen, "goodie.bmp")
      if UserSprite then
        local AutoSprite
        AutoSprite, ErrStr = MakeSprite(Screen, "baddie.bmp")
        if AutoSprite then
          -- Give them their initial positions and velocities:
          local Succ
          local ScreenWidth, ScreenHeight = Screen.w, Screen.h
          Succ, ErrStr = UserSprite:Move(
            ScreenWidth / 3 - UserSprite:Width() / 2,
            ScreenHeight / 2 - UserSprite:Height() / 2)
          if Succ then
            Succ, ErrStr = AutoSprite:Move(
              ScreenWidth * (2 / 3) - AutoSprite:Width() / 2,
              ScreenHeight / 2 - AutoSprite:Height() / 2)
            if Succ then
              -- Go in one of the four diagonal directions depending
              -- on what time it is:
              local Time = math.mod(os.time(), 4) + 1
              local A = 5 -- AutoSprite initial acceleration.
              local AccelX = ({A, A, -A, -A})[Time]
              local AccelY = ({A, -A, -A, A})[Time]
              AutoSprite:AccelX(AccelX)
              AutoSprite:AccelY(AccelY)
              io.write([[
In the new window:
- press the arrow keys to change speed
- press space to stop moving
- press 'q' (or close the window) to quit.
]])
              io.flush()
              Succ, ErrStr = MainLoop(Screen, UserSprite, AutoSprite)
            end
          end
        end
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
