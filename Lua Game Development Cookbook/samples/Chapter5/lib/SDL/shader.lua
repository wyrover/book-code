local gl = require 'luagl'
local uniforms = require 'SDL/uniforms'

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

	shaderProgramObject.bind = function(name, index)
		if type(name)=="table" then
			for i, n in pairs(name) do
				gl.BindAttribLocation(shaderProgramID, i, n)
			end
		else
			gl.BindAttribLocation(shaderProgramID, index, name)
		end
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
	shaderProgramObject.uniform = uniforms.init(shaderProgramID)

	setmetatable(shaderProgramObject, {
		__gc = shaderProgramObject.delete,
	})
	table.proxy(shaderProgramObject)
	return shaderProgramObject
end

return {
	load = createShaderProgram,
}