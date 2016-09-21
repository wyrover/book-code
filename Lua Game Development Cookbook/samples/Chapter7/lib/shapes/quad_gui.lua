local vertexBuffer = require 'SDL/vertex_buffer'
local vertexArray = require 'SDL/vertex_array'

local vectorModule = require 'utils/vector'
local vector = vectorModule.new

local function generate(ccw)
	-- x, y, z
	local vertices
	local texCoords

	if ccw then
		vertices = {
			-0.5,	-0.5,	0,
			-0.5,	0.5,	0,
			0.5,	0.5,	0,
			0.5,	0.5,	0,
			0.5,	-0.5,	0,
			-0.5,	-0.5,	0,
		}
	else
		vertices = {
			0.5,	0.5,	0,
			-0.5,	0.5,	0,
			-0.5,	-0.5,	0,

			-0.5,	-0.5,	0,
			0.5,	-0.5,	0,
			0.5,	0.5,	0,
		}
	end

	-- u, v
	texCoords = {
		0, 0,
		0, 1,
		1, 1,
		1, 1,
		1, 0,
		0, 0,
	}

   	return vertices, texCoords
end

local function newQuad()
	local buffers = {}
	-- prepare vertex buffers
	buffers.vertex = vertexBuffer.new()
	buffers.texCoord = vertexBuffer.new()

	local vertices, texCoords = generate(false)

	-- fills vertex buffers with data
	-- x, y
	buffers.vertex.data(vertices)

	-- u, v
	buffers.texCoord.data(texCoords)

	-- prepare vertex array
	local shape = vertexArray.new()
	shape.use()
	-- maps vertex buffer to vertex attributes with these parameters (attribute_index, vertex_buffer_object, elements_per_primitive)
	shape.map(0, buffers.vertex, 3)
	shape.map(1, buffers.texCoord, 2)

	-- enable all vertex attributes
	shape.enableAttrib(0)
	shape.enableAttrib(1)

	return {
		buffers = buffers,
		shape = shape,
		draw = function(shader)
			shape.draw(gl_enum.GL_TRIANGLES, 0, 6)	
		end,
	}
end

return {
	new = newQuad,
}