local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
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

local centerX, centerY = window.width/2, window.height/2
local relativeX, realtiveY, totalX, totalY = 0,0,0,0

-- mouse movement sensitivity
local mouseSensitivityX = 4.0
local mouseSensitivityY = 4.0

local fonts = {}
local fontColor = {r=0, g=0, b=0, a=1}
local fontSizeFactor = 0.60

local solarSystemScale = 0.0008
local speedScale = 10000.0 -- base unit days/s

-- scaling factors to make solar system visible ;)
local sunScale = 0.005
local invSunScale = 1/sunScale
local earthDiameter = 112737
-- 1 mil. km / Earth diameter
local distanceMultiplier = (10^6)/earthDiameter
local moonDistanceMultiplier = distanceMultiplier*invSunScale*0.2

local stars = {
	{name='Sun', size = 109125.11*sunScale, color = {1,1,0,1},
		planets = {
			{name='Mercury', distance = 58*distanceMultiplier, outerSpeed = 1/87.97, innerSpeed = 1/58.65, size = 0.38*invSunScale, color = {0.9,0.9,0.9,1},},
			{name='Venus', distance = 108*distanceMultiplier, outerSpeed = 1/224.70, innerSpeed = -1/243.02, size = 0.95*invSunScale, color = {0.8,0.5,0.6,1},},
			{name='Earth', distance = 150*distanceMultiplier, outerSpeed = 1/365.26, innerSpeed = 1, size = 1.0*invSunScale, color = {0.2,0.7,1,1},
				moons = {
					{name='The moon', distance = 0.4*moonDistanceMultiplier, outerSpeed = -1/28.5, innerSpeed = -1/28.5, size = 0.13*invSunScale, color = {0.9,0.9,0.9,1},},
				},
			},
			{name='Mars', distance = 227*distanceMultiplier, outerSpeed = 1/686.97, innerSpeed = 1/1.03, size = 0.53*invSunScale, color = {0.8,0.5,0.1,1},
				moons = {
					{name='Phobos', distance = 0.2*moonDistanceMultiplier, outerSpeed = -1/2, innerSpeed = -1, size = 0.02*invSunScale, color = {0.9,0.9,0.9,1},},
					{name='Deimos', distance = 0.3*moonDistanceMultiplier, outerSpeed = -1/1.5, innerSpeed = -1, size = 0.01*invSunScale, color = {0.7,0.7,0.9,1},},
				},
			},
			{name='Jupiter', distance = 779*distanceMultiplier, outerSpeed = 1/4332.59, innerSpeed = 1/0.41, size = 11.20*invSunScale, color = {0.9,0.5,0.2,1},},
			{name='Saturn', distance = 1428*distanceMultiplier, outerSpeed = 1/10759.22, innerSpeed = 1/0.44, size = 9.45*invSunScale, color = {0.9,0.8,0.6,1},},
			{name='Uranus', distance = 2974*distanceMultiplier, outerSpeed = 1/30687.15, innerSpeed = 1/0.72, size = 4.00*invSunScale, color = {0.5,0.7,0.9,1},},
			{name='Neptune', distance = 4506*distanceMultiplier, outerSpeed = 1/60190.03, innerSpeed = 1/0.67, size = 3.88*invSunScale, color = {0.4,0.8,0.9,1},},
		},
	},
}

local diskRadius = 0.5
local diskSlices = 20

local function printCentered(x, y, color, size, text)
	local xOffset = (#text)*size/7
	fonts.font1.print(x-xOffset, y-size/2, color, size, text)
end

local function printLeftAligned(x, y, color, size, text)
	fonts.font1.print(x, y-size/2, color, size, text)
end

local function drawObject(name, color)
	local sliceAngle = 2*math.pi/(diskSlices-1)

	gl.Begin(gl_enum.GL_TRIANGLE_FAN)
		gl.Color4f(unpack(color))
		gl.Vertex2f(0, 0)

		for i=0,diskSlices-1 do
			local x = diskRadius*math.cos(sliceAngle*i)
			local y = diskRadius*math.sin(sliceAngle*i)
			gl.Vertex2f(x, y)
		end
	gl.End()

	if type(name)=='string' then
		printCentered(0, 0, {r=0,g=0,b=0,a=0.8}, fontSizeFactor, name)
		printCentered(-0.03, 0.03, {r=1,g=1,b=1,a=1}, fontSizeFactor, name)
	end
end

local function drawSolarSystem(systemScale, dt)
	-- convert total mouse cursor coordinates into window space
	local systemOffsetX = mouseSensitivityX*(totalX/window.width)
	local systemOffsetY = -mouseSensitivityY*(totalY/window.height)

	gl.MatrixMode(gl_enum.GL_MODELVIEW)
	for _, star in ipairs(stars) do
		gl.LoadIdentity()
		
		-- move whole solar system
		gl.Translatef(systemOffsetX, systemOffsetY, 0)
		-- do solar system scaling
		gl.Scalef(systemScale, systemScale, 1)

		-- draw a star
		gl.PushMatrix()
			gl.Scalef(star.size, star.size, 1)
			drawObject(star.name, star.color)
		gl.PopMatrix()
		
		if type(star.planets)=='table' then
			for _, planet in ipairs(star.planets) do
				-- draw a planet
				gl.PushMatrix()
					planet.outerAngle = math.fmod((planet.outerAngle or 0) + planet.outerSpeed*dt, 360)
					planet.innerAngle = math.fmod((planet.innerAngle or 0) + planet.innerSpeed*dt, 360)

					gl.Rotatef(planet.outerAngle, 0, 0, 1)
					gl.Translatef(planet.distance, 0, 0)
					gl.PushMatrix()
						gl.Rotatef(planet.innerAngle, 0, 0, 1)
						gl.Scalef(planet.size, planet.size, 1)
						drawObject(planet.name, planet.color)
					gl.PopMatrix()

					if type(planet.moons)=='table' then
						for _, moon in ipairs(planet.moons) do
							-- draw a moon
							gl.PushMatrix()
								moon.outerAngle = math.fmod((moon.outerAngle or 0) + moon.outerSpeed*dt, 360)
								moon.innerAngle = math.fmod((moon.innerAngle or 0) + moon.innerSpeed*dt, 360)

								gl.Rotatef(moon.outerAngle, 0, 0, 1)
								gl.Translatef(moon.distance, 0, 0)
								gl.PushMatrix()
									gl.Rotatef(moon.innerAngle, 0, 0, 1)
									gl.Scalef(moon.size, moon.size, 1)
									drawObject(moon.name, moon.color)
								gl.PopMatrix()
							gl.PopMatrix()
						end
					end
				gl.PopMatrix()
	    	end
		end
	end
end

local function mouseMotionHandler(event_struct)
	local event = event_struct.motion
	SDL.SDL_WarpMouse(centerX, centerY)
	relativeX, relativeY = event.x - centerX, event.y - centerY
	totalX, totalY = totalX + relativeX, totalY + relativeY
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

		drawSolarSystem(solarSystemScale, dt*speedScale)

		-- reset GL_MODELVIEW matrix into original form so that the text is always in the top left corner of the screen
		gl.LoadIdentity()
		printLeftAligned(-0.95, 0.9, {r=1,g=1,b=1,a=1}, 0.15, 'Solar system')
		printLeftAligned(-0.95, 0.8, {r=1,g=1,b=1,a=1}, 0.10, 'Use mouse cursor and scroll button to move the screen')
		printLeftAligned(-0.95, 0.7, {r=1,g=1,b=1,a=1}, 0.10, 'Press spacebar to pause or unpause the simulation')

		time = currentTime

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	-- handle mouse movement and mouse buttons
	[SDL.SDL_ACTIVEEVENT] = function(event_struct)
		local event = event_struct.active
		if SDL.And(event.state, SDL.SDL_APPINPUTFOCUS) > 0 then
			if event.gain == 1 then
				SDL.SDL_WarpMouse(centerX, centerY)
				events[SDL.SDL_MOUSEMOTION] = mouseMotionHandler
			else
				events[SDL.SDL_MOUSEMOTION] = false
			end
		end
	end,
	[SDL.SDL_MOUSEMOTION] = mouseMotionHandler,
	[SDL.SDL_MOUSEBUTTONDOWN] = function(event_struct)
		local event = event_struct.button
		-- mouse scrool button up
		if event.button == 4 then
			solarSystemScale = solarSystemScale / 1.1
		-- mouse scrool button down
		elseif event.button == 5 then
			solarSystemScale = solarSystemScale * 1.1
		end
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

