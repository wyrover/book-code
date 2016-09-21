local app = require 'sdl_app'
require 'utils/proxy'

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

local function wrapSurfaceObject(rawSurfaceObject, manageMemory)
	if type(manageMemory)~='boolean' then
		manageMemory = true
	end

	local mt = {
    	__gc = function()
    		if manageMemory then
				SDL.SDL_FreeSurface(rawSurfaceObject)
				manageMemory = false
				rawSurfaceObject = nil
			end
		end,
	}

	local surface = {
		raw = rawSurfaceObject,
		blit = function(destination, srcRect, destRect)
			SDL.SDL_BlitSurface(rawSurfaceObject, srcRect, destination.raw, destRect)
		end,
		convert = function(newPixelFormat, flags)
			local flags = flags or 0
			return wrapSurfaceObject(SDL.SDL_ConvertSurface(rawSurfaceObject, newPixelFormat, flags))
		end,
		fill = function(rect, color)
			SDL.SDL_FillRect(rawSurfaceObject, rect, color)
		end,
		mapRGBA = function(r, g, b, a)
			return SDL.SDL_MapRGBA(rawSurfaceObject.format, r, g, b, a)
		end,
		free = function()
			mt.__gc()
		end,
	}

	setmetatable(surface, mt)
	table.proxy(surface)
	return surface
end

local function createSurface(params)
	local params = params or {}
	params.flags = params.flags or 0
	params.width = params.width or 0
	params.height = params.height or 0
	params.depth = params.depth or 0

	local defaultRmask, defaultGmask, defaultBmask, defaultAmask

	if params.depth == 32 then
		defaultRmask = 0xFF000000
		defaultGmask = 0x00FF0000
		defaultBmask = 0x0000FF00
		defaultAmask = 0x000000FF
	elseif params.depth == 24 then
		defaultRmask = 0x00FF0000
		defaultGmask = 0x0000FF00
		defaultBmask = 0x000000FF
		defaultAmask = 0x00000000
	end

	params.Rmask = params.Rmask or defaultRmask
	params.Gmask = params.Gmask or defaultGmask
	params.Bmask = params.Bmask or defaultBmask
	params.Amask = params.Amask or defaultAmask

	local rawSurfaceObject = SDL.SDL_CreateRGBSurface(
		params.flags,
		params.width, params.height,
		params.depth,
		params.Rmask, params.Gmask, params.Bmask, params.Amask
	)

	return wrapSurfaceObject(rawSurfaceObject)
end

local events = {
	startup = function()
		local screen = wrapSurfaceObject(window.screen, false)

		-- create a surface with 128x128 pixels and 32-bit color depth
		local surface = createSurface({
			width = 128,
			height = 128,
			depth = 32,
		})

		local rectangle = SDL.SDL_Rect_local()
		rectangle.x = 0
		rectangle.y = 0
		rectangle.w = 128
		rectangle.h = 128

		local color = surface.mapRGBA(255, 0, 0, 255)

		-- fill whole surface with red color
		surface.fill(rectangle, color)

		local srcRectangle = SDL.SDL_Rect_local()
		srcRectangle.x = 0
		srcRectangle.y = 0
		srcRectangle.w = 128
		srcRectangle.h = 128

		-- place top-left corner of the red rectangle at coordinates [128, 128]
		local destRectangle = SDL.SDL_Rect_local()
		destRectangle.x = 128
		destRectangle.y = 128
		destRectangle.w = 640
		destRectangle.h = 480

		-- put surface content to the screen
		surface.blit(screen, srcRectangle, destRectangle)

		-- flip content of the back-buffer into front-buffer (screen)
		SDL.SDL_Flip(window.screen)
	end,
}

app.main(window, events)

