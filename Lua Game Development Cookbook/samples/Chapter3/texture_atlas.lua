local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'

local window = {
	x = 128,
	y = 128,
	width = 640,
	height = 480,
	bpp = 32,
	flags = SDL.SDL_OPENGL,
	caption = 'PackPub Example',
	class = 'Example',
	fullScreen = false,
}

if window.fullScreen then
	window.flags = bit.bor(window.flags, SDL.SDL_FULLSCREEN)
end

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
		assert(type(itemName)=='string')
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

	return {
		items = items,
		add = add,
		load = load,
		use = atlasTexture.use,
		unUse = atlasTexture.unUse,
	}
end

local atlas

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		gl.ClearColor(0.3,0.3,0.3,1)

		-- prepare atlas texture
		atlas = createAtlas(2048, 2048)

		-- load few images into atlas
		assert(atlas.load('Lena', "../../common/assets/lena_std.png"))
		assert(atlas.load('Bitmap font', "../../common/assets/bitmap_font.png"))
		assert(atlas.load('Earth', "../../common/assets/earth.png"))
		assert(atlas.load('Lua', "../../common/assets/Lua-Logo_2.png"))
		assert(atlas.load('Normal map', "../../common/assets/normal_map.jpg"))
		-- you can load images more than once, if you want to do some postprocessing on this version
		-- otherwise it's better to store and check image file names you've already used
		assert(atlas.load('Lena 2', "../../common/assets/lena_std.png"))

		-- enable texture rendering
		gl.Enable(gl_enum.GL_TEXTURE_2D)

		-- enable blending function for alpha transparency
   		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
   		gl.BlendEquation(gl_enum.GL_FUNC_ADD)
   		gl.BlendColor(1,1,1,1)
   		gl.Enable(gl_enum.GL_BLEND)
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		atlas.use()

		local width, height = 0.5, 0.5
		local x, y = -1, 1-height

		-- draw all images from the atlas in grid-like fashion
		for name, item in pairs(atlas.items) do
			gl.Begin(gl_enum.GL_QUADS)
				gl.Color4f(1, 1, 1, 1)

				gl.TexCoord2f(item.fX, item.fY + item.fHeight)
				gl.Vertex2f(x, y)

				gl.TexCoord2f(item.fX + item.fWidth, item.fY + item.fHeight)
				gl.Vertex2f(x + width, y)

				gl.TexCoord2f(item.fX + item.fWidth, item.fY)
				gl.Vertex2f(x + width, y + height)

				gl.TexCoord2f(item.fX, item.fY)
				gl.Vertex2f(x, y + height)
			gl.End()

			x = x + width
			if x > 1-width then
				x = -1
				y = y - height
			end
		end

		atlas.unUse()

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
	end,
}

app.main(window, events)

