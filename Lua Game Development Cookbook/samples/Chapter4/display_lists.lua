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
local displayLists = {}
local movementScale = 2.5
local angle = 0

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local function drawBox()
	gl.Begin(gl_enum.GL_QUADS)
		-- top
		gl.Vertex3f(-0.5, 0.5, -0.5)
		gl.Vertex3f(-0.5, 0.5, 0.5)
		gl.Vertex3f(0.5, 0.5, 0.5)
		gl.Vertex3f(0.5, 0.5, -0.5)
		-- bottom
		gl.Vertex3f(-0.5, -0.5, -0.5)
		gl.Vertex3f(-0.5, -0.5, 0.5)
		gl.Vertex3f(0.5, -0.5, 0.5)
		gl.Vertex3f(0.5, -0.5, -0.5)
		-- front
		gl.Vertex3f(-0.5, 0.5, 0.5)
		gl.Vertex3f(-0.5, -0.5, 0.5)
		gl.Vertex3f(0.5, -0.5, 0.5)
		gl.Vertex3f(0.5, 0.5, 0.5)
		-- back
		gl.Vertex3f(-0.5, 0.5, -0.5)
		gl.Vertex3f(-0.5, -0.5, -0.5)
		gl.Vertex3f(0.5, -0.5, -0.5)
		gl.Vertex3f(0.5, 0.5, -0.5)
		-- left
		gl.Vertex3f(-0.5, 0.5, -0.5)
		gl.Vertex3f(-0.5, -0.5, -0.5)
		gl.Vertex3f(-0.5, -0.5, 0.5)
		gl.Vertex3f(-0.5, 0.5, 0.5)
		-- right
		gl.Vertex3f(0.5, 0.5, -0.5)
		gl.Vertex3f(0.5, -0.5, -0.5)
		gl.Vertex3f(0.5, -0.5, 0.5)
		gl.Vertex3f(0.5, 0.5, 0.5)
	gl.End()
end

local function createDisplayList()
	local displayListID = gl.GenLists(1)
	local displayList = {}
	local deleted = false

	displayList.wrap = function(fn, mode)
		local mode = mode or gl_enum.GL_COMPILE
		assert(type(fn)=='function')
		gl.NewList(displayListID, mode)
			fn()
		gl.EndList()
	end

	displayList.call = function()
		gl.CallList(displayListID)
	end

	displayList.delete = function()
		if not deleted then
			gl.DeleteLists({displayListID}, 1)
			deleted = true
		end
	end

	setmetatable(displayList, {
		__gc = displayList.delete,
	})
	table.proxy(displayList)
	return displayList
end

local function prepareScene()
	local displayList = displayLists[1]
	
	displayList.wrap(function()
		drawBox()
	end)
end

local function drawScene(dt)
	local displayList = displayLists[1]
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

	local boxSize = 0.05
	local count = 50
	local maxDistance = 1.5

	angle = math.fmod(angle + 45 * dt * movementScale, 360)

	-- draw moving spiral made of small boxes
	for i=0,count-1 do
		local iterationFloat = i/(count-1)
		local distance = maxDistance*(math.cos(math.rad(angle))+1)*0.5

	    -- the first part
		gl.LoadIdentity()
		
		local x = distance*math.cos(math.rad(iterationFloat*360 + angle))
		local y = distance*math.sin(math.rad(iterationFloat*360 + angle))
		local z = -i*(boxSize+0.01)

		gl.Translatef(x, y, z)
		gl.Scalef(boxSize, boxSize, boxSize)

		local r,g,b = colors.hsl2rgb(iterationFloat, 1, 0.5)
		gl.Color4f(r, g, b, 1)
		-- draw a box by using display list
	    displayList.call()

	    -- the second part
		local distance = maxDistance*(1-(math.cos(math.rad(angle))+1)*0.5)
		gl.LoadIdentity()
		
		local x = -distance*math.cos(math.rad(iterationFloat*360 + angle))
		local y = distance*math.sin(math.rad(iterationFloat*360 + angle))
		local z = -i*(boxSize+0.01)

		gl.Translatef(x, y, z)
		gl.Scalef(boxSize, boxSize, boxSize)

		local r,g,b = colors.hsl2rgb(1-iterationFloat, 1, 0.5)
		gl.Color4f(r, g, b, 1)
		-- draw a box by using display list
	    displayList.call()

	    -- the third part
		local distance = maxDistance*((math.cos(math.rad(angle))+1)*0.5)
		gl.LoadIdentity()
		
		local x = -distance*math.cos(math.rad(iterationFloat*360 + angle))
		local y = -distance*math.sin(math.rad(iterationFloat*360 + angle))
		local z = -i*(boxSize+0.01)

		gl.Translatef(x, y, z)
		gl.Scalef(boxSize, boxSize, boxSize)

		local r,g,b = colors.hsl2rgb(1-iterationFloat, 1, 0.5)
		gl.Color4f(r, g, b, 1)
		-- draw a box by using display list
	    displayList.call()

	    -- the fourth part
		local distance = maxDistance*(1-(math.cos(math.rad(angle))+1)*0.5)
		gl.LoadIdentity()
		
		local x = distance*math.cos(math.rad(iterationFloat*360 + angle))
		local y = -distance*math.sin(math.rad(iterationFloat*360 + angle))
		local z = -i*(boxSize+0.01)

		gl.Translatef(x, y, z)
		gl.Scalef(boxSize, boxSize, boxSize)

		local r,g,b = colors.hsl2rgb(1-iterationFloat, 1, 0.5)
		gl.Color4f(r, g, b, 1)
		-- draw a box by using display list
	    displayList.call()
	end

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

		displayLists[1] = createDisplayList()

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
		printLeftAligned(-0.95, 0.9, {r=1,g=1,b=1,a=1}, 0.15, 'Display lists')
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

