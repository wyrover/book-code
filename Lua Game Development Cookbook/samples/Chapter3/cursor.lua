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
	bpp = 0,
	flags = SDL.SDL_OPENGL,
	caption = 'PackPub Example',
	class = 'Example',
}

local mouseCursor = {
	x = 0, y = 0,
	width = 32/window.width,
	height = 32/window.height,
}

local events = {
	[SDL.SDL_MOUSEMOTION] = function(event_struct)
		local event = event_struct.motion
		mouseCursor.x = (event.x / window.width) * 2 - 1
		mouseCursor.y = -((event.y / window.height) * 2 - 1) - mouseCursor.height
	end,
	startup = function()
		local screen = surface.wrap(window.screen, false)
		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)
		
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))
		-- draws a white square over mouse cursor position
		gl.Begin(gl_enum.GL_QUADS)
			gl.Color4f(1, 1, 1, 1)
			gl.TexCoord2f(0, 0)
			gl.Vertex2f(mouseCursor.x, mouseCursor.y)
			gl.TexCoord2f(1, 0)
			gl.Vertex2f(mouseCursor.x + mouseCursor.width, mouseCursor.y)
			gl.TexCoord2f(1, 1)
			gl.Vertex2f(mouseCursor.x + mouseCursor.width, mouseCursor.y + mouseCursor.height)
			gl.TexCoord2f(0, 1)
			gl.Vertex2f(mouseCursor.x, mouseCursor.y + mouseCursor.height)
		gl.End()
		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
}

app.main(window, events)

