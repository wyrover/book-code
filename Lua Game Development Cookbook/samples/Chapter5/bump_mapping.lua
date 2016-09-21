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

local vectorModule = require 'utils/vector'
local vector = vectorModule.new

local camera = require 'utils/camera'
local box = require 'shapes/box'
local quad = require 'shapes/quad'
local axis = require 'shapes/axis'
local lightsModule = require 'utils/lights'

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

local fontColor = {r=1, g=1, b=1, a=1}

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local scene = {}

local lights
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

local function prepareLightShader()
	-- prepare shaders
	shaders.bumpmap = shader.load('../../common/shaders/bump_mapping')

	-- ### Font shader setup
	shaders.bumpmap.use()
	shaders.bumpmap.uniform.define {
		{name = 'sceneAmbient', type = {'f', 4}},
		{name = 'diffuseTexture', type = {'i'}},
		{name = 'normalTexture', type = {'i'}},
		
		{name = 'camera.view', type = {'f', 4, 4, true}},
		{name = 'camera.perspective', type = {'f', 4, 4, false}},

		{name = 'object.view', type = {'f', 4, 4, true}},
	
		{name = 'totalLights', type = {'i'}},

		{name = 'debug', type = {'i'}},
		{name = 'debugColor', type = {'f', 4}},
		{name = 'normalScale', type = {'f', 3}},

		{name = 'material.ambient', type = {'f', 4}},
		{name = 'material.diffuse', type = {'f', 4}},
		{name = 'material.specular', type = {'f', 4}},
		{name = 'material.shininess', type = {'f'}},
	}

	lights = lightsModule.init(shaders.bumpmap, lightDefs)

	local activeTextureUnit = 0

	shaders.bumpmap.uniform({
		['sceneAmbient'] = {0.1, 0.1, 0.1, 1},
		['diffuseTexture'] = activeTextureUnit,
		['normalTexture'] = activeTextureUnit,

		['debug'] = 0,
		['debugColor'] = {1,1,1,1},
		['normalScale'] = {1,1,1},

		['camera.view'] = matrix(),
		['camera.perspective'] = matrix(),
		['object.view'] = matrixBase.translate(0, 0, 0),
		
		['material.ambient'] = {0.2, 0.2, 0.2, 1},
		['material.diffuse'] = {1, 1, 1, 1},
		['material.specular'] = {1, 1, 1, 1},
		['material.shininess'] = 5.2,

	})

	-- bind vertex attributes
	shaders.bumpmap.bind {
		[0] = 'VertexPosition',
		[1] = 'VertexTexCoord',
		[2] = 'VertexNormal',
		[3] = 'VertexTangent',
	}
end

local function prepareScene()
	prepareLightShader()
	scene.box = box.new()
	scene.quad = quad.new()
	scene.axis = axis.new()
end

local switchMode = 0

local function switchNormals()
	local u = shaders.bumpmap.uniform
	shaders.bumpmap.use()
	switchMode = (switchMode + 1) % 8

	if switchMode == 0 then
		u['normalScale'] = {1,  1,  1}
	elseif switchMode == 1 then
		u['normalScale'] = {1,  1,  -1}
	elseif switchMode == 2 then
		u['normalScale'] = {1, -1,  1}
	elseif switchMode == 3 then
		u['normalScale'] = {1, -1,  -1}
	elseif switchMode == 4 then
		u['normalScale'] = {-1, 1,  1}
	elseif switchMode == 5 then
		u['normalScale'] = {-1, 1,  -1}
	elseif switchMode == 6 then
		u['normalScale'] = {-1, -1,  1}
	elseif switchMode == 7 then
		u['normalScale'] = {-1, -1,  -1}
	end
	print('Normal map mode: ', vector(3, u['normalScale']))

end

local angle = 0
local movementScale = 1.0

-- textured box matrix
local objectMatrix = matrixBase.translate(0, 0, 0)

local function drawScene(dt)
	gl.Enable(gl_enum.GL_CULL_FACE)
	gl.Enable(gl_enum.GL_DEPTH_TEST)

	angle = math.fmod(angle + 1 * dt * movementScale, 360)
	
	shaders.bumpmap.use()

	--[[
		Set up textures into texture units:
		Texture unit 0 - diffuse texture
		Texture unit 1 - normal map texture
	--]]
	textures.rocks.use(0)
	textures.rocks_normalmap.use(1)

	-- draw axis
	scene.axis.draw(shaders.bumpmap)

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

	-- slowly rotate textured box - 10 degrees per second with normal movementScale
	objectMatrix = objectMatrix * matrixBase.rotate(0, 1, 0, math.rad(10 * dt * movementScale))

	shaders.bumpmap.uniform {
		['object.view'] = objectMatrix,
		['diffuseTexture'] = 0,
		['normalTexture'] = 1,
	}

	scene.box.draw(shaders.bumpmap)

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

		gl.CullFace(gl_enum.GL_FRONT)

		--textures.rocks = texture.load('../../common/assets/uv_checker.png')
		--textures.rocks_normalmap = texture.load('../../common/assets/normal_map.jpg')
		
		textures.rocks = texture.load('../../common/assets/rock.png')
		textures.rocks_normalmap = texture.load('../../common/assets/rock_n.png')
		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)

		prepareScene()
		cameras.camera0 = camera.new(screenAspectRatio, keyStates, mouseState, shaders.bumpmap)
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

		printLeftAligned(-1, 0.9, {r=1,g=1,b=1,a=1}, 0.0008, "Bump mapping")
		printLeftAligned(-1, 0.8, {r=1,g=1,b=1,a=1}, 0.0005, 'Press spacebar to pause or unpause movement')
		printLeftAligned(-1, 0.7, {r=1,g=1,b=1,a=1}, 0.0005, 'Use W, S, A, D keys and mouse to move around')
		printLeftAligned(-1, 0.6, {r=1,g=1,b=1,a=1}, 0.0005, 'Press 1 to change normal mapping mode')

		local Co = cameras.camera0.orientationVector
		local Cp = cameras.camera0.position

		printLeftAligned(-0.9, 0.5, {r=1,g=1,b=1,a=1}, 0.0004, ('Orientation: (%4.4f %4.4f %4.4f %4.4f)'):format(Co[1], Co[2], Co[3], Co[4]))
		printLeftAligned(-0.9, 0.44, {r=1,g=1,b=1,a=1}, 0.0004, ('Position: (%4.4f %4.4f %4.4f)'):format(Cp[1], Cp[2], Cp[3]))

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
		elseif keySym == SDL.SDLK_1 then
			switchNormals()
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

