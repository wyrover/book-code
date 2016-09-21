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


-- tileset definitions
local tileSets = {
	['creatures'] = {
		fileName = "../../common/assets/creatures_alpha.png",
		transparent = true,
		tiles = {
			['dragon'] = {
				x=32, y=192, w=32, h=32,
			},
			['wizard'] = {
				x=448, y=128, w=32, h=32,
			},
		},
		texture = false,
	},
	['terrain'] = {
		fileName = "../../common/assets/terrain_alpha.png",
		transparent = true,
		tiles = {
			['stones'] = {
				x=64, y=0, w=32, h=32,
			},
			['grass'] = {
				x=0, y=160, w=32, h=32,
			},
			['statue'] = {
				x=0, y=64, w=32, h=32,
			},
		},
		texture = false,
	},
}

--returns tile information
local function getTile(tileSetName, tileName)
    local tileSet = tileSets[tileSetName]
    if type(tileSet)=='table' then
    	local tile = tileSet.tiles[tileName]
    	if type(tile)=='table' then
    		local tileSetTexture = tileSet.texture
    		return tileSetTexture, tileSetTexture.width, tileSetTexture.height, tile.x, tile.y, tile.w, tile.h
		end
	end
	return false, 'Unknown tile'
end

-- obtain texture coordinates that can be used in TexCoord2f function directly
-- converts pixel coordinates into numeric range <0, 1>
function transformCoordinates(tileSetTexture, tileset_width, tileset_height, x, y, w, h)
	local out_x = x/tileset_width
	local out_y = y/tileset_height
	local out_w = w/tileset_width
	local out_h = h/tileset_height
	return tileSetTexture, {
		{x = out_x, y = out_y + out_h},
		{x = out_x + out_w, y = out_y + out_h},
		{x = out_x + out_w, y = out_y},
		{x = out_x, y = out_y},
	}
end

local function drawTile(tileSetName, tileName, x, y, width, height)
	-- obtain tileset texture and tile coordinates
	local tileSetTexture, tile = transformCoordinates(assert(getTile(tileSetName, tileName)))

	tileSetTexture.use()

	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(1, 1, 1, 1)

		gl.TexCoord2f(tile[1].x, tile[1].y)
		gl.Vertex2f(x, y)

		gl.TexCoord2f(tile[2].x, tile[2].y)
		gl.Vertex2f(x + width, y)

		gl.TexCoord2f(tile[3].x, tile[3].y)
		gl.Vertex2f(x + width, y + height)

		gl.TexCoord2f(tile[4].x, tile[4].y)
		gl.Vertex2f(x, y + height)
	gl.End()

	tileSetTexture.unUse()
end

local aspectRatio = window.width/window.height

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		gl.ClearColor(0.3,0.3,0.3,1)

		-- load Dungeon Crawl tilesets
		for name, tileset in pairs(tileSets) do
			tileset.texture = texture.load(tileset.fileName, tileset.transparent)
		end

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

		-- a shorthand for tile dimensions - 8x8 rectangular tiles on screen
		local tileWidth = 0.25
		local tileHeight = tileWidth*aspectRatio

		-- draws the first layer - terrain
		for y=0,7 do
			for x=0,7 do
				drawTile('terrain', 'grass', -1 + x*tileWidth, 1-(y+1)*tileHeight, tileWidth, tileHeight)
			end
		end

		-- draws the second layer - items
		drawTile('terrain', 'stones', -1+tileWidth, 1-tileHeight*2, tileWidth, tileHeight)
		drawTile('terrain', 'stones', -1+tileWidth, 1-tileHeight*3, tileWidth, tileHeight)
		drawTile('terrain', 'statue', -1+tileWidth*2, 1-tileHeight*3, tileWidth, tileHeight)

		-- draw the third layer - creatures
		drawTile('creatures', 'wizard', -1+tileWidth, 1-tileHeight, tileWidth, tileHeight)
		drawTile('creatures', 'dragon', -1+tileWidth*2, 1-tileHeight*5, tileWidth, tileHeight)

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
	end,
}

app.main(window, events)

