-- Here's the SDL program from the exercise in this chapter.
-- It is not very well commented.  Before you run it, figure
-- out what it does by reading it.

assert(cheia.load("SDL"))

local function GetImgs()
  local Goodie, Baddie, ErrStr
  Goodie = SDL.SDL_LoadBMP("goodie.bmp")
  if Goodie then
    Baddie = SDL.SDL_LoadBMP("baddie.bmp")
    if Baddie then
      if Goodie.w ~= Baddie.w or Goodie.h ~= Baddie.h then
        Goodie = nil
        ErrStr = debug.traceback(
          "goodie.bmp and baddie.bmp must have the same shape")
      end
    end
  else
    ErrStr = debug.traceback(SDL.SDL_GetError())
  end
  return Goodie, Baddie, ErrStr
end

local function Tile(Goodie, Baddie, Screen)
  math.randomseed(os.time())
  local ErrStr
  for X = 0, 3 do
    for Y = 0, 3 do
      local Img = math.random(1, 2) == 1 and Goodie or Baddie
      if SDL.SDL_BlitSurface(Img, nil, Screen,
        {x = X * Goodie.w, y = Y * Goodie.h}) ~= 0
      then
        ErrStr = debug.traceback(SDL.SDL_GetError())
        break -- BREAK ON ERROR.
      end
    end
  end
  return not ErrStr, ErrStr
end

local function WaitForQuit()
  local Event = SDL.SDL_Event_new()
  while Event do
    local Pending = SDL.SDL_PollEvent(Event) == 1
    if Pending
      and (Event.type == SDL.SDL_QUIT
        or Event.type == SDL.SDL_KEYDOWN
        and Event.key.keysym.sym == SDL.SDLK_q)
    then
      Event = false
    end
  end
end

local ErrStr
if SDL.SDL_Init(SDL.SDL_INIT_VIDEO) == 0 then
  local Goodie, Baddie
  Goodie, Baddie, ErrStr = GetImgs()
  if Goodie then
    local Screen = SDL.SDL_SetVideoMode(Goodie.w * 4, Goodie.w * 4, 8,
      SDL.SDL_SWSURFACE)
    if Screen then
      local TileSucc
      TileSucc, ErrStr = Tile(Goodie, Baddie, Screen)
      if TileSucc then
        SDL.SDL_UpdateRect(Screen, 0, 0, 0, 0)
        WaitForQuit()
      end
    else
      ErrStr = debug.traceback(SDL.SDL_GetError())
    end
  else
    ErrStr = debug.traceback(SDL.SDL_GetError())
  end
else
  ErrStr = debug.traceback(SDL.SDL_GetError())
end
if ErrStr then
  io.stderr:write(ErrStr, "\n")
end
