local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
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

local screenAspectRatio = window.width/window.height
local fonts = {}
local fontColor = {r=1, g=1, b=1, a=1}
local fontSizeFactor = 0.10
local sphere
local lightAngle = 0
local movementScale = 1.5

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local function setupMaterial()
	local m_diffuse =	{0.9, 0.9, 0.9, 1.0}
	local m_ambient =	{0.1, 0.1, 0.1, 1.0}
	local m_specular =	{1.0, 1.0, 1.0, 1.0}
	local m_emission =	{0.0, 0.0, 0.0, 1.0}
	local m_shininess =	50
	local m_side = gl_enum.GL_FRONT

	gl.Materialfv(m_side, gl_enum.GL_DIFFUSE, m_diffuse)
	gl.Materialfv(m_side, gl_enum.GL_AMBIENT, m_ambient)
	gl.Materialfv(m_side, gl_enum.GL_SPECULAR, m_specular)
	gl.Materialfv(m_side, gl_enum.GL_EMISSION, m_emission)

	gl.Materialf(m_side, gl_enum.GL_SHININESS, m_shininess)
end

local function setupLighting(lightNumber, diffuseColor)
	local lightNumber = lightNumber or 0
	-- light color settings
	local light_source =	gl_enum.GL_LIGHT0 + lightNumber
	local light_diffuse =	diffuseColor
	local light_specular =	{1, 1, 1, 1}
	local light_ambient =	{0, 0, 0, 1}

	-- light position and light type settings
	local light_spot_direction =		{0, 0, 0}
	local light_attenuation_constant =	1
	local light_attenuation_linear =	0.04
	local light_attenuation_quadratic =	0.08
	local light_spot_cutoff =			120

	gl.Lightfv(light_source, gl_enum.GL_DIFFUSE, light_diffuse)
	gl.Lightfv(light_source, gl_enum.GL_SPECULAR, light_specular)
	gl.Lightfv(light_source, gl_enum.GL_AMBIENT, light_ambient)

	gl.Lightfv(light_source, gl_enum.GL_SPOT_DIRECTION, light_spot_direction)
	gl.Lightf(light_source, gl_enum.GL_CONSTANT_ATTENUATION, light_attenuation_constant)
	gl.Lightf(light_source, gl_enum.GL_LINEAR_ATTENUATION, light_attenuation_linear)
	gl.Lightf(light_source, gl_enum.GL_QUADRATIC_ATTENUATION, light_attenuation_quadratic)
	gl.Lightf(light_source, gl_enum.GL_SPOT_CUTOFF, light_spot_cutoff)

	return {
		enable = function()
			gl.Enable(light_source)
		end,
		disable = function()
			gl.Disable(light_source)
		end,
		setPosition = function(x,y,z)
			gl.Lightfv(light_source, gl_enum.GL_POSITION, {x, y, z, 1})
		end,
		-- returns function that changes light position automatically
		updatePosition = function(dt)
			lightAngle = math.fmod(lightAngle + 45 * dt * movementScale, 360)
			local lightDistance = 0.5
			local lightX = lightDistance * math.cos(math.rad(lightAngle))
			local lightY = 0.2 
			local lightZ = lightDistance * math.sin(math.rad(lightAngle))

			local light_position =	{lightX, lightY, lightZ, 1.0}
			gl.Lightfv(light_source, gl_enum.GL_POSITION, light_position)
		end
	}
end

local function drawSphere()
	local radius = 0.3
	local slices = 100
	local stacks = 100
	gl.Sphere(sphere, radius, slices, stacks)
end

local lights = {}

local function drawScene(dt)
	-- turn on camera perspective
	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()
	gl.Perspective(60, screenAspectRatio, 0.01, 10)
	gl.Translatef(0, 0, -1.5)

	gl.MatrixMode(gl_enum.GL_MODELVIEW)
	gl.LoadIdentity()

	setupMaterial()

	for _, light in ipairs(lights) do
		light.enable()
	end

	-- set light positions
	lights[1].updatePosition(dt)
	lights[2].setPosition(0.5, -0.2, 0.5)

	gl.Enable(gl_enum.GL_LIGHTING)

	drawSphere()

	gl.Disable(gl_enum.GL_LIGHTING)

	for _, light in ipairs(lights) do
		light.disable()
	end

	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()
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

		-- enable blending
   		gl.Enable(gl_enum.GL_BLEND)
		gl.BlendFunc(gl_enum.GL_SRC_ALPHA, gl_enum.GL_ONE_MINUS_SRC_ALPHA)
		gl.BlendEquation(gl_enum.GL_FUNC_ADD)

		-- enable z-buffer test to clip invisible back-faces
		gl.Enable(gl_enum.GL_DEPTH_TEST)

		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)

		-- Prepare Quadric object for a sphere
		sphere = gl.NewQuadric()

		gl.QuadricNormals(sphere, gl_enum.GLU_SMOOTH)
		gl.QuadricOrientation(sphere, gl_enum.GLU_OUTSIDE)

		-- set up smooth shading
		gl.ShadeModel(gl_enum.GL_SMOOTH)

		-- set up two lights
		lights[1] = setupLighting(0, {1,0.6,0.4,1})
		lights[2] = setupLighting(1, {0.2,0.3,1,1})
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
		printLeftAligned(-0.95, 0.9, {r=1,g=1,b=1,a=1}, 0.15, 'Two lights one ball')
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

