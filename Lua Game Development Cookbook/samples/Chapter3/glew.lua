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
		-- don't forget to initialize GLEW
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
Supported features:
	GLSL Shading language 3.3: %s
	GLSL Shading language 4.0: %s
	GLSL Shading language 4.2: %s
]]):format(
			tostring(gl.IsSupported("GL_VERSION_3_3 GL_ARB_vertex_shader GL_ARB_fragment_shader GL_ARB_geometry_shader4")),
			tostring(gl.IsSupported("GL_VERSION_4_0 GL_ARB_vertex_shader GL_ARB_fragment_shader GL_ARB_geometry_shader4 GL_ARB_tessellation_shader")),
			tostring(gl.IsSupported("GL_VERSION_4_2 GL_ARB_vertex_shader GL_ARB_fragment_shader GL_ARB_geometry_shader4 GL_ARB_tessellation_shader GL_ARB_compute_shader"))
		))
	end,
}

app.main(window, events)

