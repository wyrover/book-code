require 'LuaSDL'

function SDL_assert(condition, msg)
  local msg = (msg and msg .. ": " .. SDL.SDL_GetError())
  return assert(condition, msg)
end

local window = {
	x = 128,
	y = 128,
	width = 640,
	height = 480,
	bpp = 0,
	flags = 0,
	caption = 'PackPub Example',
	class = 'Example',
}

local function main()
	local running = true
	
	local events = {
		[SDL.SDL_MOUSEMOTION] = function(event_struct)
			local event_struct = event_struct.motion
			-- do something when this event occurs
		end,
		[SDL.SDL_MOUSEBUTTONDOWN] = function(event_struct)
			local event = event_struct.button
		end,
		[SDL.SDL_MOUSEBUTTONUP] = function(event_struct) 
			local event = event_struct.button
		end,
		[SDL.SDL_KEYDOWN] = function(event_struct) 
			local event = event_struct.key
			local key = event.keysym.sym
		
		    -- send SDL_QUIT event on Escape key
			if key == SDL.SDLK_ESCAPE then
				local event = SDL.SDL_Event_local()
				event.type = SDL.SDL_QUIT
				SDL.SDL_PushEvent(event)
			end
		end,
		[SDL.SDL_KEYUP] = function(event_struct) 
			local event = event_struct.key
			local key = event.keysym.sym
		end,
		[SDL.SDL_VIDEORESIZE] = function(event_struct) 
			local event = event_struct.resize
		end,
		[SDL.SDL_QUIT] = function(event_struct) 
			running = false
		end,
	}

	SDL_assert(SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) >= 0, "Couldn't initialize SDL")
	SDL.SDL_putenv(string.format("SDL_VIDEO_WINDOW_POS=%d,%d",window.x, window.y))
	SDL.SDL_WM_SetCaption(window.caption, window.class)
	local screen = SDL_assert(SDL.SDL_SetVideoMode(window.width, window.height, window.bpp, window.flags))

	-- prepare local instance of SDL_Event object
	local local_event = SDL.SDL_Event_local()

	while (running) do
		-- check for events in the poll
		if (SDL.SDL_PollEvent(local_event)~=0) then
			local event_fn = events[local_event.type]
			if type(event_fn)=='function' then
				event_fn(local_event)
			end
		end
	end

	SDL.SDL_Quit()
end

main()
