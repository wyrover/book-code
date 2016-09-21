local gl = require 'luagl'

local function createVertexArray(target)
	local vertexArrayObject = {}
	local vertexArrayObjectID = (gl.GenVertexArrays(1))[1]
	vertexArrayObject.raw = vertexArrayObjectID

	local target = target or gl_enum.GL_ARRAY_BUFFER
	local deleted = false
		
	vertexArrayObject.enableAttrib = function(index)
		gl.EnableVertexAttribArray(index)
	end

	vertexArrayObject.disableAttrib = function(index)
		gl.DisableVertexArray(index)
	end
		
	local function use()
		gl.BindVertexArray(vertexArrayObjectID)
	end
	vertexArrayObject.use = use

	vertexArrayObject.map = function(index, buffer, size)
		local size = size or 0
		gl.BindBuffer(target, buffer.raw)
		gl.VertexAttribPointer(index, size, false, 0)
	end

	vertexArrayObject.draw = function(drawType, first, count)
		local first = first or 0
		local count = count or 1
		use()
		gl.DrawArrays(drawType, first, count)
	end

	vertexArrayObject.delete = function()
		if not deleted then
			gl.DeleteVertexArrays({vertexArrayObjectID})
			deleted = true
		end
	end

	setmetatable(vertexArrayObject, {
		__gc = vertexArrayObject.delete,
	})
	table.proxy(vertexArrayObject)
	return vertexArrayObject
end

return {
	new = createVertexArray,
}