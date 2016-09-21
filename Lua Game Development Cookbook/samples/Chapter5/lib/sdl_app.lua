require 'LuaSDL'

local M = {}

function SDL_assert(condition, msg)
  local msg = (msg and msg .. ": " .. SDL.SDL_GetError())
  return assert(condition, msg)
end

local function main(window, events)
	local running = true

	-- quits the application
	M.quit = function()
		running = false
	end

	local events = events or {}

	-- add default event handler for SDL_KEYDOWN event
	events[SDL.SDL_KEYDOWN] = events[SDL.SDL_KEYDOWN] or function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
	
	    -- send SDL_QUIT event on Escape key
		if keySym == SDL.SDLK_ESCAPE then
			local event = SDL.SDL_Event_local()
			event.type = SDL.SDL_QUIT
			SDL.SDL_PushEvent(event)
		end
	end

	-- add default event handler for SDL_QUIT event
	events[SDL.SDL_QUIT] = events[SDL.SDL_QUIT] or function(event_struct) 
		M.quit()
	end
	
	SDL_assert(SDL.SDL_Init(SDL.SDL_INIT_EVERYTHING) >= 0, "Couldn't initialize SDL")
	SDL.SDL_putenv(string.format("SDL_VIDEO_WINDOW_POS=%d,%d",window.x, window.y))
	SDL.SDL_WM_SetCaption(window.caption, window.class)
	local screen = SDL_assert(SDL.SDL_SetVideoMode(window.width, window.height, window.bpp, window.flags))
	window.screen = screen

	-- prepare local instance of SDL_Event object
	local local_event = SDL.SDL_Event_local()

	if type(events.startup)=='function' then
		events.startup()
	end

	while (running) do
		-- check for events in the poll
		if (SDL.SDL_PollEvent(local_event)~=0) then
			local event_fn = events[local_event.type]
			if type(event_fn)=='function' then
				event_fn(local_event)
			end
		else
			if type(events.idle)=='function' then
				events.idle()
			end
		end
	end

	SDL.SDL_Quit()

	if type(events.cleanup)=='function' then
		events.cleanup()
	end
end

M.main = main

return M
