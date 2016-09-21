local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
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

local function primitiveSample(title, code)
	return function(x, y, width, height)
		gl.Disable(gl_enum.GL_TEXTURE_2D)
		
		code(x, y, width, height)
	
		gl.Enable(gl_enum.GL_TEXTURE_2D)

		local xOffset = (#title)*fontSizeFactor/7
		fonts.font1.print(x - xOffset, y+height, color, fontSizeFactor, title)
	end
end


local drawPoints = primitiveSample('GL_POINTS', function(x, y, width, height)
	gl.Begin(gl_enum.GL_POINTS)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- D
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end)

local drawLines = primitiveSample('GL_LINES', function(x, y, width, height)
	gl.Begin(gl_enum.GL_LINES)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- D
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end)

local drawLineStrip = primitiveSample('GL_LINE_STRIP', function(x, y, width, height)
	gl.Begin(gl_enum.GL_LINE_STRIP)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- D
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end)

local drawLineLoop = primitiveSample('GL_LINE_STRIP', function(x, y, width, height)
	gl.Begin(gl_enum.GL_LINE_LOOP)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- D
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end)

local drawTriangles = primitiveSample('GL_TRIANGLES', function(x, y, width, height)
	gl.Begin(gl_enum.GL_TRIANGLES)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x, y + height/2, 0)
	gl.End()
end)

local drawTriangleStrip = primitiveSample('GL_TRIANGLE_STRIP', function(x, y, width, height)
	gl.Begin(gl_enum.GL_TRIANGLE_STRIP)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/4, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/4, y - height/2, 0)
		-- C
		gl.Color4f(1, 1, 1, 1)
		gl.Vertex3f(x - width/4, y, 0)
		-- D
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/4, y, 0)
		-- E
		gl.Color4f(1, 0, 1, 1)
		gl.Vertex3f(x - width/4, y + height/2, 0)
		-- F
		gl.Color4f(0, 1, 1, 1)
		gl.Vertex3f(x + width/4, y + height/2, 0)
		-- G
		gl.Color4f(1, 1, 1, 1)
		gl.Vertex3f(x - width/4, y + height, 0)
	gl.End()
   	
   	gl.LineWidth(1)

	gl.Begin(gl_enum.GL_LINE_STRIP)
		-- A
		gl.Color4f(0, 0, 0, 1)
		gl.Vertex3f(x - width/4, y - height/2, 0)
		-- B
		gl.Vertex3f(x + width/4, y - height/2, 0)
		-- C
		gl.Vertex3f(x - width/4, y, 0)
		-- D
		gl.Vertex3f(x + width/4, y, 0)
		-- E
		gl.Vertex3f(x - width/4, y + height/2, 0)
		-- F
		gl.Vertex3f(x + width/4, y + height/2, 0)
		-- G
		gl.Vertex3f(x - width/4, y + height, 0)
	gl.End()

	gl.LineWidth(3)
end)

local drawTriangleFan = primitiveSample('GL_TRIANGLE_FAN', function(x, y, width, height)
	gl.Begin(gl_enum.GL_TRIANGLE_FAN)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x, y, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x, y - height/1.5, 0)
		-- C
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- D
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/1.5, y, 0)
		-- E
		gl.Color4f(1, 0, 1, 1)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- F
		gl.Color4f(0, 1, 1, 1)
		gl.Vertex3f(x, y + height/1.5, 0)
		-- G
		gl.Color4f(1, 1, 1, 1)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end)

local drawQuad = primitiveSample('GL_QUADS', function(x, y, width, height)
	gl.Begin(gl_enum.GL_QUADS)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/2, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/2, y - height/2, 0)
		-- C
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x + width/2, y + height/2, 0)
		-- D
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x - width/2, y + height/2, 0)
	gl.End()
end)

local drawQuadStrip = primitiveSample('GL_QUAD_STRIP', function(x, y, width, height)
	gl.Begin(gl_enum.GL_QUAD_STRIP)
		-- A
		gl.Color4f(1, 0, 0, 1)
		gl.Vertex3f(x - width/4, y - height/2, 0)
		-- B
		gl.Color4f(0, 1, 0, 1)
		gl.Vertex3f(x + width/4, y - height/2, 0)
		-- C
		gl.Color4f(1, 1, 0, 1)
		gl.Vertex3f(x - width/2, y, 0)
		-- D
		gl.Color4f(0, 0, 1, 1)
		gl.Vertex3f(x + width/2, y, 0)
		-- E
		gl.Color4f(1, 0, 1, 1)
		gl.Vertex3f(x - width/4, y + height/2, 0)
		-- F
		gl.Color4f(0, 1, 1, 1)
		gl.Vertex3f(x + width/4, y + height/2, 0)
	gl.End()
end)

local events = {
	startup = function()
		SDL.TTF_Init()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		-- enable blending function for alpha transparency
   		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
   		gl.BlendEquation(gl_enum.GL_FUNC_ADD)
   		gl.BlendColor(1,1,1,1)
   		gl.Enable(gl_enum.GL_BLEND)

   		-- enable point primitive antialias
   		gl.Enable(gl_enum.GL_POINT_SMOOTH)
   		-- set point primitive size
   		gl.PointSize(7)

   		-- enable line smoothing
   		gl.Enable(gl_enum.GL_LINE_SMOOTH)

   		-- set line width
   		gl.LineWidth(3)

		gl.ClearColor(0.1,0.1,0.1,1)

		gl.Enable(gl_enum.GL_TEXTURE_2D)

		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		drawPoints(-0.7, 0.6, 0.25, 0.25)
		drawLines(-0.1, 0.6, 0.25, 0.25)
		drawLineStrip(0.6, 0.6, 0.25, 0.25)

		drawLineLoop(-0.7, -0.1, 0.25, 0.25)
		drawTriangles(-0.1, -0.1, 0.25, 0.25)
		drawTriangleStrip(0.6, -0.1, 0.25, 0.25)

		drawTriangleFan(-0.7, -0.75, 0.25, 0.25)
		drawQuad(-0.1, -0.75, 0.25, 0.25)
		drawQuadStrip(0.6, -0.75, 0.25, 0.25)

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
	fonts.font1.delete()
		SDL.TTF_Quit()
	end,
}

app.main(window, events)

