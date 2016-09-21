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

local timers = {}
local timerID = 0

-- Each call to createTimer function will create a new timer object.
-- Interval argument expects a time in millisecond units
-- Supplied function in the second argument will be called repeatedly
local function createTimer(interval, fn)
	assert(type(interval)=='number' and type(fn)=='function')
	local timerObj = {}
	-- Unique timerID number generator with simple incremental counting
	timerID = timerID+1
	-- timerEvent will be used to contain timerID user value
	local timerEvent = SDL.SDL_Event_local()
	-- Store a reference for timerEvent object
	-- so it won't be prematurely garbage collected
	timerObj.event = timerEvent
	timerObj.call = fn
	timerEvent.type = SDL.SDL_USEREVENT
	timerEvent.user.code = timerID
	-- Create LuaSDL timer object
	local timer = SDL.SDL_AddTimer(interval, timerEvent)
	timerObj.timer = timer
	-- Destroys current timer object
	timerObj.remove = function()
		SDL.SDL_RemoveTimer(timer)
		timers[timerID] = nil
	end
	timers[timerID] = timerObj
end

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
	[SDL.SDL_USEREVENT] = function(event_struct)
		local event = event_struct.user
		if event.code > 0 then
			local timer = timers[event.code]
			if timer and type(timer.call)=="function" then
				timer.call()
			end
		end
	end,
	startup = function()
		createTimer(2000, function()
			print("timercallback1()")
		end)
		createTimer(1000, function()
			print("timercallback2()")
		end)
		createTimer(500, function()
			print("timercallback3()")
		end)
	end,
	cleanup = function()
		for _, timer in pairs(timers) do
			timer.remove()
		end
	end,
	idle = function()
	end,
}

app.main(window, events)
