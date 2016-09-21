local app = require 'sdl_app'
local bit = require 'bit'
local surface = require 'SDL/surface'

local window = {
	x = 128,
	y = 128,
	width = 640,
	height = 480,
	bpp = 32,
	flags = 0,
	caption = 'PackPub Example',
	class = 'Example',
	fullScreen = false,
}

if window.fullScreen then
	window.flags = bit.bor(window.flags, SDL.SDL_FULLSCREEN)
end

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		-- load image file
		local image = surface.wrap(assert(SDL.IMG_Load("../../common/assets/lena_std.png")))

		-- draws image on the screen
		image.blit(screen, {
			x = 0,
			y = 0,
			w = window.width - 32*2,
			h = window.height - 32*2,
		}, {
			x = 32,
			y = 32,
			w = 0,
			h = 0,
		})
		SDL.SDL_Flip(window.screen)
	end,
}

app.main(window, events)

