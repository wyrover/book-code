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

-- font parameters - they are dependent on bitmap texture dimensions
local font = {
	glyph = {
		width = 46,
		height = 49,
	},
	glyphsPerLine = 11,
	-- ASCII code for the first character in the bitmap texture
	firstCharIndex = 33
}

local function drawGlyph(font, glyphIndex, x, y, width, height)
	-- set up glyph texture coordinates
	local font_texture_x = font.glyph.width * (glyphIndex % font.glyphsPerLine)
	local font_texture_y = font.glyph.height * math.floor(glyphIndex / font.glyphsPerLine)

	local texcoord_x0 = font_texture_x / font.texture.width
	local texcoord_y0 = font_texture_y / font.texture.height
	local texcoord_x1 = texcoord_x0 + font.glyph.width/font.texture.width
	local texcoord_y1 = texcoord_y0 + font.glyph.height/font.texture.height

	font.texture.use()

	-- draws textured rectangle with glyph bitmap inside
	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(1,1,1,1)
		gl.TexCoord2f(texcoord_x0, texcoord_y1)
		gl.Vertex2f(x, y)
		gl.TexCoord2f(texcoord_x1, texcoord_y1)
		gl.Vertex2f(x + width, y)
		gl.TexCoord2f(texcoord_x1, texcoord_y0)
		gl.Vertex2f(x + width, y + height)
		gl.TexCoord2f(texcoord_x0, texcoord_y0)
		gl.Vertex2f(x, y + height)
	gl.End()

	font.texture.unUse()
end

local function drawText(font, text, x, y, width, height)
	assert(type(text)=='string')
	if #text>0 then
		local glyphWidth = width/#text
		local currentX = x

		for i = 1, #text do
			local glyphIndex = string.byte(text, i) - font.firstCharIndex

			drawGlyph(font, glyphIndex, currentX, y, glyphWidth, height)
			currentX = currentX + glyphWidth
		end
	end
end

local textures

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		-- enable texture rendering
		gl.Enable(gl_enum.GL_TEXTURE_2D)

		-- load image file
		font.texture = texture.load("../../common/assets/bitmap_font.png", true)

		-- enable blending function for alpha transparency
   		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
   		gl.BlendEquation(gl_enum.GL_FUNC_ADD)
   		gl.BlendColor(1,1,1,1)
   		gl.Enable(gl_enum.GL_BLEND)

		gl.ClearColor(0.3,0.3,0.3,1)
	end,
	idle = function()
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		drawText(font, 'Hello world', -0.75, -0.125, 1.5, 0.25)
		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
		font.texture.delete()
	end,
}

app.main(window, events)

