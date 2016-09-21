local vertexBuffer = require 'SDL/vertex_buffer'
local vertexArray = require 'SDL/vertex_array'

local vectorModule = require 'utils/vector'
local vector = vectorModule.new

-- ccw sets inverts normal direction
local function generate(ccw)
	if type(ccw)~='boolean' then
		ccw = true
	end
	--[[
		Base vertices:

			5------ 8
		  /	|     / |
		1-------4   |
		|   |   |   |
		|   6---|-- 7
		| /     | /
		2-------3
	--]]

	local baseVertices = {
		{-0.5, 0.5, -0.5},
		{-0.5, -0.5, -0.5},
		{0.5, -0.5, -0.5},
		{0.5, 0.5, -0.5},

		{-0.5, 0.5, 0.5},
		{-0.5, -0.5, 0.5},
		{0.5, -0.5, 0.5},
		{0.5, 0.5, 0.5},
	}

	local baseUV = {
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1},
	}

	-- x, y, z
	local vertices = {}
	-- u, v
	local texCoords = {}
	-- x, y, z
	local normals = {}
	-- x, y, z, w
	local tangents = {}

	local function putVertices(indices)
		for i, index in ipairs(indices) do
			local vertex = baseVertices[index]
			table.insert(vertices, vertex[1])
			table.insert(vertices, vertex[2])
			table.insert(vertices, vertex[3])
		end
	end

	local function putUVs(indices)
		for i, index in ipairs(indices) do
			local uv = baseUV[index]
			table.insert(texCoords, uv[1])
			table.insert(texCoords, uv[2])
		end
	end

    putVertices {
		2, 1, 6, 6, 1, 5,
		8, 5, 4, 4, 5, 1,
		7, 8, 3, 3, 8, 4,
		3, 2, 7, 7, 2, 6,
		1, 2, 4, 4, 2, 3,
		8, 7, 5, 5, 7, 6,
    }

    putUVs {
		4, 3, 1, 1, 3, 2,
		2, 1, 3, 3, 1, 4,
		2, 1, 3, 3, 1, 4,
		4, 3, 1, 1, 3, 2,
		2, 1, 3, 3, 1, 4,
		2, 1, 3, 3, 1, 4,
    }

	-- 4 vertices for a quad primitive
	local primitiveElements = 3
	local totalVertices = math.floor(#vertices/3) 

   	-- build tangents
	for i=1,totalVertices do
		local faceNum = math.floor((i-1) / primitiveElements)
		local offset = faceNum*primitiveElements
		local inFaceOffset = (i-1) % primitiveElements

		local normalOffset = (i-1) * 3 + 1
		local tangentOffset = (i-1) * 4 + 1
		local uvOffset = (i-1) * 2 + 1

		local vOffset0 = (offset + (inFaceOffset) % primitiveElements)
		local vOffset1 = (offset + (inFaceOffset + 1) % primitiveElements)
		local vOffset2 = (offset + (inFaceOffset + 2) % primitiveElements)

		-- obtain vertex indices
		local iv0 = vOffset0 * 3 + 1
		local iv1 = vOffset1 * 3 + 1
		local iv2 = vOffset2 * 3 + 1

		local it0 = vOffset0 * 2 + 1
		local it1 = vOffset1 * 2 + 1
		local it2 = vOffset2 * 2 + 1

		local v0 = vector(3, {vertices[iv0], vertices[iv0+1], vertices[iv0+2]})
		local v1 = vector(3, {vertices[iv1], vertices[iv1+1], vertices[iv1+2]})
		local v2 = vector(3, {vertices[iv2], vertices[iv2+1], vertices[iv2+2]})

		local w0 = vector(2, {texCoords[it0], texCoords[it0+1]})
		local w1 = vector(2, {texCoords[it1], texCoords[it1+1]})
		local w2 = vector(2, {texCoords[it2], texCoords[it2+1]})

		local U = v1 - v0
		local V = v2 - v0
        				
		local S = w1 - w0
		local T = w2 - w0

		-- build tangent
		local r = 1/(S.x*T.y - S.y*T.x)
		local Sdir = vector(3, {T.y*U.x - S.y*V.x, T.y*U.y - S.y*V.y, T.y*U.z - S.y*V.z})*r
		local Tdir = vector(3, {S.x*V.x - T.x*U.x, S.x*V.y - T.x*U.y, S.x*V.z - T.x*U.z})*r

		local n
		if ccw then
			n = -(U.cross(V).normalize())
		else
			n = U.cross(V).normalize()
		end
		
		normals[normalOffset] = n.x
		normals[normalOffset+1] = n.y
		normals[normalOffset+2] = n.z

		-- Gram-Schmidt orthogonalize
		local tan = (Sdir - n * (n.dot(Sdir))).normalize()
		local w = (n.cross(Sdir)).dot(Tdir)
		tan = tan.resize(4)
		tan.w = (w < 0) and 1 or -1

		-- store resulting tangent vector
		tangents[tangentOffset] = tan.x
		tangents[tangentOffset+1] = tan.y
		tangents[tangentOffset+2] = tan.z
		tangents[tangentOffset+3] = tan.w
   	end

   	return vertices, texCoords, normals, tangents
end

local function newBox()
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
			shape.draw(gl_enum.GL_TRIANGLES, 0, 6*6)	
		end,
	}
end

return {
	new = newBox,
}