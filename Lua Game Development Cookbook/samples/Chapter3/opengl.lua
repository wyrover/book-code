local app = require 'sdl_app'
local gl = require 'luagl'
require 'GL/defs'
local bit = require 'bit'
local surface = require 'SDL/surface'

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

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()
		-- forces OpenGL to use 32-bit color mode
		SDL.SDL_GL_SetAttribute(SDL.SDL_GL_RED_SIZE, 8)
		SDL.SDL_GL_SetAttribute(SDL.SDL_GL_GREEN_SIZE, 8)
		SDL.SDL_GL_SetAttribute(SDL.SDL_GL_BLUE_SIZE, 8)
		SDL.SDL_GL_SetAttribute(SDL.SDL_GL_ALPHA_SIZE, 8)
		-- depth buffer (Z-buffer) will use 16-bit precission
		SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DEPTH_SIZE, 16)
		-- forces to use double-buffering
		SDL.SDL_GL_SetAttribute(SDL.SDL_GL_DOUBLEBUFFER, 1)

		-- displays information about current GPU
		print(([[
GPU info:
	OpenGL version: %s
	Vendor: %s
	Renderer: %s
	Extensions: %s
]]):format(
			gl.Get("s(1)", gl_enum.GL_VERSION),
			gl.Get("s(1)", gl_enum.GL_VENDOR),
			gl.Get("s(1)", gl_enum.GL_RENDERER),
			gl.Get("s(1)", gl_enum.GL_EXTENSIONS)
		))

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))
		-- draws colored triangle
		gl.Begin(gl_enum.GL_TRIANGLES)
			gl.Color4f(1, 0, 0, 1)
			gl.Vertex2f(0, 1)
			gl.Color4f(0, 1, 0, 1)
			gl.Vertex2f(-1, -1)
			gl.Color4f(0, 0, 1, 1)
			gl.Vertex2f(1, -1)
		gl.End()
		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
}

app.main(window, events)

