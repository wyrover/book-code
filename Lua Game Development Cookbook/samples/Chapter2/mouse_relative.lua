local bit = require 'bit'
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

local centerX, centerY = window.width/2, window.height/2
local relativeX, realtiveY, totalX, totalY = 0,0,0,0

local function mouseMotionHandler(event_struct)
	local event = event_struct.motion
	SDL.SDL_WarpMouse(centerX, centerY)
	relativeX, relativeY = event.x - centerX, event.y - centerY
	totalX, totalY = totalX + relativeX, totalY + relativeY
	print(("Mouse cursor coordinates [%-8d, %-8d]"):format(totalX, totalY))
end

local events = {
	[SDL.SDL_ACTIVEEVENT] = function(event_struct)
		local event = event_struct.active
		if SDL.And(event.state, SDL.SDL_APPINPUTFOCUS) > 0 then
			if event.gain == 1 then
				SDL.SDL_WarpMouse(centerX, centerY)
				events[SDL.SDL_MOUSEMOTION] = mouseMotionHandler
			else
				events[SDL.SDL_MOUSEMOTION] = false
			end
		end
	end,
	[SDL.SDL_MOUSEMOTION] = mouseMotionHandler,
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

