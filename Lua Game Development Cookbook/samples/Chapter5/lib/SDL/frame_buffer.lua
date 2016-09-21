local gl = require 'luagl'

local function createFrameBuffer(target)
	local frameBufferObject = {}
	local frameBufferObjectID = (gl.GenFrameBuffers(1))[1]
	frameBufferObject.raw = frameBufferObjectID

	local target = target or gl_enum.GL_FRAMEBUFFER
	frameBufferObject.target = target

	frameBufferObject.delete = function()
		gl.BindFramebuffer(target, 0)
		gl.DeleteFramebuffers(frameBufferObjectID)
	end

	frameBufferObject.use = function()
		gl.BindFramebuffer(target, frameBufferObjectID)
	end

	frameBufferObject.unUse = function()
		gl.BindFramebuffer(target, 0)
	end

	frameBufferObject.status = function()
		return gl.CheckFramebufferStatus(gl_enum.GL_DRAW_FRAMEBUFFER)
	end

	frameBufferObject.statusText = function()
		local status = frameBufferObject.status()
		local msg = {
			[gl_enum.GL_FRAMEBUFFER_UNDEFINED] = "Undefined",
			[gl_enum.GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT] = "Incomplete attachment",
			[gl_enum.GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT] = "Missing attachment",
			[gl_enum.GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER] = "Incomplete Draw buffer",
			[gl_enum.GL_FRAMEBUFFER_UNSUPPORTED] = "Unsupported",
			[gl_enum.GL_FRAMEBUFFER_COMPLETE] = "Complete",
		}
		return msg[status] or 'Unknown state'
	end

	setmetatable(frameBufferObject, {
		__gc = frameBufferObject.delete,
	})
	table.proxy(frameBufferObject)
	return frameBufferObject
end

return {
	new = createFrameBuffer,
}