local gl = require 'luagl'

local function createRenderBuffer(target)
	local renderBufferObject = {}
	local renderBufferObjectID = (gl.GenRenderBuffers(1))[1]
	renderBufferObject.raw = renderBufferObjectID

	local target = target or gl_enum.GL_RENDERBUFFER
	renderBufferObject.target = target

	renderBufferObject.delete = function()
		renderBufferObject.unUse()
		gl.DeleteFramebuffers(renderBufferObjectID)
	end

	renderBufferObject.use = function()
		gl.BindRenderbuffer(target, renderBufferObjectID)
	end

	renderBufferObject.unUse = function()
		gl.BindRenderbuffer(target, 0)
	end

	renderBufferObject.setAttachment = function(frameBuffer, attachment, internalFormat, width, height)
		renderBufferObject.use()
		gl.RenderbufferStorage(target, internalFormat, width, height)
		gl.FramebufferRenderbuffer(frameBuffer.target, attachment, target, renderBufferObjectID)
	end

	setmetatable(renderBufferObject, {
		__gc = renderBufferObject.delete,
	})
	table.proxy(renderBufferObject)
	return renderBufferObject
end

return {
	new = createRenderBuffer,
}