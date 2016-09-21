local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'
local ttf = require 'SDL/fonts/ttf'

local unpack = table.unpack or unpack

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
local fontColor = {r=0, g=0, b=0, a=1}
local fontSizeFactor = 0.60

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local textures = {}

local orthogonalMode = true

local function computeFrustum(fov, screen_aspect_ratio, zNear, zFar)
	-- tangent value refers to the half of the size of
	-- viewable screen portion
	local tangent = math.tan(math.rad(fov/2))
	local bottom = tangent*zNear
	local top = -bottom
	local right = bottom*screen_aspect_ratio
	local left = -right
	return left, right, bottom, top, zNear, zFar
end

local function drawBox(color)
	textures.texture1.use()

	gl.Begin(gl_enum.GL_QUADS)
		gl.Color4f(unpack(color))
		-- top
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(-0.5, 0.5, -0.5)
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(-0.5, 0.5, 0.5)
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(0.5, 0.5, 0.5)
		gl.TexCoord2f(1, 0)
		gl.Vertex3f(0.5, 0.5, -0.5)
		-- bottom
		gl.Vertex3f(-0.5, -0.5, -0.5)
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(-0.5, -0.5, 0.5)
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(0.5, -0.5, 0.5)
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(0.5, -0.5, -0.5)
		gl.TexCoord2f(0, 0)
		-- front
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(-0.5, 0.5, 0.5)
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(-0.5, -0.5, 0.5)
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(0.5, -0.5, 0.5)
		gl.TexCoord2f(1, 0)
		gl.Vertex3f(0.5, 0.5, 0.5)
		-- back
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(-0.5, 0.5, -0.5)
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(-0.5, -0.5, -0.5)
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(0.5, -0.5, -0.5)
		gl.TexCoord2f(1, 0)
		gl.Vertex3f(0.5, 0.5, -0.5)
		-- left
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(-0.5, 0.5, -0.5)
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(-0.5, -0.5, -0.5)
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(-0.5, -0.5, 0.5)
		gl.TexCoord2f(1, 0)
		gl.Vertex3f(-0.5, 0.5, 0.5)
		-- right
		gl.TexCoord2f(0, 0)
		gl.Vertex3f(0.5, 0.5, -0.5)
		gl.TexCoord2f(0, 1)
		gl.Vertex3f(0.5, -0.5, -0.5)
		gl.TexCoord2f(1, 1)
		gl.Vertex3f(0.5, -0.5, 0.5)
		gl.TexCoord2f(1, 0)
		gl.Vertex3f(0.5, 0.5, 0.5)
	gl.End()

	textures.texture1.unUse()
end

local function drawAxis()
	gl.Begin(gl_enum.GL_LINES)
		gl.Color4f(1,0,0,1)
		gl.Vertex3f(0,0,0)
		gl.Vertex3f(1,0,0)

		gl.Color4f(0,1,0,1)
		gl.Vertex3f(0,0,0)
		gl.Vertex3f(0,1,0)

		gl.Color4f(0,0,1,1)
		gl.Vertex3f(0,0,0)
		gl.Vertex3f(0,0,1)
	gl.End()
end

local angle = 0
local movementScale = 2.0

local function drawScene(dt)
	local screenAspectRatio = window.width/window.height
	local zNear, zFar = 0.01, 100
	angle = math.fmod(angle + 45 * dt * movementScale, 360)

	-- enable z-buffer test to clip invisible back-faces
	gl.Enable(gl_enum.GL_DEPTH_TEST)

	-- turn into projection mode to setup camera
	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()

	if orthogonalMode then
		local left, right = -screenAspectRatio, screenAspectRatio
		local top, bottom = 1, -1

		gl.Ortho(left, right, bottom, top, zNear, zFar)
	else
		local fov = 60

		gl.Perspective(fov, screenAspectRatio, zNear, zFar)

		-- you can do the same with gl.Frustum and computeFrustum function
		-- gl.Frustum(computeFrustum(fov, screenAspectRatio, zNear, zFar))
	end

	-- move camera above the scene and rotate around it
	gl.Translatef(0, 0.2, -1.5)
	gl.Rotatef(45, 1, 0, 0)
	gl.Rotatef(angle, 0, 1, 0)

	-- turn into Modelview space when drawing scene objects
	gl.MatrixMode(gl_enum.GL_MODELVIEW)

	-- draw axis
	gl.LoadIdentity()
	gl.Scalef(0.1, 0.1, 0.1)
	drawAxis()

	-- draw 3 boxes
	gl.LoadIdentity()

	gl.PushMatrix()
		gl.Translatef(0, 0, -0.6)
		gl.Scalef(0.5, 0.5, 0.5)
		drawBox({0.25,0.25,1,1})
	gl.PopMatrix()

	gl.PushMatrix()
		gl.Translatef(0, 0, 0.4)
		gl.Scalef(0.25, 0.25, 0.25)
		drawBox({1, 0.25, 0.25, 1})
	gl.PopMatrix()

	gl.PushMatrix()
		gl.Translatef(0.3, 0, 0.4)
		gl.Scalef(0.15, 0.15, 0.15)
		drawBox({0.25, 1, 0.25, 1})
	gl.PopMatrix()

	-- reset into original projection mode
	gl.MatrixMode(gl_enum.GL_PROJECTION)
	gl.LoadIdentity()

	gl.Disable(gl_enum.GL_DEPTH_TEST)
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

		fonts.font1 = ttf.load('../../common/assets/DejaVuSans.ttf', 128)
		textures.texture1 = texture.load('../../common/assets/uv_checker.png', false)
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
		printLeftAligned(-0.95, 0.9, {r=1,g=1,b=1,a=1}, 0.15, (orthogonalMode and 'Orthogonal mode') or 'Perspective mode')
		printLeftAligned(-0.95, 0.8, {r=1,g=1,b=1,a=1}, 0.10, 'Press Enter key to switch between perspective and orthogonal camera mode')
		printLeftAligned(-0.95, 0.7, {r=1,g=1,b=1,a=1}, 0.10, 'Press spacebar to pause or unpause movement')

		time = currentTime

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	[SDL.SDL_KEYDOWN] = function(event_struct) 
		local event = event_struct.key
		local keySym = event.keysym.sym
		if keySym == SDL.SDLK_RETURN then
			orthogonalMode = not orthogonalMode
		elseif keySym == SDL.SDLK_SPACE then
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

