local app = require 'sdl_app'

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

local events = {
	[SDL.SDL_MOUSEMOTION] = function(event_struct)
		local event = event_struct.motion
		-- do something when this event occurs
		print(("Mouse cursor coordinates [%-8d, %-8d]"):format(event.x, event.y))
	end,
	[SDL.SDL_MOUSEBUTTONDOWN] = function(event_struct)
		local event = event_struct.button
	end,
	[SDL.SDL_MOUSEBUTTONUP] = function(event_struct) 
		local event = event_struct.button
	end,
	idle = function()
	end,
}

app.main(window, events)

