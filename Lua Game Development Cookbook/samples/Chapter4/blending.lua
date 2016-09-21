local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local ttf = require 'SDL/fonts/ttf'

local window = {
	x = 128,
	y = 128,
	width = 1024,
	height = 786,
	bpp = 32,
	flags = SDL.SDL_OPENGL,
	caption = 'PackPub Example',
	class = 'Example',
	fullScreen = false,
}

if window.fullScreen then
	window.flags = bit.bor(window.flags, SDL.SDL_FULLSCREEN)
end

local fonts = {}
local fontColor = {r=1, g=1, b=1, a=1}
local fontSizeFactor = 0.10

local textures = {}

local function blendingSample(title, code)
	return function(x, y, width, height)
		code(x, y, width, height)
	
		-- enable blending function for alpha transparency
   		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
   		gl.BlendEquation(gl_enum.GL_FUNC_ADD)
   		gl.BlendColor(1,1,1,1)

		local xOffset = (#title)*fontSizeFactor/7
		fonts.font1.print(x - xOffset, y+height/2, color, fontSizeFactor, title)
	end
end

local function drawQuad(x,y, width, height)
	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(1, 1, 1, 1)
		-- A
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.TexCoord2f(1, 0)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- D
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end

local additiveBlending = blendingSample('Additive blending', function(x, y, width, height)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE)
	gl.BlendEquation(gl_enum.GL_FUNC_ADD)

	textures.texture1.use()
	drawQuad(x,y, width, height)

	textures.texture2.use()
	drawQuad(x,y, width, height)

	textures.texture2.unUse()
end)

local subtractiveBlending = blendingSample('Subtractive blending', function(x, y, width, height)
	gl.BlendFunc(gl_enum.GL_ONE_MINUS_SRC_COLOR, gl_enum.GL_SRC_COLOR)
	gl.BlendEquation(gl_enum.GL_FUNC_ADD)

	textures.texture1.use()
	drawQuad(x,y, width, height)

	textures.texture2.use()
	drawQuad(x,y, width, height)

	textures.texture2.unUse()
end)

local alphaBlending = blendingSample('Alpha blending', function(x, y, width, height)
	gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
	gl.BlendEquation(gl_enum.GL_FUNC_ADD)

	textures.texture1.use()
	drawQuad(x,y, width, height)

	textures.texture2.use()
	drawQuad(x,y, width, height)

	textures.texture2.unUse()
end)

local events = {
	startup = function()
		SDL.TTF_Init()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		gl.ClearColor(0.1,0.1,0.1,1)

		gl.Enable(gl_enum.GL_TEXTURE_2D)

		-- enable blending
   		gl.Enable(gl_enum.GL_BLEND)

		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)
		textures.texture1 = texture.load('../../common/assets/lena_std.png')
		textures.texture2 = texture.load('../../common/assets/Lua-Logo_2.png', true)
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		additiveBlending(-0.6, 0, 0.5, 0.5)
		subtractiveBlending(0, 0, 0.5, 0.5)
		alphaBlending(0.6, 0, 0.5, 0.5)

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
	fonts.font1.delete()
		SDL.TTF_Quit()
	end,
}

app.main(window, events)

