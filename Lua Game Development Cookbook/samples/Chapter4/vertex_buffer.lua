local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local ttf = require 'SDL/fonts/ttf'
local colors = require 'utils/colors'

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

local screenAspectRatio = window.width/window.height
local fonts = {}
local fontColor = {r=1, g=1, b=1, a=1}
local fontSizeFactor = 0.10
local buffers = {}
local movementScale = 2.5
local angle = 0

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local function createBufferObject(usage)
	local bufferUsage = usage or gl_enum.GL_STATIC_DRAW
	local bufferObjectID = (gl.GenBuffers(1))[1]
	local bufferObject = {}
	local deleted = false
	local bufferTarget = gl_enum.GL_ARRAY_BUFFER

	local function use ()
		gl.BindBuffer(bufferTarget, bufferObjectID)
	end

	bufferObject.use = use

	bufferObject.unUse = function()
		gl.BindBuffer(bufferTarget, 0)
	end

	-- reserve memory for buffer object
	bufferObject.reserve = function(size, usage)
		local usage = usage or bufferUsage
		use()
		gl.BufferData(bufferTarget, size, usage)
	end

	-- set data for whole buffer object
	bufferObject.data = function(data, usage)
		local usage = usage or bufferUsage
		use()
		gl.BufferData(bufferTarget, data, usage)
	end

	-- set data for a part of buffer object
	bufferObject.subData = function(offset, data, usage)
		local usage = usage or bufferUsage
		use()
		gl.BufferSubData(bufferTarget, offset, data, usage)
	end

	bufferObject.delete = function()
		if not deleted then
			gl.DeleteBuffers({bufferObjectID})
			deleted = true
		end
	end

	setmetatable(bufferObject, {
		__gc = bufferObject.delete,
	})
	table.proxy(bufferObject)
	return bufferObject
end

local hsl2rgb = colors.hsl2rgb
local totalLength = 10
local parts = 128

-- generate values for a pair of vertices
local function vertexGenerator(i)
	local iterationFloat = i/(parts-1)

	local x = (iterationFloat-0.5)*totalLength
	local y = math.sin(iterationFloat*math.pi*2)
	local z = -0.5

	local r,g,b = hsl2rgb(iterationFloat, 1, 0.5)
	local a = 1

	return
		x, y, z,
		x, y, z + 1,
		r, g, b, a,
		r, g, b, a
end

-- fills vertexData and colorData tables with vertex values at specified offset
local function setPartData(vertexData, colorData, offset, i, generator)
	-- vertex coordinates
	vertexData[offset*6 + 1],
	vertexData[offset*6 + 2],
	vertexData[offset*6 + 3],

	vertexData[offset*6 + 4],
	vertexData[offset*6 + 5],
	vertexData[offset*6 + 6],

	-- vertex colors
	colorData[offset*8 + 1],
	colorData[offset*8 + 2],
	colorData[offset*8 + 3],
	colorData[offset*8 + 4],

	colorData[offset*8 + 5],
	colorData[offset*8 + 6],
	colorData[offset*8 + 7],
	colorData[offset*8 + 8]
		= generator(i)
end

local function initializeBuffers()
	local vertices = buffers.vertices
	local colors = buffers.colors

	local vertexData = {}
	local colorData = {}

	for i=0,parts-1 do
		setPartData(vertexData, colorData, i, i, vertexGenerator)
	end

	-- write values into buffers
	vertices.subData(0, vertexData)
	colors.subData(0, colorData)
end

local function prepareScene()
	buffers.vertices = createBufferObject()
	buffers.colors = createBufferObject()
	local vertices = buffers.vertices
	local colors = buffers.colors

	-- each vertex will use 3 components: x, y, z
	vertices.reserve(parts*3*2)
	-- each color will use 4 components: r, g, b, a
	colors.reserve(parts*4*2)
	initializeBuffers()
end

local function drawScene(dt)
	gl.Disable(gl_enum.GL_TEXTURE_2D)
	
	-- enable z-buffer test to clip invisible back-faces
	gl.Enable(gl_enum.GL_DEPTH_TEST)

	-- turn on camera perspective
	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()
	gl.Perspective(60, screenAspectRatio, 0.01, 10)
	gl.Translatef(0, -0.2, -2.5)
	gl.Rotatef(25, 1, 0, 0)

	gl.MatrixMode(gl_enum.GL_MODELVIEW)
	gl.LoadIdentity()
	-- scale down object a bit
	gl.Scalef(0.25, 0.5, 1)

	-- enable vertex buffer for vertices and colors
	gl.EnableClientState(gl_enum.GL_VERTEX_ARRAY)
	gl.EnableClientState(gl_enum.GL_COLOR_ARRAY)

	-- apply vertex buffer for vertices
	buffers.vertices.use()
	gl.VertexPointer(3)

	-- apply vertex buffer for colors
	buffers.colors.use()
	gl.ColorPointer(4)

	-- actually draw specified vertices
	gl.DrawArrays(gl_enum.GL_QUAD_STRIP, 0, parts*2)

	gl.DisableClientState(gl_enum.GL_VERTEX_ARRAY)
	gl.DisableClientState(gl_enum.GL_COLOR_ARRAY)

	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()
	
	gl.Disable(gl_enum.GL_DEPTH_TEST)
	gl.Enable(gl_enum.GL_TEXTURE_2D)
end

-- set up initial time for movement
local time = os.clock()
local paused = false

local events = {
	startup = function()
		SDL.TTF_Init()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()
		assert(gl.IsSupported("GL_ARB_vertex_buffer_object"), "Vertex Buffer Object is not supported on your GPU")

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		gl.ClearColor(0.1,0.1,0.1,1)

		gl.Enable(gl_enum.GL_TEXTURE_2D)
		gl.Enable(gl_enum.GL_COLOR_MATERIAL)

		-- enable blending
   		gl.Enable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
		gl.BlendEquation(gl_enum.GL_FUNC_ADD)

		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)

		prepareScene()
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		-- movement is time dependent
		local currentTime = os.clock()
		local dt
		if paused then
			dt = 0
		else	
			dt = currentTime-time
		end

		drawScene(dt)

		-- reset GL_MODELVIEW matrix into original form so that the text is always in the top left corner of the screen
		gl.MatrixMode(gl_enum.GL_MODELVIEW)
		gl.LoadIdentity()
		printLeftAligned(-0.95, 0.9, {r=1,g=1,b=1,a=1}, 0.15, 'Vertex buffer objects')
		printLeftAligned(-0.95, 0.8, {r=1,g=1,b=1,a=1}, 0.10, 'Press spacebar to pause or unpause movement')

		time = currentTime

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	[SDL.SDL_KEYDOWN] = function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
		if keySym == SDL.SDLK_SPACE then
			paused = not paused
	    -- send SDL_QUIT event on Escape key
		elseif keySym == SDL.SDLK_ESCAPE then
			local event = SDL.SDL_Event_local()
			event.type = SDL.SDL_QUIT
			SDL.SDL_PushEvent(event)
		end
	end,
	cleanup = function()
	fonts.font1.delete()
		SDL.TTF_Quit()
	end,
}

app.main(window, events)

