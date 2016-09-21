local gl = require 'luagl'

local function createBufferObject(usage)
	local bufferUsage = usage or gl_enum.GL_STATIC_DRAW
	local bufferObjectID = (gl.GenBuffers(1))[1]
	local bufferObject = {}
	local deleted = false
	local bufferTarget = gl_enum.GL_ARRAY_BUFFER
	bufferObject.raw = bufferObjectID

	local function use ()
		gl.BindBuffer(bufferTarget, bufferObjectID)
	end

	bufferObject.use = use

	bufferObject.unUse = function()
		gl.BindBuffer(bufferTarget, 0)
	end

	-- reserve memory for buffer object
	bufferObject.reserve = function(size, usage)
		local usage = usage or bufferUsage
		use()
		gl.BufferData(bufferTarget, size, usage)
	end

	-- set data for whole buffer object
	bufferObject.data = function(data, usage)
		local usage = usage or bufferUsage
		use()
		gl.BufferData(bufferTarget, data, usage)
	end

	-- set data for a part of buffer object
	bufferObject.subData = function(offset, data, usage)
		local usage = usage or bufferUsage
		use()
		gl.BufferSubData(bufferTarget, offset, data, usage)
	end

	bufferObject.delete = function()
		if not deleted then
			gl.DeleteBuffers({bufferObjectID})
			deleted = true
		end
	end

	setmetatable(bufferObject, {
		__gc = bufferObject.delete,
	})
	table.proxy(bufferObject)
	return bufferObject
end

return {
	new = createBufferObject,
}