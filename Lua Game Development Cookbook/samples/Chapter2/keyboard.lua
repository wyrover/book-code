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

local keyStates = {}
local keyModStates = {}

-- a list of key modifiers paired with their scan codes
local modKeysScanCodes = {
	[SDL.KMOD_LSHIFT]=SDL.SDLK_LSHIFT, [SDL.KMOD_RSHIFT]=SDL.SDLK_RSHIFT,
	[SDL.KMOD_LCTRL]=SDLK_LCTRL, [SDL.KMOD_RCTRL]=SDL.SDLK_RCTRLT,
	[SDL.KMOD_LALT]=SDL.SDLK_LALT, [SDL.KMOD_RALT]=SDL.SDLK_RALT,
	[SDL.KMOD_LMETA]=SDL.SDLK_LMETA, [SDL.KMOD_RMETA]=SDL.SDLK_RMETA,
	[SDL.KMOD_NUM]=SDL.SDLK_NUMLOCK,
	[SDL.KMOD_CAPS]=SDL.SDLK_CAPSLOCK,
	[SDL.KMOD_MODE]=SDL.SDLK_MODE,
}

local function processModKeys()
	local modState = SDL.SDL_GetModState()
	for keyMod, keySym in pairs(modKeysScanCodes) do
		-- apply binary and operator to obtain modifier key state
		keyModStates[keySym] = (bit.band(modState, keyMod) > 0)
	end
end

local events = {
	[SDL.SDL_KEYDOWN] = function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
		keyStates[keySym] = true
	
	    -- send SDL_QUIT event on Escape key
		if keySym == SDL.SDLK_ESCAPE then
			local event = SDL.SDL_Event_local()
			event.type = SDL.SDL_QUIT
			SDL.SDL_PushEvent(event)
		end
	end,
	[SDL.SDL_KEYUP] = function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
		keyStates[keySym] = false
	end,
	idle = function()
	end,
}

app.main(window, events)

