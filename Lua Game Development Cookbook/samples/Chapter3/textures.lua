local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'

local function createTexture()
	local texture_id = (gl.GenTextures(1))[1]
	gl.BindTexture(gl_enum.GL_TEXTURE_2D, texture_id)
	
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_WRAP_S, gl_enum.GL_CLAMP_TO_EDGE)
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_WRAP_T, gl_enum.GL_CLAMP_TO_EDGE)
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_MIN_FILTER, gl_enum.GL_LINEAR)
	gl.TexParameteri(gl_enum.GL_TEXTURE_2D, gl_enum.GL_TEXTURE_MAG_FILTER, gl_enum.GL_LINEAR)

	return texture_id
end

local function createEmptyTexture(width, height)
    local texture_id = createTexture()

	local texture_target = gl_enum.GL_TEXTURE_2D
	local mipmap_level = 0
	local internal_format = gl_enum.GL_RGBA8
	local border = 0
	local format = gl_enum.GL_RGBA
	local data = 0
	gl.TexImage2D(texture_target, mipmap_levels, internal_format, width, height, border, format, data)
	gl.Finish()

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, 0)
	return texture_id
end

local function createTextureFromImage(surface)
    local texture_id = createTexture()
    local rawSurfaceObject = surface.raw
    local bpp = rawSurfaceObject.format.BitsPerPixel

	local texture_target = gl_enum.GL_TEXTURE_2D
	local mipmap_level = 0
	local internal_format = gl_enum.GL_RGBA
	local width = rawSurfaceObject.w
	local height = rawSurfaceObject.h
	local border = 0
    local format

	-- determine surface image data format
	if bpp == 24 then
    	format = gl_enum.GL_RGB
    elseif bpp == 32 then
    	format = gl_enum.GL_RGBA
    else
    	error('Unsupported image format')
    end

	local data = rawSurfaceObject.pixels
	gl.TexImage2D(texture_target, mipmap_levels, internal_format, width, height, border, format, data)
	gl.Finish()

	gl.BindTexture(gl_enum.GL_TEXTURE_2D, 0)
	return texture_id
end

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

local texture_id

local aspect_ratio = window.width/window.height

local rectangle = {
	x = -0.5,
	y = -0.5,
	width = 1 ,
	height = aspect_ratio,
}

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		-- enable texture rendering
		gl.Enable(gl_enum.GL_TEXTURE_2D)

		-- load image file
		local image = surface.wrap(assert(SDL.IMG_Load("../../common/assets/lena_std.png")))
		texture_id = createTextureFromImage(image)
		-- sets current texture
		gl.BindTexture(gl_enum.GL_TEXTURE_2D, texture_id)
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))
		-- draws textured rectangle
		gl.Begin(gl_enum.GL_QUADS)
			gl.Color4f(1, 1, 1, 1)
			gl.TexCoord2f(0, 1)
			gl.Vertex2f(rectangle.x, rectangle.y)
			gl.TexCoord2f(1, 1)
			gl.Vertex2f(rectangle.x + rectangle.width, rectangle.y)
			gl.TexCoord2f(1, 0)
			gl.Vertex2f(rectangle.x + rectangle.width, rectangle.y + rectangle.height)
			gl.TexCoord2f(0, 0)
			gl.Vertex2f(rectangle.x, rectangle.y + rectangle.height)
		gl.End()
		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
		gl.DeleteTextures(gl_enum.GL_TEXTURE_2D, {texture_id})
	end,
}

app.main(window, events)

