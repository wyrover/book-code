local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'

local function createAtlas(atlasWidth, atlasHeight)
	local allocated = {}
	local full = false
	local atlasTexture = texture.empty(atlasWidth, atlasHeight)
	local items = {}
	
	local function allocBlock(width, height)
		local i, j, best1, best2, x, y = 0, 0, atlasHeight, 0, 0, 0
		for i = 0, atlasWidth - width do
			best2 = 0
			for k = 0, width - 1 do
				local column = allocated[i + k]
				j = k
				if not column then
					column = 0
					allocated[i + k] = column
				end
				if (column >= best1) then
					break
				end
				if (column > best2) then
					best2 = column
				end
			end
			if j == width - 1 then
				-- valid area
				x = i
				y, best1 = best2, best2
			end
		end
		if (best1 + height) > atlasHeight then
			return false
		end
		for i = 0, width - 1 do
			allocated[x + i] = best1 + height
		end
		return x, y
	end

	local function add(itemName, surface, transparent)
		assert(type(itemName)=='string' or type(itemName)=='number')
		local width, height = surface.width, surface.height
		local bpp = surface.raw.format.BitsPerPixel
		local glFormat

		if bpp == 24 then
			glFormat = gl_enum.GL_RGB
		elseif bpp == 32 then
			glFormat = gl_enum.GL_RGBA
		else
			error('Unsupported texture format')
		end
		if (transparent) then
			SDL.SDL_SetAlpha(surface.raw, 0, 0)
		end

		local x,y = allocBlock(width, height)
		if x and y then
			atlasTexture.use()
			
			-- put surface data into atlas texture at position specified by allocBlock function
			gl.TexSubImage2D(gl_enum.GL_TEXTURE_2D, 0, x, y, width, height, glFormat, surface.raw.pixels)

			atlasTexture.unUse()

			local item = {
				-- integer dimensions and coordinates
				x = x,
				y = y,
				width = width,
				height = height,

				-- floating-point coordinates in numerical range <0, 1> usable in TexCoord2f function
				fX = x/atlasWidth,
				fY = y/atlasHeight,
				fWidth = width/atlasWidth,
				fHeight = height/atlasHeight,
			}

			items[itemName] = item
			return item
		else
			return false, "Not enough space in atlas"
		end
	end

	local function load(itemName, fname, transparent)
		assert(type(itemName)=='string' and type(fname)=='string')
		local rawSurfaceObject = surface.wrap(assert(SDL.IMG_Load(fname)))
		return add(itemName, rawSurfaceObject, transparent)
	end

	local deleted = false

	local delete = function()
		if not deleted then
			atlasTexture.delete()
			deleted = true
		end
	end

	local atlas = {
		items = items,
		add = add,
		load = load,
		use = atlasTexture.use,
		unUse = atlasTexture.unUse,
		delete = delete,
	}

	setmetatable(atlas, {
		__gc = delete,
	})
	table.proxy(atlas)

	return atlas
end

return {
	new = createAtlas,
}