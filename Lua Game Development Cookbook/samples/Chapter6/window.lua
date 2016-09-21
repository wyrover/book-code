local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local shader = require 'SDL/shader'
local ttf = require 'SDL/fonts/ttf'

local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local quad = require 'shapes/quad_gui'

-- useful shortcuts for matrix transformations
local T,S = matrixBase.translate, matrixBase.scale

local window = {
	x = 512,
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

local shaders = {}
local fonts = {}
local textures = {}
local shapes = {}
local windows = {}

local focusedWindow = false
local draggingWindow = false
local windowOverCursor = false

local windowSpaceMatrix

local windowMovementStatus = {
	startX = 0,
	startY = 0,
}

local fontColor = {r=1, g=1, b=1, a=1}

-- Window event handlers
local eventHandlers = {}

local function handleEvent(name, ...)
	local handlers = eventHandlers[name]
	if type(handlers)=="table" then
		for _, handler in ipairs(handlers) do
			if type(handler)=="function" then
				handler(...)
			end
		end
	end
end

local function addHandler(name, fn)
	local handlers = eventHandlers[name] or {}
	table.insert(handlers, fn)
	eventHandlers[name] = handlers
end

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local function prepareGUIshader()
	local shaderObject = shader.load('../../common/shaders/gui')
	shaders.gui = shaderObject

	shaderObject.use()

	local uniforms = shaderObject.uniform

	uniforms.define {
		{name = 'baseColor', type = {'f', 4}},
		{name = 'diffuseTexture', type = {'i'}},
		{name = 'modelViewMatrix', type = {'f', 4, 4, true}},
		{name = 'projectionMatrix', type = {'f', 4, 4, true}},
		{name = 'texMatrix', type = {'f', 3, 3, true}},
		{name = 'textured', type = {'i'}},
	}



	local maxDepth = 1/16378

	--[[
	You need to convert viewport coordinates into OpenGL default screen-space.
	It's done by using inverted transformation matrix where you actually reverse whole process. Therefore, matrix transformations need to be done in reverse order.

	-1, 1                 1, 1     -1, -1                 1, -1      0, 0                 2, 0       0, 0                 1, 0       0, 0                 width, 0
		+-----------------+				+-----------------+				+-----------------+				+-----------------+				+-----------------+
		|                 |             |                 |         	|                 |             |                 |             |                 |
		|        1.       |    --->     |        2.       |    ---> 	|        3.       |     --->    |        4.       |    --->     |        5.       |
		|                 |             |                 |         	|                 |             |                 |             |                 |
		+-----------------+				+-----------------+				+-----------------+				+-----------------+				+-----------------+
	-1, -1                1, -1     -1, 1                 1, 1       0, 2                 2, 2       0, 1                 1, 1       0, height            width, height



	--]]

	windowSpaceMatrix = (S(window.width, window.height, maxDepth) * S(1/2, 1/2, 1) * T(1, 1, 0) * S(1, -1, 1) ).inv()

	uniforms {
		baseColor = {1,1,1,1},
		diffuseTexture = 0,
		modelViewMatrix = matrix(),
		projectionMatrix = windowSpaceMatrix,
		texMatrix = {
			1,0,0,
			0,1,0,
			0,0,1,
		},
	}

	-- bind vertex attributes
	shaderObject.bind {
		[0] = 'VertexPosition',
		[1] = 'VertexTexCoord',
	}
end

local function createWindow(windowDef)
	local windowObject = windowDef or {}
	local shaderObject = shaders.gui
	local uniforms = shaderObject.uniform
	local modelViewMatrix
	local tempMatrix

	-- default values
	windowDef.originX = windowDef.originX or 0.5
	windowDef.originY = windowDef.originY or 0.5
	windowDef.color = windowDef.color or {0.2,0.3,0.5,1}

	if type(windowDef.visible) ~= 'boolean' then
		windowDef.visible = true
	end

	windowObject.update = function()
		-- this matrix will be useful later
		tempMatrix = S(windowDef.width, windowDef.height, 1)
			* T(windowDef.originX, windowDef.originY, 0)

		modelViewMatrix = T(windowDef.x, windowDef.y, 0)
			* tempMatrix
	end

	windowObject.draw = function()
		if windowDef.visible then
			shaderObject.use()
			uniforms {
				modelViewMatrix = modelViewMatrix,
				baseColor = windowDef.color,
				textured = (windowDef.textured and 1) or 0,
			}
			shapes.quad.draw()
		end
	end

	windowObject.hasFocus = function()
		return focusedWindow == windowObject
	end

	windowObject.relativeCoordinates = function(x, y)
		return modelViewMatrix.inv() * {x, y, 0, 1}
	end

	windowObject.projectCursorToWindow = function(x, y)
		local relativeMouseCoords = windowObject.relativeCoordinates(x, y)

		local mouseCoordsOnWindow = tempMatrix * relativeMouseCoords

		return mouseCoordsOnWindow[1], mouseCoordsOnWindow[2]
	end

	windowObject.isMouseOverWindow = function(x, y)
		local relativeMouseCoords = windowObject.relativeCoordinates(x, y)
		local wX, wY = relativeMouseCoords[1], relativeMouseCoords[2]
		return (wX<=0.5 and wX>=-0.5 and wY<=0.5 and wY>=-0.5)
	end

	windowObject.update()

	return windowObject
end

local function prepareGUIhandlers()
	addHandler('mouseButtonDown', function(x, y, button)
		for name, window in pairs(windows) do
			-- left mouse button uses number 1 identifier
			if button==1 and window.isMouseOverWindow(x, y) and (draggingWindow ~= window) then
			    local projectedX, projectedY = window.projectCursorToWindow(x, y)

				windowMovementStatus.startX = projectedX
				windowMovementStatus.startY = projectedY
				draggingWindow = window
			end
		end
	end)

	addHandler('mouseMove', function(x, y)
		if draggingWindow then
		    local projectedX, projectedY = draggingWindow.projectCursorToWindow(x, y)

		    local movementMatrix = T(
				projectedX - windowMovementStatus.startX,
				projectedY - windowMovementStatus.startY,
				0
			)

			local newWindowPosition = movementMatrix * {draggingWindow.x, draggingWindow.y, 0, 1}
			draggingWindow.x = newWindowPosition[1]
			draggingWindow.y = newWindowPosition[2]

			-- update modelview matrix for selected window
			draggingWindow.update()
		end

		windowOverCursor = false

		for name, window in pairs(windows) do
			if window.isMouseOverWindow(x, y) then
		    	windowOverCursor = window
		    	break
		    end
		end
	end)

	addHandler('mouseButtonUp', function(x, y, button)
		if button==1 and draggingWindow then
			draggingWindow = false
		end
		for name, window in pairs(windows) do
			if button==1 and window.isMouseOverWindow(x, y) and not window.hasFocus() then
				focusedWindow = window
			end			
		end
	end)
end

local function prepareScene()
	prepareGUIshader()
	shapes.quad = quad.new()

	prepareGUIhandlers()

	windows.window1 = createWindow {
		x = 320,
		y = 240,
		width = 320,
		height = 240,
		color = {0.2,0.3,0.5,1},
	}

	windows.window2 = createWindow {
		x = 0,
		y = 0,
		width = 100,
		height = 100,
		color = {0.5,0.3,0.2,1},
	}

	windows.window3 = createWindow {
		x = 32,
		y = 200,
		width = 200,
		height = 100,
		color = {0.2,0.5,0.3,1},
	}
end

local function drawScene(dt)
	gl.Enable(gl_enum.GL_CULL_FACE)
	gl.Enable(gl_enum.GL_DEPTH_TEST)

	for name, window in pairs(windows) do
		window.draw()
	end

	gl.Disable(gl_enum.GL_DEPTH_TEST)
	gl.Disable(gl_enum.GL_CULL_FACE)
end

local keyStates = {}
local keyModStates = {}

-- a list of key modifiers paired with their scan codes
local modKeysScanCodes = {
	[SDL.KMOD_LSHIFT]=SDL.SDLK_LSHIFT, [SDL.KMOD_RSHIFT]=SDL.SDLK_RSHIFT,
	[SDL.KMOD_LCTRL]=SDLK_LCTRL, [SDL.KMOD_RCTRL]=SDL.SDLK_RCTRLT,
	[SDL.KMOD_LALT]=SDL.SDLK_LALT, [SDL.KMOD_RALT]=SDL.SDLK_RALT,
	[SDL.KMOD_LMETA]=SDL.SDLK_LMETA, [SDL.KMOD_RMETA]=SDL.SDLK_RMETA,
	[SDL.KMOD_NUM]=SDL.SDLK_NUMLOCK,
	[SDL.KMOD_CAPS]=SDL.SDLK_CAPSLOCK,
	[SDL.KMOD_MODE]=SDL.SDLK_MODE,
}

local function processModKeys()
	local modState = SDL.SDL_GetModState()
	for keyMod, keySym in pairs(modKeysScanCodes) do
		-- apply binary and operator to obtain modifier key state
		keyModStates[keySym] = (bit.band(modState, keyMod) > 0)
	end
end

-- set up initial time for movement
local time = os.clock()
local paused = false

local events
events = {
	startup = function()
		SDL.TTF_Init()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()
		assert(gl.IsSupported("GL_VERSION_3_0 GL_ARB_vertex_shader GL_ARB_fragment_shader"), "Your GPU doesn't support GLSL 3.3")

		-- show mouse cursor
		SDL.SDL_ShowCursor(1)

		-- enable blending
   		gl.Enable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
		gl.BlendEquation(gl_enum.GL_FUNC_ADD)

		gl.ClearColor(0.1, 0.1, 0.1, 1)

		gl.CullFace(gl_enum.GL_FRONT)

		textures.rocks = texture.load('../../common/assets/rock.png')
		textures.rocks_normalmap = texture.load('../../common/assets/rock_n.png')
		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)

		prepareScene()
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		processModKeys()

		-- movement is time dependent
		local currentTime = os.clock()
		local dt
		if paused then
			dt = 0
		else	
			dt = currentTime-time
		end

		drawScene(dt)

		time = currentTime

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	[SDL.SDL_ACTIVEEVENT] = function(event_struct)
		local event = event_struct.active
		--[[
		if SDL.And(event.state, SDL.SDL_APPINPUTFOCUS) > 0 then
			if event.gain == 1 then
				SDL.SDL_WarpMouse(mouseState.centerX, mouseState.centerY)
				events[SDL.SDL_MOUSEMOTION] = mouseMotionHandler
			else
				events[SDL.SDL_MOUSEMOTION] = false
			end
		end
		--]]
	end,
	
	[SDL.SDL_MOUSEMOTION] = function(event_struct)
		local event = event_struct.motion
		handleEvent('mouseMove', event.x, event.y)
	end,
	[SDL.SDL_MOUSEBUTTONDOWN] = function(event_struct)
    	local event = event_struct.button
		handleEvent('mouseButtonDown', event.x, event.y, event.button)
	end,
	[SDL.SDL_MOUSEBUTTONUP] = function(event_struct)
		local event = event_struct.button
		handleEvent('mouseButtonUp', event.x, event.y, event.button)
	end,

	[SDL.SDL_KEYDOWN] = function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
		keyStates[keySym] = true

		if keySym == SDL.SDLK_SPACE then
			paused = not paused
	    -- send SDL_QUIT event on Escape key
		elseif keySym == SDL.SDLK_ESCAPE then
			local event = SDL.SDL_Event_local()
			event.type = SDL.SDL_QUIT
			SDL.SDL_PushEvent(event)
		end
	end,
	[SDL.SDL_KEYUP] = function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
		keyStates[keySym] = false
	end,
	cleanup = function()
		fonts.font1.delete()
		SDL.TTF_Quit()
	end,
}

app.main(window, events)

