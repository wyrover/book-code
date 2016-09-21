local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local shader = require 'SDL/shader'
local vertexBuffer = require 'SDL/vertex_buffer'
local vertexArray = require 'SDL/vertex_array'
local ttf = require 'SDL/fonts/ttf'

local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local camera = require 'utils/camera'

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

local buffers = {}
local shaders = {}
local fonts = {}
local textures = {}
local vertexArrays = {}

local lightDefaults = {
	position = {0,0,0,1},
	diffuse = {1,0.8,0.8,1},
	specular = {1,1,1,1},
	spotCutoff = 100,
	spotExponent = 1.2,
	constantAttenuation = 0,
	linearAttenuation = 1,
	quadraticAttenuation = 0,
	spotDirection = {0,0,0},
}

local scene = {}
local lights = {}

local lightDefs = {
	{
		diffuse = {1,0.6,0.4,1},
	},
	{
		diffuse = {0.2,0.3,1,1},
		spotCutoff = 45,
		spotExponent = 3,
		linearAttenuation = 1,
		quadraticAttenuation = 1,
	},
	{
		diffuse = {1,1,1,1},
		spotCutoff = 5,
		spotExponent = 8,
		linearAttenuation = 1,
		quadraticAttenuation = 1,
	},
}

local function defineLight(index)
	assert(type(index)=='number' and index>=0)

	local prefix = ('lights[%d].'):format(index)

	shaders.light.uniform.define {
		{name = prefix..'position', type = {'f', 4}},
		{name = prefix..'diffuse', type = {'f', 4}},
		{name = prefix..'specular', type = {'f', 4}},
		{name = prefix..'spotCutoff', type = {'f'}},
		{name = prefix..'spotExponent', type = {'f'}},
		{name = prefix..'constantAttenuation', type = {'f'}},
		{name = prefix..'linearAttenuation', type = {'f'}},
		{name = prefix..'quadraticAttenuation', type = {'f'}},
		{name = prefix..'spotDirection', type = {'f', 3}},
	}
end

local function setLight(index, lightDefinition)
	local prefix = ('lights[%d].'):format(index)

	local paramTable = {}
	for k,v in pairs(lightDefaults) do
		local value = lightDefinition[k] or v
	    lightDefinition[k] = value
		paramTable[prefix..k] = value
	end

	shaders.light.uniform(paramTable)

	local lightObject = {
		matrix = matrix(),
	}

	setmetatable(lightObject, {
		__index = function(t,k)
			return lightDefinition[k]
		end,
		__newindex = function(t, k, v)
		    shaders.light.uniform[prefix..k] = v
			lightDefinition[k] = v
		end,
		-- draw a light box
		__call = function(t)
			shaders.light.uniform['object.translation'] = t.matrix * matrixBase.scale(0.1, 0.1, 0.1)
			shaders.light.uniform['object.rotation'] = matrix()
			shaders.light.uniform['debug'] = 1
			shaders.light.uniform['debugColor'] = lightDefinition.diffuse

			scene.box.draw()
		
			shaders.light.uniform['debug'] = 0
		end,
	})
	return lightObject
end

local function prepareLightShader()
	-- prepare shaders
	shaders.light = shader.load('../../common/shaders/light')

	-- ### Font shader setup
	shaders.light.use()
	shaders.light.uniform.define {
		{name = 'sceneAmbient', type = {'f', 4}},
		{name = 'diffuseTexture', type = {'i'}},
		
		{name = 'camera.view', type = {'f', 4, 4, true}},
		{name = 'camera.perspective', type = {'f', 4, 4, false}},

		{name = 'object.translation', type = {'f', 4, 4, true}},
		{name = 'object.rotation', type = {'f', 4, 4, true}},
	
		{name = 'totalLights', type = {'i'}},

		{name = 'debug', type = {'i'}},
		{name = 'debugColor', type = {'f', 4}},

		{name = 'material.ambient', type = {'f', 4}},
		{name = 'material.diffuse', type = {'f', 4}},
		{name = 'material.specular', type = {'f', 4}},
		{name = 'material.shininess', type = {'f'}},
	}

	-- define light parameters types
	for i=0,#lightDefs-1 do
		defineLight(i)
	end

	local activeTextureUnit = 0

	shaders.light.uniform({
		['sceneAmbient'] = {0.1, 0.1, 0.1, 1},
		['diffuseTexture'] = activeTextureUnit,
		['totalLights'] = #lightDefs,
		['debug'] = 0,
		['debugColor'] = {1,1,1,1},

		['camera.view'] = matrix(),
		['camera.perspective'] = matrix(),
		['object.translation'] = matrixBase.translate(0, 0, 0),
		['object.rotation'] = matrix(),
		
		['material.ambient'] = {0.2, 0.2, 0.2, 1},
		['material.diffuse'] = {1, 1, 1, 1},
		['material.specular'] = {1, 1, 1, 1},
		['material.shininess'] = 5.0,

	})

	for i,lightParameters in ipairs(lightDefs) do
		lights[i] = setLight(i-1, lightParameters)
	end

	-- bind vertex attributes
	shaders.light.bind {
		[0] = 'VertexPosition',
		[1] = 'VertexTexCoord',
		[2] = 'VertexNormal',
	}
end

local function prepareBox()
	local buffers = {}
	-- prepare vertex buffers
	buffers.vertex = vertexBuffer.new()
	buffers.texCoord = vertexBuffer.new()
	buffers.normals = vertexBuffer.new()

	-- fills vertex buffers with data
	-- x, y
	buffers.vertex.data {
		-- top
		-0.5,	0.5,	-0.5,
		-0.5,	0.5,	0.5,
		0.5,	0.5,	0.5,
		0.5,	0.5,	-0.5,
		-- bottom
		-0.5,	-0.5,	-0.5,
		0.5,	-0.5,	-0.5,
		0.5,	-0.5,	0.5,
		-0.5,	-0.5,	0.5,
		-- front
		-0.5,	0.5,	0.5,
		-0.5,	-0.5,	0.5,
		0.5,	-0.5,	0.5,
		0.5,	0.5,	0.5,
		-- back
		-0.5,	0.5,	-0.5,
		0.5,	0.5,	-0.5,
		0.5,	-0.5,	-0.5,
		-0.5,	-0.5,	-0.5,
		-- left
		-0.5,	0.5,	-0.5,
		-0.5,	-0.5,	-0.5,
		-0.5,	-0.5,	0.5,
		-0.5,	0.5,	0.5,
		-- right
		0.5,	-0.5,	-0.5,
		0.5,	0.5,	-0.5,
		0.5,	0.5,	0.5,
		0.5,	-0.5,	0.5,
	}

	-- u, v
	buffers.texCoord.data {
	    -- top
		0,	0,
		0,	1,
		1,	1,
		1,	0,
	    -- bottom
		1,	1,
		0,	1,
		0,	0,
		1,	0,
	    -- front
		0,	0,
		0,	1,
		1,	1,
		1,	0,
	    -- back
		1,	0,
		0,	0,
		0,	1,
		1,	1,
	    -- left
		0,	0,
		0,	1,
		1,	1,
		1,	0,
	    -- right
		0,	1,
		1,	1,
		1,	0,
		0,	0,
	}

	-- x, y, z
	buffers.normals.data {
		-- top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		-- bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		-- front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		-- back
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		-- left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-- right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
	}

	-- prepare vertex array
	local box = vertexArray.new()
	box.use()
	-- maps vertex buffer to vertex attributes with these parameters (attribute_index, vertex_buffer_object, elements_per_primitive)
	box.map(0, buffers.vertex, 3)
	box.map(1, buffers.texCoord, 2)
	box.map(2, buffers.normals, 3)

	-- enable all vertex attributes
	box.enableAttrib(0)
	box.enableAttrib(1)
	box.enableAttrib(2)

	return {
		buffers = buffers,
		box = box,
		draw = function()
			box.draw(gl_enum.GL_QUADS, 0, 4*6)	
		end,
	}
end

local function prepareScene()
	prepareLightShader()
	scene.box = prepareBox()
end

local fontColor = {r=1, g=1, b=1, a=1}

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local angle = 0
local movementScale = 1.0

local objectMatrix = matrixBase.translate(0, 0, 0)

local function drawScene(dt)
	gl.Enable(gl_enum.GL_CULL_FACE)
	gl.Enable(gl_enum.GL_DEPTH_TEST)

	angle = math.fmod(angle + 1 * dt * movementScale, 360)
	
	shaders.light.use()

	textures.texture1.use(0)

	-- light 1
	lights[1].matrix = matrixBase.rotateZ(angle) * matrixBase.translate(0, 1, 0)
	lights[1].position = lights[1].matrix * {0,0,0,1}
	-- draw a light box 
	lights[1]()

	-- light 2
	lights[2].matrix = matrixBase.rotateY(angle) * matrixBase.translate(0, 0, -1)
	lights[2].position = lights[2].matrix * {0,0,0,1}
	local spotDirection = lights[2].matrix * {0,0,-1,0}
	lights[2].spotDirection = {spotDirection[1], spotDirection[2], spotDirection[3]}
	-- draw a light box 
	lights[2]()

	-- light 3
	lights[3].matrix = matrixBase.rotate(1,0,-1,math.rad(45)) * matrixBase.translate(0, math.sin(-angle) + 1.8, 0)
	lights[3].position = lights[3].matrix * {0,0,0,1}
	local spotDirection = lights[3].matrix * {0, 1, 0, 0}
	lights[3].spotDirection = {spotDirection[1], spotDirection[2], spotDirection[3]}
	-- draw a light box 
	lights[3]()

	shaders.light.uniform['object.translation'] = matrix()
	shaders.light.uniform['object.rotation'] = matrix()

	scene.box.draw()

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

local mouseState = {
	centerX = window.width/2, 
	centerY = window.height/2,
	relativeX = 0,
	relativeY = 0,
	speedMultiplier = {
		x = 1.0,
		y = 1.0,
	},
}

-- set up initial time for movement
local time = os.clock()
local paused = false
local cameras = {}

local firstMovement = true

local function mouseMotionHandler(event_struct)
	local event = event_struct.motion
	SDL.SDL_WarpMouse(mouseState.centerX, mouseState.centerY)
	if not firstMovement then
		mouseState.relativeX, mouseState.relativeY = event.x - mouseState.centerX, event.y - mouseState.centerY
	else
		mouseState.relativeX, mouseState.relativeY = 0, 0
		firstMovement = false
	end

	local currentTime = os.clock()
	local dt
	if paused then
		dt = 0
	else	
		dt = currentTime-time
	end

	cameras.camera0.updateViewOrientation(dt)

	time = currentTime
end
local events
events = {
	startup = function()
		SDL.TTF_Init()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()
		assert(gl.IsSupported("GL_VERSION_3_0 GL_ARB_vertex_shader GL_ARB_fragment_shader"), "Your GPU doesn't support GLSL 3.3")

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		-- enable blending
   		gl.Enable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
		gl.BlendEquation(gl_enum.GL_FUNC_ADD)

		gl.ClearColor(0.1, 0.1, 0.1, 1)

		textures.texture1 = texture.load('../../common/assets/lena_std.png')
		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)

		prepareScene()
		cameras.camera0 = camera.new(screenAspectRatio, keyStates, mouseState, shaders.light)
		cameras.camera0.init()

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

		cameras.camera0.update(dt)
		drawScene(dt)

		printLeftAligned(-1, 0.9, {r=1,g=1,b=1,a=1}, 0.0008, "Lighting")
		printLeftAligned(-1, 0.8, {r=1,g=1,b=1,a=1}, 0.0005, 'Press spacebar to pause or unpause movement')
		printLeftAligned(-1, 0.7, {r=1,g=1,b=1,a=1}, 0.0005, 'Use W, S, A, D keys and mouse to move around')

		local Co = cameras.camera0.orientationVector
		local Cp = cameras.camera0.position

		printLeftAligned(-0.9, 0.6, {r=1,g=1,b=1,a=1}, 0.0004, ('Orientation: (%4.4f %4.4f %4.4f %4.4f)'):format(Co[1], Co[2], Co[3], Co[4]))
		printLeftAligned(-0.9, 0.54, {r=1,g=1,b=1,a=1}, 0.0004, ('Position: (%4.4f %4.4f %4.4f)'):format(Cp[1], Cp[2], Cp[3]))

		time = currentTime

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	[SDL.SDL_ACTIVEEVENT] = function(event_struct)
		local event = event_struct.active
		if SDL.And(event.state, SDL.SDL_APPINPUTFOCUS) > 0 then
			if event.gain == 1 then
				SDL.SDL_WarpMouse(mouseState.centerX, mouseState.centerY)
				events[SDL.SDL_MOUSEMOTION] = mouseMotionHandler
			else
				events[SDL.SDL_MOUSEMOTION] = false
			end
		end
	end,
	[SDL.SDL_MOUSEMOTION] = mouseMotionHandler,
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

