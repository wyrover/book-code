require 'utils/proxy'

local function rect(params)
	if type(params)=='table' then
		local rectangle = SDL.SDL_Rect_local()
		rectangle.x = params.x or 0
		rectangle.y = params.y or 0
		rectangle.w = params.w or 0
		rectangle.h = params.h or 0
		return rectangle
	else
		return nil
	end
end

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
		blit = function(destination, srcRectTable, destRectTable)
			local srcRect = rect(srcRectTable)
			local destRect = rect(destRectTable)
			SDL.SDL_BlitSurface(rawSurfaceObject, srcRect, destination.raw, destRect)
		end,
		convert = function(newPixelFormat, flags)
			local flags = flags or 0
			return wrapSurfaceObject(SDL.SDL_ConvertSurface(rawSurfaceObject, newPixelFormat, flags))
		end,
		fill = function(fillRectTable, color)
			local fillRect = rect(fillRectTable)
			SDL.SDL_FillRect(rawSurfaceObject, fillRect, color)
		end,
		mapRGBA = function(r, g, b, a)
			return SDL.SDL_MapRGBA(rawSurfaceObject.format, r, g, b, a)
		end,
		zoom = function(scaleX, scaleY)
			local scaleX = scaleX or 1
			local scaleY = scaleY or 1
			if type(SDL.GFX_ZoomSurface)=='function' then
				return wrapSurfaceObject(SDL.GFX_ZoomSurface(rawSurfaceObject, scaleX, scaleY, 1))
			end
		end,
		free = function()
			mt.__gc()
		end,
		width = rawSurfaceObject.w,
		height = rawSurfaceObject.h,
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

return {
	new = createSurface,
	wrap = wrapSurfaceObject,
}