local vertexBuffer = require 'SDL/vertex_buffer'
local vertexArray = require 'SDL/vertex_array'

local vectorModule = require 'utils/vector'
local vector = vectorModule.new

local matrixModule = require 'utils/matrix'
local matrix = matrixModule.new
local matrixBase = matrixModule.new()

local function generate()
	-- x, y, z
	local vertices = {
		0, 0, 0,
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,
		0, 0, 0,
		0, 0, 1,
	}

	-- u, v
	local texCoords = {
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
	}

	-- x, y, z
	local normals = {
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
	}

	-- x, y, z, w
	local tangents = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
	}

   	return vertices, texCoords, normals, tangents
end

local function newAxis()
	local buffers = {}
	-- prepare vertex buffers
	buffers.vertex = vertexBuffer.new()
	buffers.texCoord = vertexBuffer.new()
	buffers.normals = vertexBuffer.new()
	buffers.tangents = vertexBuffer.new()

	local vertices, texCoords, normals, tangents = generate()

	-- fills vertex buffers with data
	-- x, y
	buffers.vertex.data(vertices)

	-- u, v
	buffers.texCoord.data(texCoords)

	-- x, y, z
	buffers.normals.data(normals)

	-- x, y, z, w
	buffers.tangents.data(tangents)

	-- prepare vertex array
	local shape = vertexArray.new()
	shape.use()
	-- maps vertex buffer to vertex attributes with these parameters (attribute_index, vertex_buffer_object, elements_per_primitive)
	shape.map(0, buffers.vertex, 3)
	shape.map(1, buffers.texCoord, 2)
	shape.map(2, buffers.normals, 3)
	shape.map(3, buffers.tangents, 4)

	-- enable all vertex attributes
	shape.enableAttrib(0)
	shape.enableAttrib(1)
	shape.enableAttrib(2)
	shape.enableAttrib(3)

	return {
		buffers = buffers,
		shape = shape,
		draw = function(shader)
			shader.uniform['object.view'] = matrixBase.scale(2,2,2)
			shader.uniform['debug'] = 1

			shader.uniform['debugColor'] = {1,0,0,1}
			shape.draw(gl_enum.GL_LINES, 0, 2)	
			shader.uniform['debugColor'] = {0,1,0,1}
			shape.draw(gl_enum.GL_LINES, 2, 2)	
			shader.uniform['debugColor'] = {0,0,1,1}
			shape.draw(gl_enum.GL_LINES, 4, 2)	

			shader.uniform['debug'] = 0
		end,
	}
end

return {
	new = newAxis,
}