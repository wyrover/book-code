local app = require 'sdl_app'
local bit = require 'bit'
local gl = require 'luagl'
require 'GL/defs'
local surface = require 'SDL/surface'
local texture = require 'SDL/texture'

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
local shaders = {}

local function loadShader(stage, code)
	-- generate shader object ID
	local shaderObjectID = gl.CreateShader(stage)
	local shaderObject = {}
	local deleted = false
	shaderObject.raw = shaderObjectID

	shaderObject.delete = function()
		if not deleted then
			gl.DeleteShader(shaderObjectID)
			deleted = true
		end
	end

	-- replace shader code
	gl.ShaderSource(shaderObjectID, code)
	-- compile shader
	gl.CompileShader(shaderObjectID)

	local compilationStatus = ""
	local status = gl.GetShaderiv(shaderObjectID, gl_enum.GL_COMPILE_STATUS)
	if status == gl_enum.GL_FALSE then
		compilationStatus = gl.GetShaderInfoLog(shaderObjectID)
		shaderObject.delete()
		error(('Error compiling shader: "%s"'):format(tostring(compilationStatus)))
	end

	setmetatable(shaderObject, {
		__gc = shaderObject.delete,
	})
	table.proxy(shaderObject)
	return shaderObject

end

local function createShaderProgram(name)
	local shaderProgramObject = {}
	local deleted = false

	local vertexShaderFile = assert(io.open(('%s/vertex.shader'):format(name), 'r'), "Can't open vertex.shader file")
	local fragmentShaderFile = assert(io.open(('%s/fragment.shader'):format(name), 'r'), "Can't open fragment.shader file")

	local vertexShaderCode = vertexShaderFile:read('*a')
	local fragmentShaderCode = fragmentShaderFile:read('*a')

	vertexShaderFile:close()
	fragmentShaderFile:close()

	local vertexShader = loadShader(gl_enum.GL_VERTEX_SHADER, vertexShaderCode)
	local fragmentShader = loadShader(gl_enum.GL_FRAGMENT_SHADER, fragmentShaderCode)

	-- generate shader program ID
	local shaderProgramID = gl.CreateProgram()
	shaderProgramObject.raw = shaderProgramID

	shaderProgramObject.delete = function()
		if not deleted then
			gl.DeleteProgram(shaderProgramID)
			deleted = true
		end
	end

	shaderProgramObject.use = function()
		gl.UseProgram(shaderProgramID)
	end

	shaderProgramObject.uniformLocation = function(name)
		return gl.GetUniformLocation(shaderProgramID, name)
	end

	-- attach both vertex and fragment shader to this shader program
	gl.AttachShader(shaderProgramID, vertexShader.raw)
	gl.AttachShader(shaderProgramID, fragmentShader.raw)

	-- link shader program into usable state
	gl.LinkProgram(shaderProgramID)

	local linkStatus = ""
	local status = gl.GetProgramiv(shaderProgramID, gl_enum.GL_LINK_STATUS)
	if status == gl_enum.GL_FALSE then
		linkStatus = gl.GetProgramInfoLog(shaderProgramID)
		shaderProgramObject.delete()
		error(linkStatus)
	end

	shaderProgramObject.shaders = {
		vertex = vertexShader,
		fragment = fragmentShader,
	}

	setmetatable(shaderProgramObject, {
		__gc = shaderProgramObject.delete,
	})
	table.proxy(shaderProgramObject)
	return shaderProgramObject
end

local function prepareScene()
	shaders.basic = createShaderProgram('../../common/shaders/basic')
end

local function drawScene()
	shaders.basic.use()

	local vertexColor = shaders.basic.uniformLocation('vertexColor')
	gl.Uniformf(vertexColor, 1, 0, 0, 1)

	gl.Begin(gl_enum.GL_TRIANGLES)
		gl.Vertex2f(0, 1)
		gl.Vertex2f(-1, -1)
		gl.Vertex2f(1, -1)
	gl.End()
end

local events = {
	startup = function()
		local screen = surface.wrap(window.screen, false)

		gl.InitGLEW()
		assert(gl.IsSupported("GL_VERSION_3_0 GL_ARB_vertex_shader GL_ARB_fragment_shader"), "Your GPU doesn't support GLSL 3.3")

		-- hide mouse cursor
		SDL.SDL_ShowCursor(0)

		gl.ClearColor(0.1,0.1,0.1,1)

		prepareScene()
	end,
	idle = function()
		-- clears the screen
		gl.Clear(bit.bor(gl_enum.GL_COLOR_BUFFER_BIT, gl_enum.GL_DEPTH_BUFFER_BIT))

		drawScene()

		-- flip content of the back-buffer into front-buffer (screen) - similar to SDL.SDL_Flip function
		SDL.SDL_GL_SwapBuffers()
	end,
	cleanup = function()
	end,
}

app.main(window, events)

